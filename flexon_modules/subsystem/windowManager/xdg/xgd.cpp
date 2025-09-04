#include "./xdg.hpp"
#include "../wayland.hpp"
#include "../xdg-shell-client-protocol.h"
#include <iostream>

static void xdg_wm_base_ping_callback(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial);
static void xdg_surface_callback_configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial);
static void xdg_surface_callback_toplevel_configure(void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height, struct wl_array* states);
static void xdg_surface_callback_toplevel_close(void* data, struct xdg_toplevel* xdg_toplevel);

const struct xdg_wm_base_listener xdg_wm_base_callback_listener = {
    .ping = xdg_wm_base_ping_callback
};

const struct xdg_surface_listener xdg_surface_callback_listener = {
    .configure = xdg_surface_callback_configure
};

const struct xdg_toplevel_listener xdg_surface_callback_listener_toplevel = {
    .configure = xdg_surface_callback_toplevel_configure,
    .close = xdg_surface_callback_toplevel_close
};



static void xdg_wm_base_ping_callback(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial)
{
  std::cout<<"pong"<<std::endl;
  xdg_wm_base_pong(xdg_wm_base, serial);

}

static void xdg_surface_callback_configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial)
{
  struct windowWaylandData *wayland_config = (windowWaylandData *)data;
    xdg_surface_ack_configure(xdg_surface, serial);
    if(wayland_config->configured){
     wl_surface_commit(wayland_config->surface);
   }
  std::cout<<"configured"<<std::endl;
 wayland_config->configured = true;
}

static void xdg_surface_callback_toplevel_configure(void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height, struct wl_array* states)
{
   std::cout << "width  : " << width << "\n height  : " << height << std::endl;
}

static void xdg_surface_callback_toplevel_close(void* data, struct xdg_toplevel* xdg_toplevel)
{
  struct windowWaylandData *wayland_config = (windowWaylandData *)data;
   wayland_config->running = false;
  std::cout << "closing request found" << std::endl;
}




