#include "wayland.hpp"
#include "./wayland-callback.hpp"
#include "../../utilities/string.hpp"
#include <xkbcommon/xkbcommon.h>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <wayland-util.h>
#include <cstring>
#include <sys/mman.h>

constexpr size_t loop = sizeof(struct pointer_event)/sizeof(uint64_t);

static struct pointer_event pointer_state;
   

static void pointer_button(void *data, struct wl_pointer *pointer,
		    uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {

  
    pointer_state.event_type = WL_POINTER_EVENT_BUTTON;
    pointer_state.serial = serial;
    pointer_state.button = button,
    pointer_state.state = state;
    pointer_state.time = time;
}


static void pointer_enter (void *data, struct wl_pointer *wl_pointer,uint32_t serial,
		      struct wl_surface *surface,wl_fixed_t surface_x,wl_fixed_t surface_y){

    pointer_state.event_type = WL_POINTER_EVENT_ENTER;
    pointer_state.x = surface_x;
    pointer_state.y = surface_y;
    pointer_state.serial = serial;
    

      
};

static void pointer_leave(void *data,struct wl_pointer *wl_pointer,
		                        uint32_t serial,struct wl_surface *surface){
    

    pointer_state.event_type = WL_POINTER_EVENT_LEAVE;
    pointer_state.serial = serial;

};


static void pointer_motion(void *data,struct wl_pointer *wl_pointer,
		             uint32_t time, wl_fixed_t surface_x,wl_fixed_t surface_y){


    pointer_state.event_type = WL_POINTER_EVENT_MOTION;
    pointer_state.x = surface_x;
    pointer_state.y = surface_y;
    pointer_state.time = time;
  

};


static void pointer_axis(void *data, struct wl_pointer *wl_pointer,
		                           uint32_t time,uint32_t axis,wl_fixed_t value){

    pointer_state.event_type = WL_POINTER_EVENT_AXIS;
    pointer_state.time = time;
    pointer_state.axis[axis].valid = 1;
    pointer_state.axis[axis].value = value;
     

};

static void pointer_frame(void *data,struct wl_pointer *wl_pointer){

  //TODO: propagate the event data to the state manager pointer event queue
  flexon::memset64(&pointer_state,0,loop); 
};

static void pointer_axis_source(void *data, struct wl_pointer *wl_pointer,
                             			    uint32_t axis_source){

    pointer_state.event_type = WL_POINTER_EVENT_AXIS_SOURCE;
    pointer_state.axis_source = axis_source;
};

static void pointer_axis_stop(void *data, struct wl_pointer *wl_pointer,
		                                uint32_t time,uint32_t axis){

    pointer_state.event_type = WL_POINTER_EVENT_AXIS_STOP;
    pointer_state.time = time;
    pointer_state.axis[axis].valid = 1;
     
};

static void pointer_axis_discrete(void *data, struct wl_pointer *wl_pointer,
                                   uint32_t axis,int32_t discrete){
  

    pointer_state.event_type = WL_POINTER_EVENT_AXIS_DISCRETE;
    pointer_state.axis[axis].valid = 1;
    pointer_state.axis[axis].discrete = discrete; 
  
};

const struct wl_pointer_listener pointer_listener = {
	.enter = pointer_enter,
	.leave = pointer_leave,
	.motion = pointer_motion,
	.button = pointer_button,
	.axis = pointer_axis,
  .frame = pointer_frame,
  .axis_source = pointer_axis_source,
  .axis_stop = pointer_axis_stop,
  .axis_discrete = pointer_axis_discrete,
};


static struct xkb_state *key_xkb_state = nullptr;
static struct xkb_context *key_xkb_context = nullptr;
static struct xkb_keymap *key_xkb_keymap = nullptr;    

int keyboard_init(){
   key_xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS); 
   if(key_xkb_context == nullptr)
          return -1;
   return 0;
};

void keyboard_keymap(void *data,struct wl_keyboard *wl_keyboard,
		              uint32_t format,int32_t fd,uint32_t size){

  //TODO: propagate error to thread manager to terminate all operations
  assert(format == WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1);
  
  char *key_map = (char*)mmap(NULL,size,PROT_READ,MAP_SHARED,fd,0);
  assert(key_map != MAP_FAILED);

  struct xkb_keymap *xkb_keymap = xkb_keymap_new_from_string(
                       key_xkb_context, key_map,
                       XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);

  munmap(key_map,size);
  close(fd);

       struct xkb_state *xkb_state = xkb_state_new(xkb_keymap);
   

       xkb_keymap_unref(key_xkb_keymap);
       xkb_state_unref(key_xkb_state);

       key_xkb_keymap = xkb_keymap;
       key_xkb_state = xkb_state;

};

