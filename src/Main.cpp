/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Backend/CD.h"
#include "Backend/DLL.h"

#include <psxgpu.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	// GPU stuff
	ResetGraph(0);
	DrawSync(0);
	VSync(0);

	// Initialize backend systems
	printf("Initing cd...\n");
	Backend::CD::Init();
	printf("Initing dll...\n");
	Backend::DLL::Init();
	printf("System init done!\n");

	// Load DLL
	printf("Loading WEEK1.DLL...\n");
	size_t len;
	void *ptr = Backend::CD::FindReadFile("\\WEEK1.DLL;1", &len);
	Backend::DLL::Library week_dll;
	if (week_dll.Open(ptr, len))
	{
		printf("Failed to open WEEK1.DLL\n");
		return 1;
	}

	printf("Opened WEEK1.DLL!\n");
	printf("Finding init...\n");
	void (*init)(void) = (void (*)(void))week_dll.GetSymbol("Init");
	if (init == nullptr)
	{
		printf("Couldn't find init\n");
		return 1;
	}
	printf("Found Init!\n");
	DL_CALL(init);

	// Deinitialize backend systems
	Backend::DLL::Quit();
	Backend::CD::Quit();

	return 0;
}
