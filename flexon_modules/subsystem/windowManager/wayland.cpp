#include "./keyboard/keyboard.hpp"
#include "./xdg/xdg.hpp"

#include "wayland.hpp"
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>

#include <time.h>
#include <unistd.h>
#include <wayland-client.h>
#include <linux/input-event-codes.h>

int height = 500;
int width = 500;

int stride = width * 4;
int size = stride * height;

static int allocate_shm(void* data);
static void register_global(void* data, wl_registry* registry, uint32_t name, const char* interface, uint32_t version);
static void remove_global(void* data, wl_registry* registry, uint32_t name);
static void seat_capability_callback(void* data, struct wl_seat* wl_seat, uint32_t capabilities);
static void seat_name_callback(void* data, struct wl_seat* wl_seat, const char* name);
static void surface_pixel_format(void* data, struct wl_shm* wl_shm, uint32_t format);

static void noop() {
	// This space intentionally left blank
}
static void pointer_handle_button(void *data, struct wl_pointer *pointer,
		uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
 	 struct wl_seat *seat = (struct wl_seat *)data;

   std::cout<<"pointer"<<std::endl;
	// If the user presses the left pointer button, start an interactive move
	// of the toplevel
//	if (button == BTN_LEFT && state == WL_POINTER_BUTTON_STATE_PRESSED) {
	//	xdg_toplevel_move(xdg_toplevel, seat, serial);
	//}
}
static const struct wl_registry_listener display_registry_listener = {
    .global = register_global,
    .global_remove = remove_global
};

static const struct wl_seat_listener wl_seat_obj = {
    .capabilities = seat_capability_callback,
    .name = nullptr
};

static const struct wl_shm_listener wl_shm_callback = {
    .format = surface_pixel_format
};


static const struct wl_pointer_listener pointer_listener = {
	.enter = nullptr,
	.leave = nullptr,
	.motion = nullptr,
	.button = pointer_handle_button,
	.axis = nullptr,
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

static void register_global(void* data, wl_registry* registry, uint32_t name, const char* interface, uint32_t version)
{

    struct windowWaylandData* bypass = (windowWaylandData*)data;
    if (strcmp(interface, wl_compositor_interface.name) == 0){
       
        bypass->display_compositor = (wl_compositor*)wl_registry_bind(registry, name, &wl_compositor_interface, 1);
       
    }

    if (strcmp(interface, wl_seat_interface.name) == 0) {
        bypass->display_seat = (wl_seat*)wl_registry_bind(registry, name, &wl_seat_interface, 1);
        wl_seat_add_listener(bypass->display_seat, &wl_seat_obj, bypass);
    }
    if(strcmp(interface,wl_shm_interface.name) == 0){
      bypass->display_shm = (wl_shm *)wl_registry_bind(registry,name,&wl_shm_interface,1);
    }

       if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        bypass->display_xdg_base = (xdg_wm_base*)wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(bypass->display_xdg_base, &xdg_wm_base_callback_listener, data);
    }

    return;
}

static void remove_global(void* data, wl_registry* registry, uint32_t name)
{
    std::cout << "destroyed" << std::endl;
};

static void seat_capability_callback(void* data, struct wl_seat* wl_seat, uint32_t capabilities)
{

    struct windowWaylandData* bypass = (windowWaylandData*)data;
    bool keyboardPresent = capabilities & WL_SEAT_CAPABILITY_KEYBOARD;

    if (keyboardPresent && bypass->display_keyboard == NULL) {
        bypass->display_keyboard = wl_seat_get_keyboard(wl_seat);
        wl_keyboard_add_listener(bypass->display_keyboard, &wl_seat_keyboard_callback, data);
        std::cout << "system support keyboard;" << std::endl;
    }
   if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
	  	 bypass->display_pointer = wl_seat_get_pointer(wl_seat);
	  	wl_pointer_add_listener(bypass->display_pointer, &pointer_listener,wl_seat);
	 }
};

static void seat_name_callback(void* data, struct wl_seat* wl_seat, const char* name)
{
    std::cout << "oy yoy ko " << name << std::endl;
};

