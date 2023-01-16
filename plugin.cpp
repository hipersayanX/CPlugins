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

#include "pluginapi.h"

#include "csayhello.h"

static Object *obj = nullptr;

int helloInit(int argc, char **argv)
{
    obj = new Object;
    callHello(obj->unk());

    return 0;
}

void helloExit()
{
    obj->release();
}

// Define the entry point.
plugin_init(helloInit)
plugin_exit(helloExit)
