#include <wayland-client.h>
#include "../wayland.hpp"
#include "./keyboard.hpp"
#include <cstdint>
#include <iostream>

static void keyboard_callback_keymap(void *data,struct wl_keyboard *wl_keyboard,uint32_t format,int32_t fd,uint32_t size);
static void keyboard_callback_enter(void *data,struct wl_keyboard *wl_keyboard , uint32_t serial , struct wl_surface *surface, struct wl_array *keys);
static void keyboard_callback_leave(void *data, struct wl_keyboard *wl_keyboard,uint32_t serial, struct wl_surface *surface);
static void keyboard_callback_key(void *data , struct wl_keyboard *wl_keyboard,uint32_t serial,uint32_t time,uint32_t key,uint32_t state);
static void keyboard_callback_modifier(void *data,struct wl_keyboard *wl_keyboard,uint32_t serial,uint32_t mods_depressed,uint32_t mods_latched,uint32_t mods_locked, uint32_t group);
static void keyboard_callback_repeat_info(void *data,struct wl_keyboard *wl_keyboard,int32_t rate,int32_t delay);


const struct wl_keyboard_listener wl_seat_keyboard_callback = {
     .keymap = keyboard_callback_keymap,
     .enter = keyboard_callback_enter,
     .leave = keyboard_callback_leave,
     .key = keyboard_callback_key,
     .modifiers = keyboard_callback_modifier,
     .repeat_info = keyboard_callback_repeat_info
};

static void keyboard_callback_keymap(void *data,struct wl_keyboard *wl_keyboard,uint32_t format,int32_t fd,uint32_t size){

};

static void keyboard_callback_enter(void *data,struct wl_keyboard *wl_keyboard , uint32_t serial , struct wl_surface *surface, struct wl_array *keys){
  std::cout<<"pointer focused"<<std::endl;

};

static void keyboard_callback_leave(void *data, struct wl_keyboard *wl_keyboard,uint32_t serial, struct wl_surface *surface){

};

static void keyboard_callback_key(void *data , struct wl_keyboard *wl_keyboard,uint32_t serial,uint32_t time,uint32_t key,uint32_t state){

  struct windowWaylandData *wayland_config = (windowWaylandData *)data;
  const char *action = state == WL_KEYBOARD_KEY_STATE_PRESSED ? "press" : "release";
};

static void keyboard_callback_modifier(void *data,struct wl_keyboard *wl_keyboard,uint32_t serial,uint32_t mods_depressed,uint32_t mods_latched,uint32_t mods_locked, uint32_t group){
  std::cout<<"hello"<<std::endl;
}

static void keyboard_callback_repeat_info(void *data,struct wl_keyboard *wl_keyboard,int32_t rate,int32_t delay){

};

