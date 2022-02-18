/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <psxcd.h>

#include <stdlib.h>
#include <stddef.h>

namespace Backend
{
	namespace CD
	{
		// CD file class
		class File
		{
			public:
				// File data
				void *ptr = nullptr;
				size_t len;

			public:
				// Constructors and destructor
				File() {}
				File(const char *name) { Open(name); }
				~File() { free(ptr); }

				// File functions
				File &Open(const char *name);
				void Close();

				// Bool operator
				explicit operator bool() const noexcept { return ptr != nullptr; }
		};

		// CD functions
		void Init();
		void Quit();
	}
}
