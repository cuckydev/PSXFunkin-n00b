/*
 * [PSXFunkin-n00b]
 *   DLL.cpp
 * Author(s): Regan Green
 * Date: 03/15/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Backend/DLL.h"

#include "Backend/CD.h"
#include "Hash.h"

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

			// Set resolve callback
			static uint32_t *map_data = (uint32_t*)file_map.ptr;
			static uint32_t map_size = *map_data++;

			DL_SetResolveCallback([](_DLL *dll, const char *name){
				// Hash name and compare
				uint32_t hash = Hash::FromString(name);
				uint32_t *mapp = map_data;
				for (uint32_t i = 0; i < map_size; i++, mapp += 2)
				{
					if (mapp[0] == hash)
						return (void*)mapp[1];
				}
				return (void*)0;
			});
		}

		void Quit()
		{
			// Deinitialize DLL system
		}
	}
}
