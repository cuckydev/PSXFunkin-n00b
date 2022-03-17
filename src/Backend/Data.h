/*
 * [PSXFunkin-n00b]
 *   Data.h
 * Author(s): Regan Green
 * Date: 03/17/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "Hash.h"

namespace Backend
{
	namespace Data
	{
		// Data functions
		void Init();
		void Quit();

		uint32_t GetValue(const uint32_t *key);
		void SetValue(const uint32_t *key, uint32_t value);
	}
}
