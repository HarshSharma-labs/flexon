#ifndef __FELXON_WAYLAND_CALLBACK__
#define __FELXON_WAYLAND_CALLBACK__


enum pointer_event_type{
   WL_POINTER_EVENT_ENTER = 1,
   WL_POINTER_EVENT_LEAVE,
   WL_POINTER_EVENT_MOTION,
   WL_POINTER_EVENT_BUTTON,
   WL_POINTER_EVENT_AXIS,
   WL_POINTER_EVENT_AXIS_SOURCE,
   WL_POINTER_EVENT_AXIS_STOP,
   WL_POINTER_EVENT_AXIS_DISCRETE,
};

struct pointer_event{

  uint32_t event_type;
  uint32_t axis_source;

  wl_fixed_t x , y;
  uint32_t button,state;
  uint32_t time,serial;

  struct{
     uint32_t valid;
     wl_fixed_t value;
     int32_t discrete;
  }axis[2];
};


extern const struct wl_seat_listener wl_seat_obj;
extern const struct wl_shm_listener wl_shm_callback;
extern const struct wl_pointer_listener pointer_listener;
extern const struct wl_output_listener display_output_listener;
extern int keyboard_init();

#endif

