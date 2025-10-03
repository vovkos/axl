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

#pragma once

#define _AXL_ABR_AUTOBAUD_H

#include "axl_abr_Pch.h"

namespace axl {
namespace abr {

#define _AXL_ABR_FOURIER 0
#define _AXL_ABR_ALT     1
#define _AXL_ABR_GCD     1

//..............................................................................

struct AutoBaudRateResult {
	uint_t m_baudRate;
	uint_t m_frameBits; // data bits + parity

	operator bool() const {
		return m_frameBits != 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class AutoBaudRate {
public:
	enum Def {
		Def_MaxBaudRate        = 256000,
		Def_BaudGridCellCount  = 10000,
		Def_Horizon            = 30000000, // 3 sec
		Def_EdgeErrorTolerance = 10, // 10% off is not an edge error
#if (_AXL_ABR_ALT)
		Def_HarmonicTolerance  = 10, // 10% off is still a harmonic
#endif
#if (_AXL_ABR_GCD)
		Def_TimerPrecision     = 10, // 1 mcs
#endif
	};

protected:
	struct UartSimStats {
		size_t m_frameCount;
		size_t m_frameErrorCount;
		size_t m_edgeCount;
		size_t m_edgeOverflowCount;
		size_t m_edgeErrorCount;
#if (_AXL_ABR_ALT)
		double m_edgeError;
#endif

		void
		reset();

		void
		subtract(const UartSimStats& stats);
	};

	struct UartSim: UartSimStats {
		double m_frameTime;
		size_t m_frameEdgeCount;

		void
		reset();
	};

	struct BaudGridCell {
		struct Lt {
			bool
			operator () (
				const BaudGridCell& a,
				const BaudGridCell& b
			) const {
				return a.m_baudRate < b.m_baudRate;
			}
		};

		double m_baudRate;
		UartSim m_uartSim[5]; // 5, 6, 7, 8, 9 bits
		UartSimStats m_uartSimHorizon[5];
#if (_AXL_ABR_FOURIER)
		std::complex<double> m_fourier;
		std::complex<double> m_fourierHorizon;
#endif
	};

#if (_AXL_ABR_GCD)
	struct Interval {
		double m_time;
		double m_dtime;
	};
#endif

protected:
	double m_horizonTime;
	double m_time;
	bool m_uart;
	sl::Array<BaudGridCell> m_baudGrid;
	sl::Array<const BaudGridCell*> m_baudCandidateArray;
#if (_AXL_ABR_GCD)
	sl::Array<Interval> m_intervalArray;
#endif

public:
	// freely adjustible

	double m_horizon;
	double m_edgeErrorTolerance;
#if (_AXL_ABR_ALT)
	double m_harmonicTolerance;
#endif
#if (_AXL_ABR_GCD)
	double m_timerPrecision;
#endif

public:
	AutoBaudRate();

	bool
	isCreated() const {
		return !m_baudGrid.isEmpty();
	}

	double
	getMaxBaudRate() const {
		return !m_baudGrid.isEmpty() ? m_baudGrid.getBack().m_baudRate : 0.;
	}

	size_t
	getBaudGridCellCount() const;

	bool
	getUart() const {
		return m_uart;
	}

	bool
	create(
		uint_t maxBaudRate = Def_MaxBaudRate,
		size_t baudGridCellCount = Def_BaudGridCellCount, // pass 0 to only detect std baud rates
		bool uart = true
	);

	void
	reset(bool uart);

	void
	addEdge(double dtime);

	void
	addEdgeArray(
		const double* p,
		size_t count
	);

	AutoBaudRateResult
	calculate();

#if (_AXL_ABR_ALT)
	AutoBaudRateResult
	calculate_alt();
#endif

#if (_AXL_ABR_GCD)
	uint_t
	calculate_gcd();
#endif

#if (_AXL_DEBUG)
	void
	print();

#	if (_AXL_ABR_FOURIER)
	void
	printFourier();
#	endif
#endif

protected:
	void
	processEdge(
		double time,
		bool uart
	);

	void
	maintainHorizon();

	UartSimStats
	finalizeUartSimStats(
		uint_t bitCount,
		const UartSim& sim
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
AutoBaudRate::UartSimStats::reset() {
	m_frameCount = 0;
	m_frameErrorCount = 0;
	m_edgeCount = 0;
	m_edgeOverflowCount = 0;
	m_edgeErrorCount = 0;
#if (_AXL_ABR_ALT)
	m_edgeError = 0.;
#endif
}

inline
void
AutoBaudRate::UartSimStats::subtract(const UartSimStats& stats) {
	m_frameCount -= stats.m_frameCount;
	m_frameErrorCount -= stats.m_frameErrorCount;
	m_edgeCount -= stats.m_edgeCount;
	m_edgeOverflowCount -= stats.m_edgeOverflowCount;
	m_edgeErrorCount -= stats.m_edgeErrorCount;
#if (_AXL_ABR_ALT)
	m_edgeError -= stats.m_edgeError;
#endif
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
AutoBaudRate::UartSim::reset() {
	UartSimStats::reset();
	m_frameTime = 0.;
	m_frameEdgeCount = 0;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
AutoBaudRate::AutoBaudRate() {
	m_edgeErrorTolerance = Def_EdgeErrorTolerance / 100.;
	m_horizon = Def_Horizon;
#if (_AXL_ABR_ALT)
	m_harmonicTolerance = Def_HarmonicTolerance / 100.;
#endif
#if (_AXL_ABR_GCD)
	m_timerPrecision = Def_TimerPrecision;
#endif
	m_horizonTime = 0;
	m_time = 0;
	m_uart = false;
}

//..............................................................................

} // namespace abr
} // namespace axl
