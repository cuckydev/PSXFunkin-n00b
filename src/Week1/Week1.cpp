/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Week1.h"

#include "Main.h"
#include "Backend/GPU.h"

#include <stdio.h>

// Game state interface
void Run()
{
	Backend::GPU::Camera scene_hud;

	while (1)
	{
		scene_hud.Set(0, 0, 0, FIXED_UNIT);

		Rect<fixed_t> porno_rect = {0, 0, FIXED_DEC(32,1), FIXED_DEC(32,1)};
		scene_hud.FillRect(porno_rect);

		Backend::GPU::Flip();
	}
}
