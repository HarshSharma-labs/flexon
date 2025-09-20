#include "./keyboard/keyboard.hpp"
#include "./wayland-callback.hpp"
#include "./xdg/xdg.hpp"

#include "wayland.hpp"
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <wayland-client.h>


void resize(void *data){
    
    struct window_state *info = (struct window_state*)data;

    int rstride = info->dwidth * sizeof(uint32_t);  
    int rsize = rstride * info->dheight;
    info->rsize = rsize;


 switch(info->fresize){
    case WINDOW_RESIZE_GROW:
     wl_buffer_destroy(info->display_buffer);
     wl_shm_pool_resize(info->display_shm_pool,rsize);
     info->display_buffer = wl_shm_pool_create_buffer(info->display_shm_pool, 0,info->dwidth, 
                                                      info->dheight, rstride, WL_SHM_FORMAT_ABGR8888);
    break;
    case WINDOW_RESIZE_SHRINK:
      wl_shm_pool_destroy(info->display_shm_pool);
      wl_buffer_destroy(info->display_buffer);

    case WINDOW_RESIZE_NONE:

      info->display_shm_pool = wl_shm_create_pool(info->display_shm, info->shm_fd, rsize);
      info->display_buffer = wl_shm_pool_create_buffer(info->display_shm_pool, 0,info->dwidth, info->dheight,
                                                                             rstride, WL_SHM_FORMAT_ABGR8888);
      info->display_buffer = wl_shm_pool_create_buffer(info->display_shm_pool, 0,info->dwidth, info->dheight, 
                                                     rstride, WL_SHM_FORMAT_ABGR8888);
    break;
    
  }; 

  
  
    wl_surface_attach(info->surface, info->display_buffer, 0,0);
    wl_surface_damage(info->surface, 0, 0, info->dwidth, info->dheight);
    wl_surface_commit(info->surface);

  
  for(int i = 0; i < (info->dwidth * info->dheight) ; i ++){
    info->pixels[i] = 0xffffffff;
  };
          
};

static void randname(char* buf)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long r = ts.tv_nsec;
    for (int i = 0; i < 6; ++i) {
        buf[i] = 'A' + (r & 15) + (r & 16) * 2;
        r >>= 5;
    }
}

int allocate_shm(void* data)
{
    struct window_state *info = (struct window_state*)data;

    int height = info->display_height;
    int width = info->display_width;
  
    char name[] = "/wl_shm-XXXXXX";
    randname(name + sizeof(name) - 7);

    int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fd < 0) {
        std::cout << "[SHM] Creation failed : " << std::endl;
        return -1;
    }
    shm_unlink(name);
    int ret = ftruncate(fd, info->size);
    if (errno == EINTR) {
        std::cout << "[FTRUNCATE] Failed : " << std::endl;
    }
    if (ret < 0) {
        std::cout << "FD failed" << std::endl;
        close(fd);
        return -1;
    }
    info->shm_fd = fd;
    info->pixels = (uint32_t*)mmap(NULL, info->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  
    if (info->pixels == MAP_FAILED) {
        std::cout << "mapping failed " << std::endl;
    }
    resize(data);

    return 1;
}


static void register_global(void* data, wl_registry* registry, uint32_t name, const char* interface, uint32_t version)
{

    struct window_state* bypass = (window_state*)data;
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        bypass->display_compositor = (wl_compositor*)wl_registry_bind(registry, name, &wl_compositor_interface, 6);
    }

    if (strcmp(interface, wl_seat_interface.name) == 0) {
        bypass->display_seat = (wl_seat*)wl_registry_bind(registry, name, &wl_seat_interface, 7);
        wl_seat_add_listener(bypass->display_seat, &wl_seat_obj, data);
    }
    if (strcmp(interface, wl_shm_interface.name) == 0) {
        bypass->display_shm = (wl_shm*)wl_registry_bind(registry, name, &wl_shm_interface, 2);
    }

    if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        bypass->display_xdg_base = (xdg_wm_base*)wl_registry_bind(registry, name, &xdg_wm_base_interface, 5);
        xdg_wm_base_add_listener(bypass->display_xdg_base, &xdg_wm_base_callback_listener, data);
    }

    if (strcmp(interface, wl_output_interface.name) == 0) {
        bypass->display_output = (wl_output*)wl_registry_bind(registry, name,
            &wl_output_interface, 3);
        wl_output_add_listener(bypass->display_output, &display_output_listener, data);
    }

    if (strcmp(interface, zxdg_output_manager_v1_interface.name) == 0) {
       bypass->xdg_output_manager = (zxdg_output_manager_v1*)wl_registry_bind(registry, name,
             &zxdg_output_manager_v1_interface, 3);
       bypass->xdg_output = zxdg_output_manager_v1_get_xdg_output(bypass->xdg_output_manager , 
                                                                     bypass->display_output);
       zxdg_output_v1_add_listener(bypass->xdg_output, &xdg_output_listener, data);
     }

 
}

