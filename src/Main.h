/*
 * [PSXFunkin-n00b]
 *   Main.h
 * Author(s): Regan Green
 * Date: 03/15/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <stdint.h>

#include <stdio.h>

namespace Main
{
	// Main loop functions
	void LibraryError(const char *message);
	void LibraryNext(const char *name);

	constexpr static inline uint32_t VersionPack(uint8_t major, uint8_t minor, uint8_t patch) { return (major << 16) | (minor << 8) | patch; }
	bool VersionCheck(uint32_t version);
}
