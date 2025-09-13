#ifndef __FLEXON_WAYLAND_WINDOWMANAGER__
#define __FLEXPN_WAYLAND_WINDOWMANAGER__

#include "./xdg-shell-client-protocol.h"
#include "xdg-output.h"
#include "../flexon_commits.hpp"
#include "wayland-callback.hpp"

class waylandWM;

typedef struct window_state {
    char* app_name;

    struct wl_display* display;
    struct wl_surface* surface;
    struct wl_registry* registry;
    struct wl_shm* display_shm;
    struct wl_shm_pool* display_shm_pool;
    struct wl_compositor* display_compositor;
    struct wl_seat* display_seat;
    struct wl_output *display_output;
    struct wl_buffer* display_buffer;

    struct wl_keyboard* display_keyboard;
    struct wl_pointer* display_pointer;
   

    struct xdg_wm_base* display_xdg_base;
    struct xdg_surface* display_xdg_surface;
    struct xdg_toplevel* xdg_surface_toplevel;
   
    struct zxdg_output_v1* xdg_output;
    struct zxdg_output_manager_v1 *xdg_output_manager;

  
    waylandWM* local_instance;
 
    int display_fd;
    int display_height;
    int display_width;
    uint32_t dwidth;
    uint32_t dheight;
    bool configured = false;
    bool running = true;

    uint32_t *pixels;
    
 }window_state;

class waylandWM {
public:
   
    void create_commit(struct commit_wm *commit);
    void destroy();
    void dispatchEvent();

private:
    struct window_state wm_config = { 0 };
   };
#endif
