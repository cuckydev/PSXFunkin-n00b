/*
 * [PSXFunkin-n00b]
 *   Timer.h
 * Author(s): Regan Green
 * Date: 03/15/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "Types.h"

namespace Backend
{
	namespace Timer
	{
		// Timer functions
		void Init();
		void Quit();

		uint32_t GetTicks();
	}
}
