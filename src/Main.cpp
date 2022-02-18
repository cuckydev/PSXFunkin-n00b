/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Backend/CD.h"
#include "Backend/DLL.h"

#include <psxgpu.h>

#include <stdio.h>
#include <string.h>

namespace MainLoop
{
	// Main loop functions
	static char next_library[64];

	void __attribute__ ((noinline)) NextLibrary(const char *name)
	{
		// Set next library name
		strcpy(next_library, name);
	}
}

// Entry point
int main(int argc, char *argv[])
{
	// GPU stuff
	ResetGraph(0);

	// Initialize backend systems
	Backend::CD::Init();
	Backend::DLL::Init();

	// Run game loop
	MainLoop::NextLibrary("\\WEEK1.DLL;1");

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

		// End frame
		DrawSync(0);
		VSync(0);
	}

	// Deinitialize backend systems
	Backend::DLL::Quit();
	Backend::CD::Quit();

	return 0;
}
