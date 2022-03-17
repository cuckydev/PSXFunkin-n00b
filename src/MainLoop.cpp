/*
 * [PSXFunkin-n00b]
 *   MainLoop.cpp
 * Author(s): Regan Green
 * Date: 03/15/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "MainLoop.h"

#include "Backend/Timer.h"
#include "Backend/GPU.h"
#include "Backend/CD.h"
#include "Backend/DLL.h"
#include "Backend/Data.h"

#include <stdio.h>
#include <string.h>

// Functions exposed to libraries
void *DO_NOT_STRIP[] __attribute__((section(".dummy"))) = {
	// Main
	(void*)&MainLoop::NextLibrary,

	// GPU
	(void*)&Backend::GPU::Flip,
	(void*)&Backend::GPU::AllocPrim,

	// Timer
	(void*)&Backend::Timer::GetTicks,

	// Data
	(void*)&Backend::Data::GetOption
};

// Main loop functions
namespace MainLoop
{
	static char next_library[32];

	void NextLibrary(const char *name)
	{
		// Set next library name
		strcpy(next_library, name);
	}
}

// Entry point
int main(int argc, char *argv[])
{
	// Force exposed functions to link
	volatile void **dummy = (volatile void**)DO_NOT_STRIP;

	// Initialize backend systems
	Backend::GPU::Init();
	Backend::Timer::Init();
	Backend::CD::Init();
	Backend::DLL::Init();
	Backend::Data::Init();

	// Run game loop
	MainLoop::NextLibrary("\\MENU.DLL;1");

	while (1)
	{
		// Load library file
		printf("Loading library %08X %08X\n", Hash::FromString(MainLoop::next_library), "\\MENU.DLL;1"_h);
		Backend::CD::File file(MainLoop::next_library);
		if (!file)
		{
			printf("Failed to load library file\n");
			return 1;
		}
		else
		{
			// Load DLL library
			Backend::DLL::Library library(file.ptr, file.len);
			if (!library)
			{
				printf("Failed to load library\n");
				return 1;
			}
			else
			{
				// Run library
				void (*library_run)() = (void(*)())library.GetSymbol("Run");
				if (library_run == nullptr)
				{
					printf("Failed to get \"Run\" in library\n");
					return 1;
				}
				else
				{
					DL_PRE_CALL(library_run);
					library_run();
				}
			}
		}
	}

	// Deinitialize backend systems
	Backend::Data::Quit();
	Backend::DLL::Quit();
	Backend::CD::Quit();
	Backend::Timer::Quit();
	Backend::GPU::Quit();

	return 0;
}