void keyboard_enter(void *data, struct wl_keyboard *wl_keyboard,
                   uint32_t serial,struct wl_surface *surface,
                                       struct wl_array *keys){
    if(keys->size == 0)
       return;

   return;

  char *pressed_key = nullptr;

  for(size_t size = 0 ; size < keys->size ; size++){
   pressed_key = (char*)keys->data  + size;

        char buf[128];
        xkb_keysym_t sym = xkb_state_key_get_one_sym(
                        key_xkb_state, *pressed_key + 8);
        
        xkb_keysym_get_name(sym, buf, sizeof(buf));
   
//    std::cout<<buf<<std::endl;

        xkb_state_key_get_utf8(key_xkb_state,
                        *pressed_key + 8, buf, sizeof(buf));
    std::cout<<sizeof(xkb_keysym_t)<<std::endl;

  };

};

void keyboard_leave(void *data,struct wl_keyboard *wl_keyboard,
                              uint32_t serial,struct wl_surface *surface){

};

void keyboard_key(void *data,struct wl_keyboard *wl_keyboard,
                                  uint32_t serial,uint32_t time,
                                        uint32_t key,uint32_t state){

   char buf[128];

    switch(state){   
    case 	WL_KEYBOARD_KEY_STATE_PRESSED: 
             xkb_keysym_t sym = xkb_state_key_get_one_sym(
                        key_xkb_state, key + 8);

   
        xkb_keysym_get_name(sym, buf, sizeof(buf));
         fprintf(stderr,"%s",buf);
    

        xkb_state_key_get_utf8(key_xkb_state,
                        key + 8, buf, sizeof(buf));
     break;

  };

};

void keyboard_modifiers(void *data,struct wl_keyboard *wl_keyboard,
                                uint32_t serial,uint32_t mods_depressed,
                                   uint32_t mods_latched,uint32_t mods_locked,
                                                                  uint32_t group){

};

void keyboard_repeat_info(void *data,struct wl_keyboard *wl_keyboard,
                                            int32_t rate,int32_t delay){

};


const struct wl_keyboard_listener keyboard_listener = {
 .keymap = keyboard_keymap,
 .enter = keyboard_enter,
 .leave = keyboard_leave,
 .key = keyboard_key,
 .modifiers = keyboard_modifiers,
 .repeat_info = keyboard_repeat_info
};

static void seat_capability_callback(void* data, struct wl_seat* wl_seat,
                                     uint32_t capabilities)
{

    struct window_state* bypass = (window_state*)data;
    bool keyboardPresent = capabilities & WL_SEAT_CAPABILITY_KEYBOARD;

    if (keyboardPresent && bypass->display_keyboard == NULL) {
        bypass->display_keyboard = wl_seat_get_keyboard(wl_seat);
        wl_keyboard_add_listener(bypass->display_keyboard, &keyboard_listener,nullptr);
      
    }

   if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
	  	 bypass->display_pointer = wl_seat_get_pointer(wl_seat);
	  	wl_pointer_add_listener(bypass->display_pointer, &pointer_listener,nullptr);
	 }
};




static void seat_name_callback(void* data, struct wl_seat* wl_seat, const char* name)
{
  std::cout<<"[seat name] "<<name <<std::endl;
};

const struct wl_seat_listener wl_seat_obj = {
    .capabilities = seat_capability_callback,
    .name = seat_name_callback
};


static void surface_pixel_format(void* data, wl_shm* wl_shm, uint32_t format)
{
    switch (format) {
    case WL_SHM_FORMAT_ARGB8888:

        break;
    case WL_SHM_FORMAT_XRGB8888:
        break;
    };
};

const struct wl_shm_listener wl_shm_callback = {
    .format = surface_pixel_format
};



static void wl_output_geometry(void *data, struct wl_output *wl_output, int32_t x,int32_t y,
			 int32_t physical_width,int32_t physical_height,int32_t subpixel,const char *make,
			 const char *model,int32_t transform){
//  std::cout<<"[hello] "<<model<<std::endl;
}

static void wl_output_mode(void *data, struct wl_output *wl_output, uint32_t flags,
		     int32_t width,int32_t height,int32_t refresh){
  std::cout<<"[geo]"<<width <<" : "<<height<<std::endl;
}

static void wl_output_done(void *data, struct wl_output *wl_output){}
static void wl_output_scale (void *data,struct wl_output *wl_output, int32_t factor){}
static void wl_output_name (void *data,struct wl_output *wl_output,const char *name){}
static void wl_output_description (void *data,struct wl_output *wl_output,const char *description){}


const struct wl_output_listener display_output_listener = {
  .geometry = wl_output_geometry,
  .mode = wl_output_mode,
  .done = wl_output_done,
  .scale = wl_output_scale,
  .name = wl_output_name,
  .description = wl_output_description,
};





