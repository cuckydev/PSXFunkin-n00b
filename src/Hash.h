/*
 * [PSXFunkin-n00b]
 *   Hash.h
 * Author(s): Regan Green
 * Date: 03/15/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <stdint.h>
#include <stddef.h>

/*
	String hashing based on a 32-bit implementation of the FNV-1a algorithm
	https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
*/
namespace Hash
{
	static const uint32_t FNV32_PRIME = 0x01000193;
	static const uint32_t FNV32_IV    = 0x811C9DC5;

	constexpr static inline uint32_t FromConst(const char *const literal, size_t max_length = 0xFFFFFFFF, uint32_t accumulator = FNV32_IV)
	{
		if (*literal && max_length)
			return FromConst(&literal[1], max_length - 1, (accumulator ^ static_cast<uint32_t>(*literal)) * FNV32_PRIME);
		return accumulator;
	}

	static inline uint32_t FromBuffer(const uint8_t *data, uint32_t length)
	{
		uint32_t accumulator = FNV32_IV;
		while (length-- > 0)
			accumulator = (accumulator ^ static_cast<uint32_t>(*data++)) * FNV32_PRIME;
		return accumulator;
	}

	static inline uint32_t FromString(const char *string)
	{
		uint32_t accumulator = FNV32_IV;
		while (*string != '\0')
			accumulator = (accumulator ^ static_cast<uint32_t>(*string++)) * FNV32_PRIME;
		return accumulator;
	}
}

constexpr static inline uint32_t operator"" _h(const char *const literal, size_t length)
{
	return Hash::FromConst(literal, length);
}
