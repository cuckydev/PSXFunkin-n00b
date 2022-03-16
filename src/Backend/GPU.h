/*
 * [PSXFunkin-n00b]
 *   GPU.h
 * Author(s): Regan Green
 * Date: 03/15/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "Types.h"

#include <psxgpu.h>

namespace Backend
{
	namespace GPU
	{
		// GPU constants
		const unsigned int SCREEN_WIDTH = 320;
		const unsigned int SCREEN_HEIGHT = 240;
		const unsigned int SCREEN_WIDTHADD = SCREEN_WIDTH - 320;
		const unsigned int SCREEN_HEIGHTADD = SCREEN_HEIGHT - 240;

		const unsigned int SCREEN_WIDTH2 = SCREEN_WIDTH / 2;
		const unsigned int SCREEN_HEIGHT2 = SCREEN_HEIGHT / 2;
		const unsigned int SCREEN_WIDTHADD2 = SCREEN_WIDTHADD / 2;
		const unsigned int SCREEN_HEIGHTADD2 = SCREEN_HEIGHTADD / 2;

		// GPU types
		enum BlendMode
		{
			Mix = 0, //  50% background +  50% foreground
			Add = 1, // 100% background + 100% foreground
			Sub = 2, // 100% background - 100% foreground
			Low = 3  // 100% background +  25% foreground
		};

		// GPU functions
		void Init();
		void Quit();

		void Flip();

		void *AllocPrim(unsigned int zindex, size_t size);

		// Layer class
		class Layer
		{
			private:
				// Index and positioning
				unsigned int view_zindex;
				fixed_t view_x, view_y, view_zoom;

			public:
				// Layer functions
				void SetView(unsigned int zindex, fixed_t x, fixed_t y, fixed_t zoom)
				{
					view_zindex = zindex;
					view_x = x;
					view_y = y;
					view_zoom = zoom;
				}

				void FillRect(const Rect<fixed_t> &dst, uint8_t r = 0xFF, uint8_t g = 0xFF, uint8_t b = 0xFF)
				{
					// Get destination coordinates
					fixed_t left   = dst.x - view_x;
					fixed_t top    = dst.y - view_y;
					fixed_t right  = left + dst.w;
					fixed_t bottom = top  + dst.h;

					left   = (FIXED_MUL(left,   view_zoom) >> FIXED_SHIFT) + SCREEN_WIDTH2;
					top    = (FIXED_MUL(top,    view_zoom) >> FIXED_SHIFT) + SCREEN_HEIGHT2;
					right  = (FIXED_MUL(right,  view_zoom) >> FIXED_SHIFT) + SCREEN_WIDTH2;
					bottom = (FIXED_MUL(bottom, view_zoom) >> FIXED_SHIFT) + SCREEN_HEIGHT2;

					// Setup poly
					POLY_F4 *poly = (POLY_F4*)AllocPrim(view_zindex, sizeof(POLY_FT4));

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
				}

				void BlendRect(const Rect<fixed_t> &dst, BlendMode blend, uint8_t r = 0xFF, uint8_t g = 0xFF, uint8_t b = 0xFF);

				void DrawTex(const Rect<unsigned int> &src, const Rect<fixed_t> &dst, uint8_t r = 0x80, uint8_t g = 0x80, uint8_t b = 0x80);
				void BlendTex(const Rect<unsigned int> &src, const Rect<fixed_t> &dst, BlendMode blend, uint8_t r = 0x80, uint8_t g = 0x80, uint8_t b = 0x80);

				void DrawSprite(const Rect<unsigned int> &src, fixed_t x, fixed_t y, uint8_t r = 0x80, uint8_t g = 0x80, uint8_t b = 0x80);
		};
	}
}
