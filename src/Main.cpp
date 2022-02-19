/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Main.h"

#include "Backend/GPU.h"
#include "Backend/CD.h"
#include "Backend/DLL.h"

#include <stdio.h>
#include <string.h>

// Functions exposed to libraries
const void *const DO_NOT_STRIP[] __attribute__((section(".dummy"))) = {
	(void*)&MainLoop::NextLibrary,
	(void*)&Backend::GPU::Camera::FillRect,
	(void*)&Backend::GPU::Init,
	(void*)&Backend::GPU::Quit,
	(void*)&Backend::GPU::Flip
};

// Main loop functions
namespace MainLoop
{
	static char next_library[16];

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
	Backend::CD::Init();
	Backend::DLL::Init();

	// Run game loop
	MainLoop::NextLibrary("\\MENU.DLL;1");

	while (1)
	{
		// Load library file
		printf("Loading library %s\n", MainLoop::next_library);
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
					DL_CALL(library_run);
				}
			}
		}
	}

	// Deinitialize backend systems
	Backend::DLL::Quit();
	Backend::CD::Quit();
	Backend::GPU::Quit();

	return 0;
}
