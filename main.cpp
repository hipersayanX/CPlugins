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

#include <memory>
#include <dlfcn.h>
#include "pluginapi.h"

int main(int argc, char *argv[])
{
    // Load the plugin, you must specify the path otherwise it will try to load
    // the library from the system.
    // RTLD_NOW will load all plugin symbols, RTLD_NODELETE will keep the
    // symbols loaded after close.
    auto plugin = dlopen("./plugin.so", RTLD_NOW | RTLD_NODELETE);

    // Load the entry and exit points of the dynamic library.
    auto pluginInit = reinterpret_cast<plugin_init_t>(dlsym(plugin, PLUGIN_INIT_STR));
    auto pluginExit = reinterpret_cast<plugin_exit_t>(dlsym(plugin, PLUGIN_EXIT_STR));

    // Close the plugin.
    dlclose(plugin);

    // Call entry point.
    int result = pluginInit(argc, argv);

    // Call exit point.
    pluginExit();

    return result;
}
