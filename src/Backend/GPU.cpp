/*
 * [PSXFunkin-n00b]
 *   GPU.cpp
 * Author(s): Regan Green
 * Date: 03/15/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Backend/GPU.h"

namespace Backend
{
	namespace GPU
	{
		// Buffer structure
		#define GFX_OTLEN 16

		typedef struct
		{
			// Environment
			DISPENV disp;
			DRAWENV draw;
			
			// Buffers
			u_long ot[1 + GFX_OTLEN]; // Ordering table
			uint8_t pri[0x2000]; // Primitive buffer
			uint8_t *prip;
		} Buffer;

		static Buffer buffer[2];
		static Buffer *bufferp;

		// GPU functions
		void Init()
		{
			// Reset GPU
			SetDispMask(0);
			ResetGraph(0);

			// Initialize display environment
			SetDefDispEnv(&buffer[0].disp, 0, 0, 320, 240);
			SetDefDispEnv(&buffer[1].disp, 0, 240, 320, 240);

			// Initialize draw environment
			SetDefDrawEnv(&buffer[0].draw, 0, 240, 320, 240);
			SetDefDrawEnv(&buffer[1].draw, 0, 0, 320, 240);

			// Set video mode depending on BIOS region
			switch(*(char*)0xbfc7ff52)
			{
				case 'E':
					SetVideoMode(MODE_PAL);
					buffer[0].disp.screen.y = buffer[1].disp.screen.y = 24;
					break;
				default:
					SetVideoMode(MODE_NTSC);
					break;
			}

			// Load font
			FntLoad(960, 0);
			FntOpen(0, 8, 320, 224, 0, 100);

			// Set and initialize buffer
			bufferp = &buffer[0];

			bufferp->prip = bufferp->pri;
			ClearOTagR(bufferp->ot, 1 + GFX_OTLEN);
		}

		void Quit()
		{

		}

		void Flip()
		{
			// Enable GPU
			SetDispMask(1);

			// Use display and draw environment
			PutDispEnv(&bufferp->disp);
			PutDrawEnv(&bufferp->draw);

			// Display screen
			DrawSync(0);
			VSync(0);

			DrawOTag(&bufferp->ot[GFX_OTLEN]);
			FntFlush(-1);

			// Flip and initialize buffer
			bufferp = (bufferp == &buffer[0]) ? &buffer[1] : &buffer[0];

			bufferp->prip = bufferp->pri;
			ClearOTagR(bufferp->ot, 1 + GFX_OTLEN);
		}

		void *AllocPrim(unsigned int zindex, size_t size)
		{
			// Allocate and link primitive of given size to the given ordering table index
			void *pri = bufferp->prip;
			bufferp->prip += size;
			addPrim(&bufferp->ot[1 + zindex], pri);
			return pri;
		}
	}
}
