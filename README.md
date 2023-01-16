# C plugin interface example

This is a simple C plugin interface that combine many ideas taken from other plugable systems like [COM](https://en.wikipedia.org/wiki/Component_Object_Model) and [linux kernel modules](https://tldp.org/LDP/lkmpg/2.6/html/x121.html). The idea was to make a very simple common plugable interface that can be programmed internally using any programming language, and that allow the plugins to be loaded both in-process and on it's own process.

# Interface definitions

The interfaces are the common language used by both, the plugins and the clients using the plugins. All interfaces are defined as a C structure. IUnknown is the common interface that all interfaces must inherit, it is a simplified version of the [Microsoft IUnknown interface](https://en.wikipedia.org/wiki/IUnknown).

**iunknown.h**

```
#define IID_IUnknown 0x1L

struct IUnknown
{
    void *user_data;
    int (* query_interface)(void *user_data, uint64_t iid, void **ppv);
    uint64_t (* add_ref)(void *user_data);
    uint64_t (* release)(void *user_data);
};
```

ISayHello is an example interface that inherits IUnknown and then implements it's own methods.

**isayhello.h**

```
#define IID_ISayHello 0x2L

struct ISayHello
{
    IUnknown unknown;
    void (* say_hello)(void *user_data);
};
```

# Entry points

Each plugin must define an entry and a cleanup function that must be publically accessible outside the DLL. The 'init' function can receive custom arguments and return a result code in the same way as the classic [C main function](https://en.wikipedia.org/wiki/Entry_point#C_and_C++). The model for the entry points where inspired by the [Linux kernel modules](https://tldp.org/LDP/lkmpg/2.6/html/x121.html) design. This design also allow to load the plugin on it's own process if needed.

**plugin.cpp**

```
int helloInit(int argc, char *argv[])
{
    return 0;
}

void helloExit()
{
}

plugin_init(helloInit)
plugin_exit(helloExit)
```

In the example, we create an Object which internally implements an IUnknown interface and then pass it to the ```callHello()```, the ```callHello()``` could be for example an API function that calls a factory interface or anything else depending on what you want to do with the interface. The object must be released using the ```release()``` function, don't use ```delete```.

**plugin.cpp**

```
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
```

In the example, the IUnknown is queried for an ISayHello interface and then the ```say_hello()``` function is called.

**pluginapi.h**

```
inline void callHello(IUnknown *obj)
{
    ISayHello *salute = nullptr;
    obj->query_interface(obj->user_data,
                         IID_ISayHello,
                         reinterpret_cast<void **>(&salute));
    salute->say_hello(obj->user_data);
}
```

# The client program

Now using the plugin is as simple as calling the entry functions.

**pload.cpp**

```
#include <memory>
#include <dlfcn.h>

#include "plugininterface.h"

int main()
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
```
