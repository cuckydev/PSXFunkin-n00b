/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <stddef.h>
#include <psxcd.h>

namespace Backend
{
	namespace CD
	{
		// CD functions
		void Init();
		void Quit();

		bool FindFile(CdlFILE *fp, const char *name);
		void *ReadFile(CdlFILE *fp, size_t *size);
		void *FindReadFile(const char *name, size_t *size);
	}
}
