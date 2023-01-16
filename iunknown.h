/* C plugin interface example.
 * Copyright (C) 2023  Gonzalo Exequiel Pedone
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Web-Site: http://github.com/hipersayanX/CPlugin
 */

#pragma once

#include <stdint.h>

#define S_OK    0
#define S_FALSE 1

#define IID_IUnknown 0x1L

struct IUnknown
{
    void *user_data;
    int (* query_interface)(void *user_data, uint64_t iid, void **ppv);
    uint64_t (* add_ref)(void *user_data);
    uint64_t (* release)(void *user_data);
};
