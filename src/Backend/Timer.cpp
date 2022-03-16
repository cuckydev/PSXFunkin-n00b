/*
 * [PSXFunkin-n00b]
 *   Timer.cpp
 * Author(s): Regan Green
 * Date: 03/15/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Backend/Timer.h"

#include <psxapi.h>
#include <psxetc.h>
#include <hwregs_c.h>

namespace Backend
{
	namespace Timer
	{
		// Timer constants
		static const uint32_t TIMER_RATE = 1000;

		// Timer state
		static volatile uint32_t ticks;

		// Timer functions
		void Init()
		{
			ticks = 0;

			EnterCriticalSection();
			ChangeClearRCnt(2, 0);
			InterruptCallback(6, [](){ ticks++; });

			// CLK/8 input, IRQ on reload, disable one-shot IRQ
			TIMER_CTRL(2)   = 0x0258;
			TIMER_RELOAD(2) = (F_CPU / 8) / TIMER_RATE;

			ExitCriticalSection();
		}

		void Quit()
		{

		}

		uint32_t GetTicks()
		{
			return ticks;
		}
	}
}
