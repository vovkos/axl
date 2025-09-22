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
#define _AXL_ABR_GCD     1

//..............................................................................

struct AutoBaudRateResult {
	uint_t m_baudRate;
	uint_t m_frameBits; // data bits + parity
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class AutoBaudRate {
public:
	enum Def {
		Def_MaxBaudRate        = 256000,
		Def_BaudGridCellCount  = 10000,
		Def_Horizon            = 30000000, // 3 sec
		Def_TimerPrecision     = 10, // 1 mcs
		Def_EdgeErrorTolerance = 10, // 10% off is not an edge error
		Def_HarmonicTolerance  = 10, // 10% off is still a harmonic
	};

protected:
	struct UartSimStats {
		size_t m_frameCount;
		size_t m_frameErrorCount;
		size_t m_edgeCount;
		size_t m_edgeOverflowCount;
		size_t m_edgeErrorCount;
		double m_edgeError;

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

	double m_timerPrecision;
	double m_edgeErrorTolerance;
	double m_harmonicTolerance;
	double m_horizon;

public:
	AutoBaudRate();

	bool
	getUart() const {
		return m_uart;
	}

	bool
	create(
		uint_t maxBaudRate = Def_MaxBaudRate,
		size_t baudGridCellCount = Def_BaudGridCellCount,
		bool uart = true
	);

	void
	reset(bool uart);

	void
	addEdge(double dtime);

	AutoBaudRateResult
	calculate();

	AutoBaudRateResult
	calculate_alt();

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
	m_edgeError = 0.;
}

inline
void
AutoBaudRate::UartSimStats::subtract(const UartSimStats& stats) {
	m_frameCount -= stats.m_frameCount;
	m_frameErrorCount -= stats.m_frameErrorCount;
	m_edgeCount -= stats.m_edgeCount;
	m_edgeOverflowCount -= stats.m_edgeOverflowCount;
	m_edgeErrorCount -= stats.m_edgeErrorCount;
	m_edgeError -= stats.m_edgeError;
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
	m_timerPrecision = Def_TimerPrecision;
	m_edgeErrorTolerance = Def_EdgeErrorTolerance / 100.;
	m_harmonicTolerance = Def_HarmonicTolerance / 100.;
	m_horizon = Def_Horizon;
	m_horizonTime = 0;
	m_time = 0;
	m_uart = false;
}

//..............................................................................

} // namespace abr
} // namespace axl