static void remove_global(void* data, wl_registry* registry, uint32_t name)
{
    std::cout << "destroyed" << std::endl;
};

static const struct wl_registry_listener display_registry_listener = {
    .global = register_global,
    .global_remove = remove_global
};



int waylandWM::create_commit(struct commit_wm* commit)
{

    if(commit->width <= 0 || commit->height <= 0){
     std::cout<<"invalid window dimensions provided.\n"
                "Exiting flexon;"<<std::endl;
      return -1;
    };

    if(keyboard_init() != 0){
       std::cout<<"[PANIC] Cannot create a keyboard system";
       return -1;
    }
    wm_config.dwidth = commit->width; 
    wm_config.dheight =  commit->height;
    

    wm_config.app_name = commit->name;
    wm_config.display = wl_display_connect(NULL);
    wm_config.display_fd = wl_display_get_fd(wm_config.display);
    wm_config.registry = wl_display_get_registry(wm_config.display);
    

    wl_registry_add_listener(wm_config.registry, &display_registry_listener, &wm_config);
    wl_display_roundtrip(wm_config.display);

    if (wm_config.display == NULL || wm_config.display_compositor == NULL) {
        std::cout << "cannot connect to wayland display" << std::endl;
        return -1;
    }

    wm_config.surface = wl_compositor_create_surface(wm_config.display_compositor);

    if (wm_config.surface == NULL) {
        std::cout << "cannot create wayland surface" << std::endl;
        return -1;
    }

    wm_config.display_xdg_surface = xdg_wm_base_get_xdg_surface(wm_config.display_xdg_base, wm_config.surface);
    wm_config.xdg_surface_toplevel = xdg_surface_get_toplevel(wm_config.display_xdg_surface);

    xdg_surface_add_listener(wm_config.display_xdg_surface, &xdg_surface_callback_listener, &wm_config);
  
    wl_surface_commit(wm_config.surface);
   
    while (wl_display_dispatch(wm_config.display) != -1 && !wm_config.configured);
 
    xdg_toplevel_add_listener(wm_config.xdg_surface_toplevel, &xdg_surface_callback_listener_toplevel, &wm_config);
    xdg_toplevel_set_title(wm_config.xdg_surface_toplevel, commit->name);
    xdg_toplevel_set_app_id(wm_config.xdg_surface_toplevel, commit->name);
    xdg_toplevel_set_min_size(wm_config.xdg_surface_toplevel, 200, 200);
    xdg_toplevel_set_max_size(wm_config.xdg_surface_toplevel,wm_config.display_width, wm_config.display_height);
    
    allocate_shm(&wm_config);
    commit->rc.fheight = wm_config.display_height;
    commit->rc.fwidth = wm_config.display_width;
    commit->rc.pixels = wm_config.pixels;
    commit->rc.display = wm_config.display;
    commit->rc.surface = wm_config.surface;
    return 0;
}

void waylandWM::dispatchEvent()
{
    while (wl_display_dispatch(wm_config.display) && wm_config.running){
      if(wm_config.resized == true){
        resize(&wm_config);
        wm_config.resized = false;
      }
    }
}
void waylandWM::destroy()
{
    close(wm_config.shm_fd);
    munmap(wm_config.pixels, wm_config.size);
    wl_shm_pool_destroy(wm_config.display_shm_pool);
    xdg_toplevel_destroy(wm_config.xdg_surface_toplevel);
    xdg_surface_destroy(wm_config.display_xdg_surface);
    wl_surface_destroy(wm_config.surface);
    xdg_wm_base_destroy(wm_config.display_xdg_base);
    wl_compositor_destroy(wm_config.display_compositor);
    wl_registry_destroy(wm_config.registry);
    wl_display_disconnect(wm_config.display);

    return;
}
