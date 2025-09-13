#include "./xdg.hpp"
#include "../wayland.hpp"
#include "../xdg-shell-client-protocol.h"
#include <iostream>

static void xdg_wm_base_ping_callback(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial);
static void xdg_surface_callback_configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial);
static void xdg_surface_callback_toplevel_configure(void* data, struct xdg_toplevel* xdg_toplevel, 
                                                    int32_t width, int32_t height, struct wl_array* states);
static void xdg_surface_callback_toplevel_close(void* data, struct xdg_toplevel* xdg_toplevel);


//xdg_output_listeners
static void xdg_output_logical_position (void *data, struct zxdg_output_v1 *zxdg_output_v1,int32_t x,int32_t y);
static void xdg_output_logical_size (void *data,struct zxdg_output_v1 *zxdg_output_v1, int32_t width, int32_t height);
static void xdg_output_done (void *data,struct zxdg_output_v1 *zxdg_output_v1);
static void xdg_output_name (void *data, struct zxdg_output_v1 *zxdg_output_v1, const char *name);
static void xdg_output_description (void *data,struct zxdg_output_v1 *zxdg_output_v1,const char *description);

const struct zxdg_output_v1_listener xdg_output_listener  = {
 .logical_position = xdg_output_logical_position,
 .logical_size = xdg_output_logical_size,
 .done = xdg_output_done,
 .name = xdg_output_name,
 .description = xdg_output_description,
};

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
  
  xdg_wm_base_pong(xdg_wm_base, serial);

}

static void xdg_surface_callback_configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial)
{
  struct window_state *wayland_config = (window_state *)data;
    xdg_surface_ack_configure(xdg_surface, serial);
    if(wayland_config->configured){
     wl_surface_commit(wayland_config->surface);
   }
  //std::cout<<"configured"<<std::endl;
 wayland_config->configured = true;
}

static void xdg_surface_callback_toplevel_configure(void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height, struct wl_array* states)
{
  // std::cout << "width  : " << width << "\n height  : " << height << std::endl;
}

static void xdg_surface_callback_toplevel_close(void* data, struct xdg_toplevel* xdg_toplevel)
{
  struct window_state *wayland_config = (window_state *)data;
   wayland_config->running = false;
  //std::cout << "closing request found" << std::endl;
}


static void xdg_output_logical_position (void *data, struct zxdg_output_v1 *zxdg_output_v1,int32_t x,int32_t y){
  std::cout<<"[xdg logical position] "<<x<<" : "<<y<<std::endl;
}
static void xdg_output_logical_size (void *data,struct zxdg_output_v1 *zxdg_output_v1, int32_t width, int32_t height){
 std::cout<<"[xdg logical size] "<<width<<" : "<<height<<std::endl;

}
static void xdg_output_done (void *data,struct zxdg_output_v1 *zxdg_output_v1){
  std::cout<<"[xdg done]"<<std::endl;
}
static void xdg_output_name (void *data, struct zxdg_output_v1 *zxdg_output_v1, const char *name){
  std::cout<<"[xdg name]"<<name<<std::endl;
}
static void xdg_output_description (void *data,struct zxdg_output_v1 *zxdg_output_v1,const char *description){
  std::cout<<"[xdg description]"<<description<<std::endl;
}





