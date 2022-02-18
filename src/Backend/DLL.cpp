/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Backend/DLL.h"

#include "Backend/CD.h"

#include <stdio.h>

namespace Backend
{
	namespace DLL
	{
		// Library class
		bool Library::Open(void *ptr, size_t len)
		{
			// Make sure we were given a file
			if (ptr == nullptr)
				return true;

			// Load DLL
			dll = DL_CreateDLL(ptr, len, (DL_ResolveMode)(RTLD_LAZY | RTLD_FREE_ON_DESTROY));
			if (dll == nullptr)
				return true;

			return false;
		}

		void Library::Close()
		{
			// Close DLL
			if (dll != nullptr)
			{
				DL_DestroyDLL(dll);
				dll = nullptr;
			}
		}

		void *Library::GetSymbol(const char *name)
		{
			return DL_GetDLLSymbol(dll, name);
		}

		// DLL functions
		void Init()
		{
			// Read .MAP file
			size_t len;
			void *ptr = CD::FindReadFile("\\MAIN.MAP;1", &len);
			if (ptr == nullptr)
			{
				printf("Failed to read MAIN.MAP");
				return;
			}
			((char*)ptr)[len] = '\0';
			printf("%s\n", ptr);

			if (!DL_ParseSymbolMap((char*)ptr, len))
			{
				printf("Failed to parse MAIN.MAP");
				return;
			}
		}

		void Quit()
		{
			// Deinitialize DLL
		}
	}
}
