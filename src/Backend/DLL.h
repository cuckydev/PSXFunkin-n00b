/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <stdint.h>
#include <dlfcn.h>

#include <stddef.h>

namespace Backend
{
	namespace DLL
	{
		// Library class
		class Library
		{
			private:
				// PSX DLL
				_DLL *dll = nullptr;

			public:
				// Constructors and destructor
				Library() {}
				Library(void *ptr, size_t len) { Open(ptr, len); }
				~Library() { Close(); }

				// DLL functions
				Library &Open(void *ptr, size_t len);
				void Close();

				void *GetSymbol(const char *name);

				// Bool operator
				explicit operator bool() const noexcept { return dll != nullptr; }
		};

		// DLL functions
		void Init();
		void Quit();
	}
}
