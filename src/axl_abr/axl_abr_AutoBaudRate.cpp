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

//..............................................................................

bool
AutoBaudRate::create(
	uint_t maxBaudRate,
	size_t baudGridCellCount,
	double horizon,
	bool uart
) {
	static double stdBaudTable[] = {
		110.,
		300.,
		600.,
		1200.,
		2400.,
		4800.,
		9600.,
		14400.,
		19200.,
		38400.,
		56000.,
		57600.,
		115200.,
		128000.,
		153600.,
		230400.,
		256000.,
		460800.,
		921600.,
	};

	size_t count = baudGridCellCount + countof(stdBaudTable);
	bool result = m_baudGrid.setCount(count);
	if (!result)
		return false;

	BaudGridCell* b = m_baudGrid.p();
	for (size_t i = 0; i < countof(stdBaudTable); i++, b++)
		b->m_baudRate = stdBaudTable[i];

	double minBaudRate = 10; // important: not 1 -- we ignore harmonics
	double baudRateLogStep = log(maxBaudRate - minBaudRate) / baudGridCellCount;
	for (size_t i = 1; i <= baudGridCellCount; i++, b++)
		b->m_baudRate = minBaudRate + exp(i * baudRateLogStep);

	std::sort(
		m_baudGrid.p(),
		m_baudGrid.p() + count,
		BaudLt()
	);

	m_horizon = horizon;
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
			u->m_frameCount = 0;
			u->m_frameErrorCount = 0;
			u->m_edgeOverflowCount = 0;
			u->m_frameEdgeCount = 0;
			u->m_frameTime = -1;
			u->m_edgeError = 0;

			h->m_frameCount = 0;
			h->m_frameErrorCount = 0;
			h->m_edgeOverflowCount = 0;
			h->m_edgeError = 0;
		}

#if (_AXL_ABR_FOURIER)
		b->m_fourier = std::complex<double>(0, 0);
		b->m_fourierHorizon = std::complex<double>(0, 0);
#endif
	}

	m_intervalArray.clear();
	m_edgeCount = 0;
	m_time = 0;
	m_horizonTime = 0;
	m_uart = false;

	if (uart)
		addEdge(0.);
}

void
AutoBaudRate::addEdge(double dtime) {
	double time = m_time += dtime;
	m_edgeCount++;
	m_uart = !m_uart;

	if (m_time - m_horizonTime > m_horizon)
		maintainHorizon();

	if (m_uart && dtime >= 1) {
		Interval interval = {
			time,
			dtime
		};

		m_intervalArray.append(interval);
	}

#if (_AXL_ABR_FOURIER)
	double t = 2 * M_PI * time / 10000000ULL;
	double m = m_uart ? 1.0 : -1.0;
#endif

	size_t baudBinCount = m_baudGrid.getCount();
	BaudGridCell* b = m_baudGrid.p();
	for (size_t i = 0; i < baudBinCount; i++, b++) {
		double bitLength = 10000000. / b->m_baudRate;

		UartSim* u = b->m_uartSim;
		for (size_t j = 0; j < countof(b->m_uartSim); j++, u++) {
			if (u->m_frameTime >= 0) {
				double frameLength = time - u->m_frameTime;
				uint_t bitCount = j + 7; // 5 bits + start + stop
				uint_t bitIdx = std::round(frameLength / bitLength);
				if (bitIdx < u->m_frameEdgeCount) // can't go back in time
					bitIdx = u->m_frameEdgeCount;

				u->m_frameEdgeCount++;

				if (bitIdx < bitCount)
					u->m_edgeError += fabs(frameLength - bitIdx * bitLength) / bitLength;
				else {
					if (u->m_frameEdgeCount > bitCount + 1) // too many edges per frame
						u->m_edgeOverflowCount++;

					if (m_uart) // stop bit was not set
						u->m_frameErrorCount++;

					u->m_frameCount++;
					u->m_frameTime = -1;
					u->m_frameEdgeCount = 0;
				}
			}

			if (u->m_frameTime < 0 && !m_uart)
				u->m_frameTime = time;
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
				u->m_edgeOverflowCount >= h->m_edgeOverflowCount &&
				u->m_edgeError >= h->m_edgeError
			);

			u->m_frameCount -= h->m_frameCount;
			u->m_frameErrorCount -= h->m_frameErrorCount;
			u->m_edgeOverflowCount -= h->m_edgeOverflowCount;
			u->m_edgeError -= h->m_edgeError;

			h->m_frameCount = u->m_frameCount;
			h->m_frameErrorCount = u->m_frameErrorCount;
			h->m_edgeOverflowCount = u->m_edgeOverflowCount;
			h->m_edgeError = u->m_edgeError;
		}

#if (_AXL_ABR_FOURIER)
		b->m_fourier -= b->m_fourierHorizon;
		b->m_fourierHorizon = b->m_fourier;
#endif
	}

	count = m_intervalArray.getCount();
	for (size_t i = 0; i < count; i++) {
		if (m_intervalArray[i].m_time >= m_horizonTime) {
			m_intervalArray.remove(0, i);
			break;
		}
	}

	m_horizonTime = m_time;
}

bool
isHarmonic(
	double base,
	double harmonic
) {
	ASSERT(base < harmonic);
	double n = std::round(harmonic / base);
	return n >= 2 && std::fabs(harmonic - base * n) / base < 0.01; // less than 1% off
}

AutoBaudRateResult
AutoBaudRate::calculate() {
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
		for (size_t j = 0; j < countof(b->m_uartSim); j++, u++)
			if (u->m_frameCount && (
					u->m_edgeOverflowCount < bestSim->m_edgeOverflowCount ||
					u->m_edgeOverflowCount == bestSim->m_edgeOverflowCount && (
						u->m_frameErrorCount < bestSim->m_frameErrorCount ||
						u->m_frameErrorCount == bestSim->m_frameErrorCount && (
							bestBaud == b && j > bestSimIdx || // prefer more bits of the same baud
							bestSim->m_edgeError > u->m_edgeError &&
							!isHarmonic(bestBaud->m_baudRate, b->m_baudRate)
						)
					)
				)
			) {
				bestBaud = b;
				bestSim = u;
				bestSimIdx = j;
			}
	}

	AutoBaudRateResult result;
	result.m_baudRate = bestBaud->m_baudRate;
	result.m_frameBits = bestSimIdx + 5;
	return result;
}

#if (_AXL_ABR_GCD)

uint_t
AutoBaudRate::calculate_gcd(double precision) {
	if (precision < 1)
		precision = 1;

	double baudGcd = 1;

	size_t count = m_intervalArray.getCount();
	if (count) {
		baudGcd = m_intervalArray[0].m_interval;

		for (size_t i = 1; i < count; i++)
			baudGcd = approximateGcd(baudGcd, m_intervalArray[i].m_interval, precision);
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
		for (size_t j = 0; j < countof(b->m_uartSim); j++, u++)
			printf(
				"%f, %d, %d, %d, %d, %f\n",
				b->m_baudRate,
				j + 5,
				u->m_frameCount,
				u->m_frameErrorCount,
				u->m_edgeOverflowCount,
				u->m_edgeError
			);
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
