/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Backend/CD.h"

#include <stdlib.h>
#include <stdio.h>

namespace Backend
{
	namespace CD
	{
		// CD functions
		void Init()
		{
			// Initialize CD
			CdInit();
		}

		void Quit()
		{
			// Deinitialize CD
		}

		bool FindFile(CdlFILE *fp, const char *name)
		{
			// Find file on CD
			if (!CdSearchFile(fp, name))
				return true;
			return false;
		}

		void *ReadFile(CdlFILE *fp, size_t *size)
		{
			// Allocate buffer for file
			size_t len = (fp->size + 2047) & ~2047;

			void *ptr = malloc(len);
			if (ptr == nullptr)
				return nullptr;

			// Read to buffer
			CdControl(CdlSetloc, &fp->pos, 0);
			CdRead(len >> 11, (u_long*)ptr, CdlModeSpeed);
			if (CdReadSync(0, 0) < 0)
				return nullptr;

			// Return buffer and size
			if (size != nullptr)
				*size = fp->size;
			return ptr;
		}

		void *FindReadFile(const char *name, size_t *size)
		{
			// Find file
			CdlFILE fp;
			if (FindFile(&fp, name))
				return nullptr;
			
			// Read file
			return ReadFile(&fp, size);
		}
	}
}
