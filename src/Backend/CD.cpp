/*
 * [PSXFunkin-n00b]
 *   CD.cpp
 * Author(s): Regan Green
 * Date: 03/15/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Backend/CD.h"

#include <stdlib.h>
#include <stdio.h>

namespace Backend
{
	namespace CD
	{
		// CD file class
		// File functions
		File &File::Open(const char *name)
		{
			// Find file on CD
			CdlFILE fp;
			if (!CdSearchFile(&fp, name))
			{
				ptr = nullptr;
				return *this;
			}

			// Allocate buffer for file
			len = (fp.size + 2047) & ~2047;
			if ((ptr = malloc(len)) == nullptr)
				return *this;

			// Read to buffer
			CdControl(CdlSetloc, &fp.pos, 0);
			CdRead(len >> 11, (u_long*)ptr, CdlModeSpeed);
			if (CdReadSync(0, 0) < 0)
			{
				ptr = nullptr;
				return *this;
			}

			return *this;
		}

		void File::Close()
		{
			// Free buffer
			free(ptr);
		}

		// CD functions
		void Init()
		{
			// Initialize CD system
			CdInit();
		}

		void Quit()
		{
			// Deinitialize CD system
		}
	}
}
