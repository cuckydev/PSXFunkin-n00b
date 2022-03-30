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
#include <psxgte.h>
#include "GPU_inline_n.h"

#include <stdio.h>

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
				int16_t view_x, view_y;
				fixed_t view_zoom;
				MATRIX view_matrix;

			public:
				// Layer functions
				void SetView(unsigned int zindex, int16_t x, int16_t y, fixed_t zoom)
				{
					view_zindex = zindex;
					view_x = x;
					view_y = y;
					view_zoom = zoom;

					// Construct view matrix
					view_matrix.m[0][0] = zoom;
					view_matrix.m[0][1] = 0;
					view_matrix.m[0][2] = 0;
					view_matrix.m[1][0] = 0;
					view_matrix.m[1][1] = zoom;
					view_matrix.m[1][2] = 0;
					view_matrix.m[2][0] = 0;
					view_matrix.m[2][1] = 0;
					view_matrix.m[2][2] = zoom;

					view_matrix.t[0] = SCREEN_WIDTH2;
					view_matrix.t[1] = SCREEN_HEIGHT2;
					view_matrix.t[2] = 0;

					gte_SetRotMatrix(&view_matrix);
					gte_SetTransMatrix(&view_matrix);
				}

				void FillRect(const Rect<int16_t> &dst, uint8_t r = 0xFF, uint8_t g = 0xFF, uint8_t b = 0xFF)
				{
					// Get destination coordinates
					int16_t left   = dst.x;
					int16_t top    = dst.y;
					int16_t right  = left + dst.w;
					int16_t bottom = top  + dst.h;

					// Use view matrix

					// Setup poly
					POLY_F4 *poly = (POLY_F4*)AllocPrim(view_zindex, sizeof(POLY_FT4));

					setPolyF4(poly);
					setRGB0(poly, r, g, b);

					// Use GTE to transform vectors
					SVECTOR vi;
					VECTOR vo;

					vi.vx = left;
					vi.vy = top;
					gte_ldv0xy(&vi);
					gte_mvmva(1, 0, 0, 0, 0);
					gte_stlvnl0(&vo.vx);
					gte_stlvnl1(&vo.vy);
					poly->x0 = vo.vx;
					poly->y0 = vo.vy;

					vi.vx = right;
					vi.vy = top;
					gte_ldv0xy(&vi);
					gte_mvmva(1, 0, 0, 0, 0);
					gte_stlvnl0(&vo.vx);
					gte_stlvnl1(&vo.vy);
					poly->x1 = vo.vx;
					poly->y1 = vo.vy;

					vi.vx = left;
					vi.vy = bottom;
					gte_ldv0xy(&vi);
					gte_mvmva(1, 0, 0, 0, 0);
					gte_stlvnl0(&vo.vx);
					gte_stlvnl1(&vo.vy);
					poly->x2 = vo.vx;
					poly->y2 = vo.vy;

					vi.vx = right;
					vi.vy = bottom;
					gte_ldv0xy(&vi);
					gte_mvmva(1, 0, 0, 0, 0);
					gte_stlvnl0(&vo.vx);
					gte_stlvnl1(&vo.vy);
					poly->x3 = vo.vx;
					poly->y3 = vo.vy;
				}

				void BlendRect(const Rect<fixed_t> &dst, BlendMode blend, uint8_t r = 0xFF, uint8_t g = 0xFF, uint8_t b = 0xFF);

				void DrawTex(const Rect<unsigned int> &src, const Rect<fixed_t> &dst, uint8_t r = 0x80, uint8_t g = 0x80, uint8_t b = 0x80);
				void BlendTex(const Rect<unsigned int> &src, const Rect<fixed_t> &dst, BlendMode blend, uint8_t r = 0x80, uint8_t g = 0x80, uint8_t b = 0x80);

				void DrawSprite(const Rect<unsigned int> &src, fixed_t x, fixed_t y, uint8_t r = 0x80, uint8_t g = 0x80, uint8_t b = 0x80);
		};
	}
}
