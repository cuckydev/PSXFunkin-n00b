/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Week1.h"

#include "Main.h"
#include "Backend/GPU.h"

// Game state interface
void Run()
{
	while (1)
	{
		Backend::GPU::Flip();
	}
}
