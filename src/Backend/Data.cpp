/*
 * [PSXFunkin-n00b]
 *   Data.cpp
 * Author(s): Regan Green
 * Date: 03/17/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Data.h"

#include "HashMap.h"

namespace Backend
{
	namespace Data
	{
		// Data state
		static HashMap::TreeHashMap<4> data_hashmap;

		// Data functions
		void Init()
		{

		}

		void Quit()
		{

		}

		uint32_t GetValue(const uint32_t *key)
		{
			return data_hashmap.Get(key);
		}

		void SetValue(const uint32_t *key, uint32_t value)
		{
			data_hashmap.Set(key, value);
		}
	}
}