void waylandWM::create(const char* name)
{
    wayland_config.app_name = (char*)name;
    wayland_config.display = wl_display_connect(NULL);
    wayland_config.display_fd = wl_display_get_fd(wayland_config.display);
    wayland_config.registry = wl_display_get_registry(wayland_config.display);

    wl_registry_add_listener(wayland_config.registry, &display_registry_listener, &wayland_config);
    wl_display_roundtrip(wayland_config.display);


    if (wayland_config.display == NULL || wayland_config.display_compositor == NULL) {
        std::cout << "cannot connect to wayland display" << std::endl;
       return;
    }
   std::cout<<wayland_config.surface<<std::endl;

    wayland_config.surface = wl_compositor_create_surface(wayland_config.display_compositor);
   std::cout<<wayland_config.surface<<std::endl;

   if (wayland_config.surface == NULL) {
        std::cout << "cannot connect to wayland display" << std::endl;
       return;
    }
  
    wayland_config.display_xdg_surface = xdg_wm_base_get_xdg_surface(wayland_config.display_xdg_base, wayland_config.surface);
    wayland_config.xdg_surface_toplevel = xdg_surface_get_toplevel(wayland_config.display_xdg_surface);

    xdg_surface_add_listener(wayland_config.display_xdg_surface, &xdg_surface_callback_listener, &wayland_config);
    xdg_toplevel_add_listener(wayland_config.xdg_surface_toplevel, &xdg_surface_callback_listener_toplevel, &wayland_config);
    xdg_toplevel_set_title(wayland_config.xdg_surface_toplevel, "hello");
    xdg_toplevel_set_app_id(wayland_config.xdg_surface_toplevel, "hello");

    wl_surface_commit(wayland_config.surface);
    while(wl_display_dispatch(wayland_config.display) != -1 && !wayland_config.configured ){
      std::cout<<"commmiting"<<std::endl;
    };
     std::cout<<"Confgured Exit"<<std::endl;  
    int shm_fd = allocate_shm(&wayland_config);
    std::cout<<shm_fd<<std::endl;

    std::cout<<"this is display surface"<<wayland_config.surface<<std::endl;
     wl_surface_attach(wayland_config.surface, wayland_config.display_buffer, 0, 0);
     wl_surface_damage(wayland_config.surface, 0, 0, 500, 500);
     wl_surface_commit(wayland_config.surface);

    return;
}

void waylandWM::dispatchEvent()
{
  //    

    while (wl_display_dispatch(wayland_config.display) && wayland_config.running) {
            std::cout << "[FAILED] System Failed we have failed. " << std::endl;
    }
}
void waylandWM::destroy()
{

    munmap(wayland_config.pixels,size);
    xdg_toplevel_destroy(wayland_config.xdg_surface_toplevel);
    xdg_surface_destroy(wayland_config.display_xdg_surface);
    wl_surface_destroy(wayland_config.surface);
    xdg_wm_base_destroy(wayland_config.display_xdg_base);
    wl_compositor_destroy(wayland_config.display_compositor);
    wl_registry_destroy(wayland_config.registry);
    wl_display_disconnect(wayland_config.display);

    return;
}


static void
randname(char *buf)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	long r = ts.tv_nsec;
	for (int i = 0; i < 6; ++i) {
		buf[i] = 'A'+(r&15)+(r&16)*2;
		r >>= 5;
	}
}

int allocate_shm(void* data)
{

    struct windowWaylandData* by_pass = (windowWaylandData*)data;
    char name [] = "/wl_shm-XXXXXX";
    randname(name + sizeof(name) - 7);
    std::cout<<name<<std::endl;
    int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fd < 0) {
        std::cout << "[SHM] Creation failed : " << std::endl;
        return -1;
    }
    shm_unlink(name);
    int ret = ftruncate(fd, size);
    if (errno == EINTR) {
        std::cout << "[FTRUNCATE] Failed : " << std::endl;
    }
    if (ret < 0) {
      std::cout<<"FD failed"<<std::endl;
        close(fd);
        return -1;
    }
    uint32_t* pool_data = (uint32_t*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    for(int i = 0 ; i < (width *height) ; i++){
     pool_data[i] = 0xffffffff;
    }

    if(pool_data == MAP_FAILED){
    std::cout<<"mapping failed "<<std::endl; 
    }
    by_pass->display_shm_pool = wl_shm_create_pool(by_pass->display_shm, fd, size);
    by_pass->display_buffer = NULL; 
    by_pass->display_buffer = wl_shm_pool_create_buffer(by_pass->display_shm_pool, 0, width, height, stride, WL_SHM_FORMAT_ABGR8888);
    wl_shm_pool_destroy(by_pass->display_shm_pool);
    close(fd);

    if(by_pass->display_buffer == NULL){
      std::cout<<"buffer creation failed"<<std::endl;
    }
    std::cout<<"[PIXEL] Address  of pixel -> "<<pool_data<<std::endl;
    by_pass->pixels = pool_data;    
    return fd;
}
