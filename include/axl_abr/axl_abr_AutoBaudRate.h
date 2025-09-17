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
		Def_MaxBaudRate       = 256000,
		Def_BaudGridCellCount = 10000,
		Def_Horizon           = 30000000, // 3 sec
		Def_TimerPrecision    = 10, // 1 mcs
		Def_HarmonicTolerance = 5,  // 5% off is still a harmonic
	};

protected:
	struct UartSimStats {
		size_t m_frameCount;
		size_t m_frameErrorCount;
		size_t m_edgeOverflowCount;
		double m_edgeError;
	};

	struct UartSim: UartSimStats {
		double m_frameTime;
		size_t m_frameEdgeCount;
	};

	struct BaudGridCell {
		double m_baudRate;
		UartSim m_uartSim[5]; // 5, 6, 7, 8, 9 bits
		UartSimStats m_uartSimHorizon[5];
#if (_AXL_ABR_FOURIER)
		std::complex<double> m_fourier;
		std::complex<double> m_fourierHorizon;
#endif
	};

	struct BaudLt {
		bool
		operator () (
			const BaudGridCell& a,
			const BaudGridCell& b
		) const {
			return a.m_baudRate < b.m_baudRate;
		}
	};

#if (_AXL_ABR_GCD)
	struct Interval {
		double m_time;
		double m_dtime;
	};
#endif

protected:
	double m_horizon;
	double m_horizonTime;
	double m_time;
	bool m_uart;
	sl::Array<BaudGridCell> m_baudGrid;
#if (_AXL_ABR_GCD)
	sl::Array<Interval> m_intervalArray;
#endif

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
		double horizon = Def_Horizon,
		bool uart = true
	);

	void
	reset(bool uart);

	void
	addEdge(double dtime);

	AutoBaudRateResult
	calculate(double harmonicTolerance = Def_HarmonicTolerance / 100.);

#if (_AXL_ABR_GCD)
	uint_t
	calculate_gcd(double precision = Def_TimerPrecision);
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
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
AutoBaudRate::AutoBaudRate() {
	m_horizon = 0;
	m_horizonTime = 0;
	m_time = 0;
	m_uart = false;
}

//..............................................................................

} // namespace abr
} // namespace axl
