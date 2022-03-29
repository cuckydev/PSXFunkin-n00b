/*
 * [PSXFunkin-n00b]
 *   Week1.cpp
 * Author(s): Regan Green
 * Date: 03/15/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Week1.h"

#include "Main.h"
#include "Backend/GPU.h"
#include "Backend/Timer.h"

#include <stdio.h>

// Game state interface
void Run()
{
	// Use library values
	//printf("ok i was given difficulty %d\n", Main::LibraryGetValue("difficulty"_h));

	// Debug scene
	Backend::GPU::Layer layer_hud;
	while (1)
	{
		layer_hud.SetView(0, 0, 0, FIXED_DEC(1,1));

		Rect<int16_t> porno_rect = {0, 0, 32, 32};
		uint32_t ticks = Backend::Timer::GetTicks();
		for (int i = 0; i < 512; i++)
			layer_hud.FillRect(porno_rect);
		FntPrint(0, "DELTA %d\n", Backend::Timer::GetTicks() - ticks);

		Backend::GPU::Flip();
	}
}
