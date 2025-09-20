#ifndef __FLEXON_WAYLAND_WINDOWMANAGER__
#define __FLEXPN_WAYLAND_WINDOWMANAGER__

#include "./xdg-shell-client-protocol.h"
#include "xdg-output.h"
#include "../flexon_commits.hpp"
#include "wayland-callback.hpp"

class waylandWM;

enum resize_types {
  WINDOW_RESIZE_NONE = 0,
  WINDOW_RESIZE_SHRINK,
  WINDOW_RESIZE_GROW,
  WINDOW_ESCAPE,
};

enum window_view{
  WINDOW_STATE_MAXIMIZED_FULL = 1,
  WINDOW_STATE_MINIMIZED,
  WINDOW_STATE_HIDDEN,
};


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
    int shm_fd;
    int stride;
    int size;

    uint32_t dwidth {0};
    uint32_t dheight {0};
    int rsize = 0;
    bool configured = false;
    bool running = true;

    uint32_t fresize = WINDOW_RESIZE_NONE;
    uint32_t windowstate = {0};

    bool resized = false;

    uint32_t *pixels;
    
 }window_state;

class waylandWM {
public:
   
    int create_commit(struct commit_wm *commit);
    void destroy();
    void dispatchEvent();

private:
    struct window_state wm_config = { 0 };
   };
#endif
