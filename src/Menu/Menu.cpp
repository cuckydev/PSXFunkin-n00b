/*
 * [PSXFunkin-n00b]
 *   Menu.cpp
 * Author(s): Regan Green
 * Date: 03/15/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Menu.h"

#include "Main.h"

// Game state interface

void Run()
{
	MainLoop::NextLibrary("\\WEEK1.DLL;1");
}
