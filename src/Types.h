/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <stdint.h>

// Rect structure
template<typename T> struct Rect { T x, y, w, h; };

// Fixed types and macros
typedef int32_t fixed_t;
typedef uint32_t ufixed_t;

#define FIXED_SHIFT (10)
#define FIXED_UNIT  (1 << FIXED_SHIFT)
#define FIXED_LAND  (FIXED_UNIT - 1)
#define FIXED_UAND  (~FIXED_LAND)

#define FIXED_DEC(d, f) ((fixed_t)(((int64_t)(d) * FIXED_UNIT) / (f)))

#define FIXED_MUL(x, y) ((fixed_t)(((int64_t)(x) * (y)) >> FIXED_SHIFT))
#define FIXED_DIV(x, y) ((fixed_t)(((int32_t)(x) * FIXED_UNIT) / (y)))

#define UFIXED_DEC(d, f) ((ufixed_t)(((uint64_t)(d) * FIXED_UNIT) / (f)))

#define UFIXED_MUL(x, y) ((ufixed_t)(((uint64_t)(x) * (y)) >> FIXED_SHIFT))
#define UFIXED_DIV(x, y) ((ufixed_t)(((uint32_t)(x) * FIXED_UNIT) / (y)))
