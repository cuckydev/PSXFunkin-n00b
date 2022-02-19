/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "Types.h"

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

		// Camera class
		class Camera
		{
			private:
				// Index and positioning
				unsigned int camera_zindex;
				fixed_t camera_x, camera_y, camera_zoom;

			public:
				// Camera functions
				void Set(unsigned int zindex, fixed_t x, fixed_t y, fixed_t zoom)
				{
					camera_zindex = zindex;
					camera_x = x;
					camera_y = y;
					camera_zoom = zoom;
				}

				void FillRect(const Rect<fixed_t> &dst, uint8_t r = 0xFF, uint8_t g = 0xFF, uint8_t b = 0xFF);
				void BlendRect(const Rect<fixed_t> &dst, BlendMode blend, uint8_t r = 0xFF, uint8_t g = 0xFF, uint8_t b = 0xFF);

				void DrawTex(const Rect<unsigned int> &src, const Rect<fixed_t> &dst, uint8_t r = 0x80, uint8_t g = 0x80, uint8_t b = 0x80);
				void BlendTex(const Rect<unsigned int> &src, const Rect<fixed_t> &dst, BlendMode blend, uint8_t r = 0x80, uint8_t g = 0x80, uint8_t b = 0x80);

				void DrawSprite(const Rect<unsigned int> &src, fixed_t x, fixed_t y, uint8_t r = 0x80, uint8_t g = 0x80, uint8_t b = 0x80);
		};

		// GPU functions
		void Init();
		void Quit();

		void Flip();
	}
}
