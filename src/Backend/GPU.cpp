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
		#define GFX_OTLEN 16

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

		// Camera class
		// Camera functions
		void Camera::FillRect(const Rect<fixed_t> &dst, uint8_t r, uint8_t g, uint8_t b)
		{
			// Get destination coordinates
			fixed_t left = dst.x - camera_x;
			fixed_t top = dst.y - camera_y;
			fixed_t right = left + dst.w;
			fixed_t bottom = top + dst.h;

			left = FIXED_MUL(left, camera_zoom) >> FIXED_SHIFT;
			top = FIXED_MUL(top, camera_zoom) >> FIXED_SHIFT;
			right = FIXED_MUL(right, camera_zoom) >> FIXED_SHIFT;
			bottom = FIXED_MUL(bottom, camera_zoom) >> FIXED_SHIFT;

			// Setup poly
			POLY_F4 *poly = (POLY_F4*)gfx_bufferp->prip;
			gfx_bufferp->prip += sizeof(POLY_FT4);

			setPolyF4(poly);
			setRGB0(poly, r, g, b);

			poly->x0 = left;
			poly->y0 = top;

			poly->x1 = right;
			poly->y1 = top;

			poly->x2 = left;
			poly->y2 = bottom;

			poly->x3 = right;
			poly->y3 = bottom;

			// Link poly to ordering table
			addPrim(gfx_bufferp->ot[camera_zindex], poly);
		}

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

			// Enable GPU
			SetDispMask(1);

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
