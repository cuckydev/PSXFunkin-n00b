/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <stdint.h>
#include <stddef.h>

namespace Saxman
{
	// Saxman constants
	static const int SAXMAN_RING_LEN = 0x1000;
	static const int SAXMAN_RING_AND = 0x0FFF;

	static const int SAXMAN_MATCH_LEN = 18;

	static const int SAXMAN_THRESHOLD = 2;

	// Saxman decode class
	class Saxman
	{
		private:
			// Saxman state
			uint8_t text_buffer[SAXMAN_RING_LEN + SAXMAN_MATCH_LEN - 1];

		public:
			// Decode function
			void Decode(const uint8_t *src, size_t src_len, uint8_t *dst)
			{
				uint8_t c;
				uint16_t i, j, k;
				uint16_t r = SAXMAN_RING_LEN - SAXMAN_MATCH_LEN;
				uint16_t flags = 0;

				for (auto &l : text_buffer)
					l = 0;

				while (1)
				{
					if (((flags >>= 1) & 0x100) == 0)
					{
						if (c = *src++, --src_len == 0) break;
						flags = 0xFF00 | c;
					}

					if (flags & 1)
					{
						if (c = *src++, --src_len == 0) break;
						*dst++ = c;
						text_buffer[r++] = c;
						r &= SAXMAN_RING_AND;
					}
					else
					{
						if (i = *src++, --src_len == 0) break;
						if (j = *src++, --src_len == 0) break;
						i |= ((j & 0xF0) << 4);
						j = (j & 0x0F) + SAXMAN_THRESHOLD;

						for (k = 0; k <= j; k++)
						{
							c = text_buffer[(i + k) & SAXMAN_RING_AND];
							*dst++ = c;
							text_buffer[r++] = c;
							r &= SAXMAN_RING_AND; 
						}
					}
				}
			}
	};
}
