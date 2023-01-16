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

#include "isayhello.h"

#define PLUGIN_INIT plugin_init_
#define PLUGIN_EXIT plugin_exit_

typedef int (* plugin_init_t)(int argc, char **argv);
typedef void (* plugin_exit_t)();

#define STR(s) #s
#define MAKE_STR(s) STR(s)
#define PLUGIN_INIT_STR MAKE_STR(PLUGIN_INIT)
#define PLUGIN_EXIT_STR MAKE_STR(PLUGIN_EXIT)

#define plugin_init(func) \
extern "C" int PLUGIN_INIT(int argc, char *argv[]) \
{ \
   return func(argc, argv); \
}
#define plugin_exit(func) \
extern "C" void PLUGIN_EXIT() \
{ \
    func(); \
}

// Do something with the object.
inline void callHello(IUnknown *obj)
{
    ISayHello *salute = nullptr;
    obj->query_interface(obj->user_data,
                         IID_ISayHello,
                         reinterpret_cast<void **>(&salute));
    salute->say_hello(obj->user_data);
}
