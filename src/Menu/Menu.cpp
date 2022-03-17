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
	// Check main version
	if (Main::VersionCheck(Main::VersionPack(FUNK_VERSION_MAJOR, FUNK_VERSION_MINOR, FUNK_VERSION_PATCH)))
	{
		return;
	}

	// Immediately run Week 1
	Main::LibrarySetValue("difficulty"_h, 2);
	Main::LibraryNext("\\WEEK1.DLL;1");
	return;
}
