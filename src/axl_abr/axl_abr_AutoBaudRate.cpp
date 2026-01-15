//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_abr_AutoBaudRate.h"

namespace axl {
namespace abr {

#if (_AXL_DEBUG)
#	define _AXL_ABR_TEST_SPECIFIC_BAUD_RATES 0
#endif

//..............................................................................

static const double g_stdBaudRateTable[] = {
#if (_AXL_ABR_TEST_SPECIFIC_BAUD_RATES)
	116101.,
#else
	110.,
	300.,
	600.,
	1200.,
	2400.,
	4800.,
	9600.,
	14400.,
	19200.,
	28800.,
	38400.,
	56000.,
	57600.,
	76800.,
	115200.,
	128000.,
	153600.,
	230400.,
	256000.,
	460800.,
	921600.,
#endif
};

//..............................................................................

double
approximateGcd(
	double a,
	double b,
	double eps
) {
	ASSERT(a >= 0 && b > 0 && eps > 0);

	if (a < b)
		std::swap(a, b);

	for (;;) {
		double r = std::remainder(a, b);
		if (r <= eps)
			return b;

		a = b;
	}
}

bool
isHarmonic(
	double base,
	double harmonic,
	double tolerance
) {
	ASSERT(base < harmonic);
	double n = std::round(harmonic / base);
	return n >= 2 && std::fabs(harmonic - base * n) / base < tolerance;
}

//..............................................................................

size_t
AutoBaudRate::getBaudGridCellCount() const {
	size_t count = m_baudGrid.getCount();
	return count > countof(g_stdBaudRateTable) ? count - countof(g_stdBaudRateTable) : 0;
}

bool
AutoBaudRate::create(
	uint_t maxBaudRate,
	size_t baudGridCellCount,
	bool uart
) {
#if (_AXL_ABR_TEST_SPECIFIC_BAUD_RATES)
	baudGridCellCount = 0;
#else
	size_t count = baudGridCellCount + countof(g_stdBaudRateTable);
	bool result =
		m_baudGrid.setCount(count) &&
		m_baudCandidateArray.reserve(count);

	if (!result)
		return false;

	BaudGridCell* b = m_baudGrid.p();
	for (size_t i = 0; i < countof(g_stdBaudRateTable); i++, b++)
		b->m_baudRate = g_stdBaudRateTable[i];

	if (baudGridCellCount) {
		double minBaudRate = 10; // important: not 1 -- we ignore harmonics
		double baudRateLogStep = log(maxBaudRate - minBaudRate) / baudGridCellCount;
		for (size_t i = 1; i <= baudGridCellCount; i++, b++)
			b->m_baudRate = minBaudRate + exp(i * baudRateLogStep);

		std::sort(
			m_baudGrid.p(),
			m_baudGrid.p() + count,
			BaudGridCell::Lt()
		);
	}
#endif

	reset(uart);
	return true;
}

void
AutoBaudRate::reset(bool uart) {
	size_t count = m_baudGrid.getCount();
	BaudGridCell* b = m_baudGrid.p();
	for (size_t i = 0; i < count; i++, b++) {
		UartSim* u = b->m_uartSim;
		UartSimStats* h = b->m_uartSimHorizon;
		for (size_t j = 0; j < countof(b->m_uartSim); j++, u++, h++) {
			u->reset();
			h->reset();
		}

#if (_AXL_ABR_FOURIER)
		b->m_fourier = std::complex<double>(0, 0);
		b->m_fourierHorizon = std::complex<double>(0, 0);
#endif
	}

#if (_AXL_ABR_GCD)
	m_intervalArray.clear();
#endif
	m_time = 0;
	m_horizonTime = 0;
	m_uart = uart;
}

void
AutoBaudRate::addEdge(double dtime) {
	bool uart = m_uart = !m_uart;

	double time;
	if (dtime > m_horizon) {
		reset(uart); // forget everything (easier than maintaining horizon)
		time = 0;
	} else {
		time = m_time += dtime;
		if (time - m_horizonTime > m_horizon) {
			maintainHorizon();
			m_horizonTime = time;
		}
	}

#if (_AXL_ABR_GCD)
	if (uart && dtime >= 1) {
		Interval interval = { time, dtime };
		m_intervalArray.append(interval);
	}
#endif

	processEdge(time, uart);
}

void
AutoBaudRate::addEdgeArray(
	const double* p,
	size_t count
) {
	const double* end = p + count;
	for (; p < end; p++) {
		double dtime = *p;
		double time = m_time += dtime;
		bool uart = m_uart = !m_uart;

#if (_AXL_ABR_GCD)
		if (uart && dtime >= 1) {
			Interval interval = { time, dtime };
			m_intervalArray.append(interval);
		}
#endif

		processEdge(time, uart);
	}

	if (m_time - m_horizonTime > m_horizon) {
		maintainHorizon();
		m_horizonTime = m_time;
	}
}

void
AutoBaudRate::processEdge(
	double time,
	bool uart
) {
#if (_AXL_ABR_FOURIER)
	double t = 2 * M_PI * time / 10000000ULL;
	double m = uart ? 1.0 : -1.0;
#endif

	size_t baudBinCount = m_baudGrid.getCount();
	BaudGridCell* b = m_baudGrid.p();
	for (size_t i = 0; i < baudBinCount; i++, b++) {
		double bitLength = 10000000. / b->m_baudRate;

		UartSim* u = b->m_uartSim;
		for (size_t j = 0; j < countof(b->m_uartSim); j++, u++)
			if (!u->m_frameEdgeCount) {
				if (uart) // start bit is not set
					u->m_frameErrorCount++;
				else {
					u->m_frameTime = time;
					u->m_frameEdgeCount++;
				}
			} else {
				double frameLength = time - u->m_frameTime;
				uint_t bitCount = j + 7; // 5 bits + start + stop
				uint_t bitIdx = std::round(frameLength / bitLength);
				if (bitIdx < u->m_frameEdgeCount) // can't go back in time
					bitIdx = u->m_frameEdgeCount;

				if (bitIdx < bitCount) {
					double delta = fabs(frameLength - bitIdx * bitLength);
					if (delta / bitLength > m_edgeErrorTolerance)
						u->m_edgeErrorCount++;
#if (_AXL_ABR_ALT)
					u->m_edgeError += delta;
#endif
					u->m_frameEdgeCount++;
				} else {
					u->m_frameCount++;
					u->m_edgeCount += u->m_frameEdgeCount;

					if (u->m_frameEdgeCount > bitCount + 1) // too many edges per frame
						u->m_edgeOverflowCount++;

					if (uart) { // stop bit was not set
						u->m_frameErrorCount++;
						u->m_frameEdgeCount = 0;
					} else {
						u->m_frameTime = m_time;
						u->m_frameEdgeCount = 1;
					}
				}
			}

#if (_AXL_ABR_FOURIER)
		double x =  fmod(t * b->m_baudRate, 2 * M_PI);
		b->m_fourier += m * std::complex<double>(cos(x), -sin(x));
#endif
	}
}

void
AutoBaudRate::maintainHorizon() {
	size_t count = m_baudGrid.getCount();
	BaudGridCell* b = m_baudGrid.p();
	for (size_t i = 0; i < count; i++, b++) {
		UartSim* u = b->m_uartSim;
		UartSimStats* h = b->m_uartSimHorizon;
		for (size_t j = 0; j < countof(b->m_uartSim); j++, u++, h++) {
			ASSERT(
				u->m_frameCount >= h->m_frameCount &&
				u->m_frameErrorCount >= h->m_frameErrorCount &&
				u->m_edgeCount >= h->m_edgeCount &&
				u->m_edgeOverflowCount >= h->m_edgeOverflowCount &&
				u->m_edgeErrorCount >= h->m_edgeErrorCount
			);
#if (_AXL_ABR_ALT)
			ASSERT(u->m_edgeError >= h->m_edgeError);
#endif
			u->subtract(*h);
			*h = *u;
		}

#if (_AXL_ABR_FOURIER)
		b->m_fourier -= b->m_fourierHorizon;
		b->m_fourierHorizon = b->m_fourier;
#endif
	}

#if (_AXL_ABR_GCD)
	count = m_intervalArray.getCount();
	for (size_t i = 0; i < count; i++) {
		if (m_intervalArray[i].m_time >= m_horizonTime) {
			m_intervalArray.remove(0, i);
			break;
		}
	}
#endif
}

AutoBaudRate::UartSimStats
AutoBaudRate::finalizeUartSimStats(
	uint_t bitCount,
	const UartSim& sim
) {
	UartSimStats stats = sim;
	if (sim.m_frameEdgeCount) {
		stats.m_frameCount++;
		if (sim.m_frameEdgeCount > bitCount + 1) // too many edges per frame
			stats.m_edgeOverflowCount++;

		if (!m_uart) // stop bit was not set
			stats.m_frameErrorCount++;

		stats.m_edgeCount += sim.m_frameEdgeCount;
	}

	return stats;
}

AutoBaudRateResult
AutoBaudRate::calculate() {
	// (1) find best error count stats

	BaudGridCell sentinelBaud = { 0	};
	sentinelBaud.m_uartSim[0].m_edgeOverflowCount = -1;
	sentinelBaud.m_uartSim[0].m_frameErrorCount = -1;

	UartSimStats bestErrorStats = { 0 };
	bestErrorStats.m_frameErrorCount = -1;
	bestErrorStats.m_edgeOverflowCount = -1;
	bestErrorStats.m_edgeErrorCount = -1;

	size_t count = m_baudGrid.getCount();
	const BaudGridCell* b = m_baudGrid.p();
	for (size_t i = 0; i < count; i++, b++) {
		const UartSim* u = b->m_uartSim;
		for (size_t j = 0; j < countof(b->m_uartSim); j++, u++) {
			UartSimStats us = finalizeUartSimStats(j + 7, *u); // 5 bits + start + stop
			if (us.m_frameCount && (
					us.m_edgeOverflowCount < bestErrorStats.m_edgeOverflowCount ||
					us.m_edgeOverflowCount == bestErrorStats.m_edgeOverflowCount && (
						us.m_frameErrorCount < bestErrorStats.m_frameErrorCount ||
						us.m_frameErrorCount == bestErrorStats.m_frameErrorCount &&
						us.m_edgeErrorCount < bestErrorStats.m_edgeErrorCount
					)
				)
			)
				bestErrorStats = us;
		}
	}

	// (2) build candidate array

	m_baudCandidateArray.clear();

	count = m_baudGrid.getCount();
	b = m_baudGrid.p();
	for (size_t i = 0; i < count; i++, b++) {
		const UartSim* u = b->m_uartSim;

		for (size_t j = 0; j < countof(b->m_uartSim); j++, u++) {
			UartSimStats us = finalizeUartSimStats(j + 7, *u); // 5 bits + start + stop
			if (us.m_frameCount &&
				us.m_edgeOverflowCount <= bestErrorStats.m_edgeOverflowCount &&
				us.m_frameErrorCount <= bestErrorStats.m_frameErrorCount &&
				us.m_edgeErrorCount <= bestErrorStats.m_edgeErrorCount
			) {
				m_baudCandidateArray.append(b);
				break;
			}
		}
	}

	if (m_baudCandidateArray.isEmpty()) {
		AutoBaudRateResult result = { 0 };
		return result;
	}

	// (3) pick the best baud

	const BaudGridCell* bestBaud = m_baudCandidateArray[0];
	double bestError = DBL_MAX;

	count = m_baudCandidateArray.getCount();
	for (size_t i = 0; i < count; i++) {
		const BaudGridCell* b = m_baudCandidateArray[i];
		double error = 0;
		for (size_t j = 0; j < count; j++) {
			double n = std::round(m_baudCandidateArray[j]->m_baudRate / b->m_baudRate);
			if (n < 1)
				n = 1;

			error += fabs(m_baudCandidateArray[j]->m_baudRate - b->m_baudRate * n);
		}

		if (error < bestError) {
			bestBaud = b;
			bestError = error;
		}
	}

	// (4) pick the best frame size

	const UartSim* bestSim = bestBaud->m_uartSim;
	size_t bestSimIdx = 0;

	const UartSim* u = bestBaud->m_uartSim;
	for (size_t j = 0; j < countof(bestBaud->m_uartSim); j++, u++) {
		UartSimStats us = finalizeUartSimStats(j + 7, *u); // 5 bits + start + stop
		if (us.m_frameCount && (
				us.m_edgeOverflowCount < bestSim->m_edgeOverflowCount ||
				us.m_edgeOverflowCount == bestSim->m_edgeOverflowCount && (
					us.m_frameErrorCount < bestSim->m_frameErrorCount ||
					us.m_frameErrorCount == bestSim->m_frameErrorCount &&
					us.m_edgeErrorCount <= bestSim->m_edgeErrorCount
				)
			)
		) {
			bestSim = u;
			bestSimIdx = j;
		}
	}

	AutoBaudRateResult result;
	result.m_baudRate = (uint_t)bestBaud->m_baudRate;
	result.m_frameBits = bestSimIdx + 5;
	return result;
}

#if (_AXL_ABR_ALT)
AutoBaudRateResult
AutoBaudRate::calculate_alt() {
	BaudGridCell sentinelBaud = { 0	};
	sentinelBaud.m_uartSim[0].m_edgeOverflowCount = -1;
	sentinelBaud.m_uartSim[0].m_frameErrorCount = -1;

	const BaudGridCell* bestBaud = &sentinelBaud;
	const UartSim* bestSim = bestBaud->m_uartSim;
	size_t bestSimIdx = 0;

	size_t count = m_baudGrid.getCount();
	const BaudGridCell* b = m_baudGrid.p();
	for (size_t i = 0; i < count; i++, b++) {
		const UartSim* u = b->m_uartSim;
		for (size_t j = 0; j < countof(b->m_uartSim); j++, u++) {
			UartSimStats us = finalizeUartSimStats(j + 7, *u); // 5 bits + start + stop
			if (us.m_frameCount && (
					us.m_edgeOverflowCount < bestSim->m_edgeOverflowCount ||
					us.m_edgeOverflowCount == bestSim->m_edgeOverflowCount && (
						us.m_frameErrorCount < bestSim->m_frameErrorCount ||
						us.m_frameErrorCount == bestSim->m_frameErrorCount && (
							us.m_edgeErrorCount < bestSim->m_edgeErrorCount ||
							us.m_edgeErrorCount == bestSim->m_edgeErrorCount && (
								bestBaud == b || // prefer more bits of the same baud
								us.m_edgeCount > us.m_frameCount &&
								bestSim->m_edgeCount > bestSim->m_frameCount &&
								us.m_edgeError / (us.m_edgeCount - us.m_frameCount) <
								bestSim->m_edgeError / (bestSim->m_edgeCount - bestSim->m_frameCount) &&
								!isHarmonic(bestBaud->m_baudRate, b->m_baudRate, m_harmonicTolerance) // ignore harmonics
							)
						)
					)
				)
			) {
				bestBaud = b;
				bestSim = u;
				bestSimIdx = j;
			}
		}
	}

	AutoBaudRateResult result;
	result.m_baudRate = (uint_t)bestBaud->m_baudRate;
	result.m_frameBits = bestBaud != &sentinelBaud ? bestSimIdx + 5 : 0;
	return result;
}
#endif

#if (_AXL_ABR_GCD)
uint_t
AutoBaudRate::calculate_gcd() {
	double baudGcd = 1;

	size_t count = m_intervalArray.getCount();
	if (count) {
		baudGcd = m_intervalArray[0].m_dtime;

		for (size_t i = 1; i < count; i++)
			baudGcd = approximateGcd(baudGcd, m_intervalArray[i].m_dtime, m_timerPrecision);
	}

	return std::round(10000000. / baudGcd);
}
#endif

#if (_AXL_DEBUG)
void
AutoBaudRate::print() {
	size_t count = m_baudGrid.getCount();
	const BaudGridCell* b = m_baudGrid.p();
	for (size_t i = 0; i < count; i++, b++) {
		const UartSim* u = b->m_uartSim;
		for (size_t j = 0; j < countof(b->m_uartSim); j++, u++) {
			UartSimStats us = finalizeUartSimStats(j + 7, *u); // 5 bits + start + stop

#	if (_AXL_ABR_ALT)
			printf(
				"%f, %d, %d, %d, %d, %d, %d, %f, %f\n",
				b->m_baudRate,
				j + 5,
				us.m_frameCount,
				us.m_frameErrorCount,
				us.m_edgeCount,
				us.m_edgeOverflowCount,
				us.m_edgeErrorCount,
				us.m_edgeError,
				us.m_edgeCount > us.m_frameCount ? us.m_edgeError / (us.m_edgeCount - us.m_frameCount) : 0.
			);
#	else
			printf(
				"%f, %d, %d, %d, %d, %d, %d\n",
				b->m_baudRate,
				j + 5,
				us.m_frameCount,
				us.m_frameErrorCount,
				us.m_edgeCount,
				us.m_edgeOverflowCount,
				us.m_edgeErrorCount
			);
#	endif
		}
	}
}

#	if (_AXL_ABR_FOURIER)
void
AutoBaudRate::printFourier() {
	static const double Eps = 1E-10;
	static const double NegInf = -100;

	size_t count = m_baudGrid.getCount();
	const BaudGridCell* b = m_baudGrid.p();
	for (size_t i = 0; i < count; i++, b++) {
		double a = std::abs(b->m_fourier);

		printf(
			"%f, %f%+fi, %f\n",
			b->m_baudRate,
			b->m_fourier.real(),
			b->m_fourier.imag(),
			a < Eps ? NegInf : log(a)
		);
	}
}
#	endif
#endif

//..............................................................................

} // namespace abr
} // namespace axl
