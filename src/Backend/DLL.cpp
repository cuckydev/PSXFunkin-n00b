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
		Library &Library::Open(void *ptr, size_t len)
		{
			// Close previous DLL
			Close();

			// Make sure we were given a file
			if (ptr == nullptr)
			{
				dll = nullptr;
				return *this;
			}

			// Load DLL
			if ((dll = DL_CreateDLL(ptr, len, RTLD_NOW)) == nullptr)
				return *this;

			return *this;
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
			// Find symbol in loaded DLL and return pointer
			return DL_GetDLLSymbol(dll, name);
		}

		// DLL functions
		void Init()
		{
			// Read .MAP file
			CD::File file_map("\\MAIN.MAP;1");
			if (!file_map)
			{
				printf("Failed to read MAIN.MAP");
				return;
			}

			// Parse .MAP file
			if (!DL_ParseSymbolMap((char*)file_map.ptr, file_map.len))
			{
				printf("Failed to parse MAIN.MAP");
				return;
			}
		}

		void Quit()
		{
			// Deinitialize DLL system
		}
	}
}
