/*
 * [PSXFunkin-n00b]
 *   Main.cpp
 * Author(s): Regan Green
 * Date: 03/15/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Main.h"

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
	(void*)&Main::LibraryError,
	(void*)&Main::LibraryNext,
	(void*)&Main::VersionCheck,

	// GPU
	(void*)&Backend::GPU::Flip,
	(void*)&Backend::GPU::AllocPrim,

	// Timer
	(void*)&Backend::Timer::GetTicks,

	// Data
	(void*)&Backend::Data::GetOption
};

// Main loop functions
namespace Main
{
	static char library_error[256];
	static char library_next[32];

	void LibraryError(const char *message)
	{
		// Set library error
		strcpy(library_error, message);
		strcpy(library_next, "\\MENU.DLL;1");
	}

	void LibraryNext(const char *name)
	{
		// Set next library name
		strcpy(library_next, name);
	}

	bool VersionCheck(uint32_t version)
	{
		if (version > VersionPack(FUNK_VERSION_MAJOR, FUNK_VERSION_MINOR, FUNK_VERSION_PATCH))
			return true;
		return false;
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
	Main::LibraryNext("\\MENU.DLL;1");

	while (1)
	{
		// Load library file
		printf("ERROR: %s\n", Main::library_error);
		Main::library_error[0] = '\0';
		printf("Loading library %s\n", Main::library_next);
		Backend::CD::File file(Main::library_next);
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