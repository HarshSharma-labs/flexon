#ifndef __FLEXON_WAYLAND_WINDOWMANAGER__
#define __FLEXPN_WAYLAND_WINDOWMANAGER__

#include "./xdg-shell-client-protocol.h"
#include <iostream>

class waylandWM;

struct windowWaylandData {
    char* app_name;

    struct wl_display* display;
    struct wl_surface* surface;
    struct wl_registry* registry;
    struct wl_shm* display_shm;
    struct wl_shm_pool* display_shm_pool;
    struct wl_compositor* display_compositor;
    struct wl_seat* display_seat;
    struct xdg_wm_base* display_xdg_base;
    struct wl_buffer* display_buffer;
    struct xdg_surface* display_xdg_surface;
    struct xdg_toplevel* xdg_surface_toplevel;
    struct wl_keyboard* display_keyboard;
    struct wl_pointer* display_pointer;

    waylandWM* local_instance;

    int display_fd;
    int display_height;
    int display_width;
    bool configured = false;
    bool running = true;

    uint32_t *pixels;
    
 };

class waylandWM {
public:
    void thread_hello(){
    std::cout<<"[hello harsh] Thread speaking"<<std::endl;
    }; 
    void create(const char* name);
    void destroy();
    void dispatchEvent();

    struct wl_surface* getSurfacePointer()
    {
        return wayland_config.surface;
    };

    struct wl_display* getDisplayPointer()
    {
        return wayland_config.display;
    };
    uint32_t *getpixel(){
     return wayland_config.pixels;
    }

private:
    struct windowWaylandData wayland_config = { 0 };
   };
#endif
