/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Backend/GPU.h"

#include <psxgpu.h>

namespace Backend
{
	namespace GPU
	{
		// Buffer structure
		#define GFX_OTLEN 1

		typedef struct
		{
			// Environment
			DISPENV disp;
			DRAWENV draw;
			
			// Buffers
			unsigned long ot[GFX_OTLEN]; // Ordering table
			unsigned char pri[0x2000]; // Primitive buffer
			unsigned char *prip;
		} Gfx_Buffer;

		static Gfx_Buffer gfx_buffer[2];
		static Gfx_Buffer *gfx_bufferp;

		// GPU functions
		void Init()
		{
			// Reset GPU
			SetDispMask(0);
			ResetGraph(0);

			// Initialize display environment
			SetDefDispEnv(&gfx_buffer[0].disp, 0, 0, 320, 240);
			SetDefDispEnv(&gfx_buffer[1].disp, 0, 240, 320, 240);

			// Initialize draw environment
			SetDefDrawEnv(&gfx_buffer[0].draw, 0, 240, 320, 240);
			SetDefDrawEnv(&gfx_buffer[1].draw, 0, 0, 320, 240);

			// Set video mode depending on BIOS region
			switch(*(char*)0xbfc7ff52)
			{
				case 'E':
					SetVideoMode(MODE_PAL);
					gfx_buffer[0].disp.screen.y = gfx_buffer[1].disp.screen.y = 24;
					break;
				default:
					SetVideoMode(MODE_NTSC);
					break;
			}

			// Load font
			FntLoad(960, 0);
			FntOpen(0, 8, 320, 224, 0, 100);

			// Set and initialize buffer
			gfx_bufferp = &gfx_buffer[0];

			gfx_bufferp->prip = gfx_bufferp->pri;
			ClearOTagR(gfx_bufferp->ot, GFX_OTLEN);
		}

		void Quit()
		{

		}

		void Flip()
		{
			// Enable GPU
			SetDispMask(1);

			// Use display and draw environment
			PutDispEnv(&gfx_bufferp->disp);
			PutDrawEnv(&gfx_bufferp->draw);

			// Display screen
			DrawSync(0);
			VSync(0);

			DrawOTag(&gfx_bufferp->ot[GFX_OTLEN - 1]);
			FntFlush(-1);

			// Flip and initialize buffer
			gfx_bufferp = (gfx_bufferp == &gfx_buffer[0]) ? &gfx_buffer[1] : &gfx_buffer[0];

			gfx_bufferp->prip = gfx_bufferp->pri;
			ClearOTagR(gfx_bufferp->ot, GFX_OTLEN);
		}
	}
}
