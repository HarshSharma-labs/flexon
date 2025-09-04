// #define VK_ONLY_EXPORTED_PROTOTYPES
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <vector>
#include <vulkan/vulkan.h>
#include <wayland-client.h>

#ifndef NDEBUG
#define VALIDATION_LAYER_MAX 1
#endif

#define create_struct(name, type, ...) \
    type name = (type) { __VA_ARGS__ }

enum exitlevel {
    EXIT_LEVEL_ALL = 1,
    EXIT_LEVEL_GRAPHICS_PIPELINE,
    EXIT_LEVEL_RESOURCE,
    EXIT_LEVEL_RENDER_BUFFER,
    EXIT_LEVEL_RENDER_PASS,
    EXIT_LEVEL_CMD_BUFFER,
    EXIT_LEVEL_CMD_POOL,
    EXIT_LEVEL_QUEUE,
    EXIT_LEVEL_VIRTUAL_DEVICE,
    EXIT_LEVEL_INSTANCE,
    EXIT_LEVEL_NONE
};

typedef struct sync_resource {
    VkFence image_fence = VK_NULL_HANDLE;
    VkSemaphore image_start_semaphore = VK_NULL_HANDLE;
    VkSemaphore image_end_semaphore = VK_NULL_HANDLE;
  } sync_resource;

typedef struct render_buffer{
    VkImage image = VK_NULL_HANDLE;
    VkImageView image_view = VK_NULL_HANDLE;
    VkDeviceSize image_offset {0};
    sync_resource sync;

 }render_frame_buffer;


typedef struct render_state{
  
  uint32_t current_index;

  render_frame_buffer fb;
  VkBuffer cpy_buffer = VK_NULL_HANDLE;
  uint32_t *raw_pixel = nullptr;
  uint32_t *surface_pixel = nullptr;
  VkCommandBuffer current_cmd_buffer;

  VkPipelineStageFlags src_stage_mask;
  VkPipelineStageFlags dst_stage_mask;

  VkImageLayout initial_layout;
  VkImageLayout final_layout;

  VkAccessFlags src_access_flag;
  VkAccessFlags dst_access_flag;

}render_state;

typedef struct render {

    uint32_t count {3};
    int image_filled_count = 0;
    int image_view_filled_count = 0;
    int memory_count = 0;
    int fence_count = 0;
    int start_semaphore_count = 0;
    int end_semaphore_count = 0;
  
    VkDeviceMemory device_memory = VK_NULL_HANDLE;
    VkDeviceMemory staging_memory = VK_NULL_HANDLE;
    VkDeviceMemory shader_data_memory = VK_NULL_HANDLE;

    VkDeviceSize device_memory_size = 0;
    VkDeviceSize image_memory_size = 0;
    VkDeviceSize image_memory_alignment = 0;

    VkShaderModule frag_shader_module = VK_NULL_HANDLE;
    VkShaderModule vert_shader_module = VK_NULL_HANDLE;
    VkCommandBuffer *image_cmd_buffer = nullptr;

    VkPipeline graphics_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout graphics_pipeline_layout = VK_NULL_HANDLE;
    size_t vert_shader_size = 0;
    size_t frag_shader_size = 0;
    uint32_t *vert_shader_compiled_code = nullptr;
    uint32_t *frag_shader_compiled_code = nullptr;

    VkFormat image_format = VK_FORMAT_R8G8B8A8_SRGB;
    render_state state;
    render_frame_buffer *frame_buffer;
   

 } render;

typedef struct physicalDeviceConf {

    int device_score { 0 };
    const float queue_priority = 1.0f;

    uint32_t queue_family_index { 0 };
    uint32_t queue_count = 1;
    uint32_t memory_type_index = 0;
    VkMemoryPropertyFlags optimal_memory_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        | VK_MEMORY_PROPERTY_HOST_CACHED_BIT
        | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    VkMemoryPropertyFlags fallback_memory_type = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    VkPhysicalDevice device = VK_NULL_HANDLE;

    VkQueue* Queue = nullptr;

} physical_device_conf;

typedef struct VkSystem {

    uint32_t api_version { 0 };
    uint32_t default_weight { 720 };
    uint32_t default_width { 1080 };

    uint32_t requested_surface_height { 500 };
    uint32_t requested_surface_width { 500 };
    bool vksystem_down = false;

    VkInstance instance = VK_NULL_HANDLE;
    VkDevice virtual_device = VK_NULL_HANDLE;
    physical_device_conf physical_device;
    VkCommandPool cmd_pool = VK_NULL_HANDLE;
    render render_pool;

    wl_surface* vulkan_wayland_surface_ptr = nullptr;
    wl_display* vulkan_wayland_display_ptr = nullptr;

    char *vertex_shader_code = nullptr;
    char *fragment_shader_code = nullptr;

    const int device_extension_count = 1;
    const char* extension_name[1] = {
        VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
    };

} VkSystem;

class vulkan_renderer {
public:
    void initlise();
    void mount_surface(wl_surface* surface, wl_display* display,uint32_t *pixels)
    {
        vksystem.vulkan_wayland_surface_ptr = surface;
        vksystem.vulkan_wayland_display_ptr = display;
        vksystem.render_pool.state.surface_pixel = pixels;
      
    };
    void destroy_renderer();
    void render();

private:
    VkSystem vksystem;

#ifndef NDEBUG

    const int enableLayerCount = VALIDATION_LAYER_MAX;
    const char* validationLayer[VALIDATION_LAYER_MAX] = {
        "VK_LAYER_KHRONOS_validation"
    };

    bool checkValidationLayerSupport(const char** layerName,
        const int* count);

#endif

    bool filter_physical_device(VkSystem* vksystem,
        std::vector<VkPhysicalDevice>& devices_list);

    int score_device(VkSystem* vksystem,
        physical_device_conf* device_conf,
        VkPhysicalDevice which_device);

    bool find_suitable_queue_family(VkSystem* vksystem,
        physical_device_conf* device_config,
        VkPhysicalDevice& phy_device);

    bool find_suitable_memory_properties(VkSystem* vksystem, VkMemoryPropertyFlags optimal_memory_type, uint32_t memory_type_bit_requirements);

    bool load_shader(VkSystem *vksystem);
    bool create_instance(VkSystem* vksystem);
    bool select_physical_device(VkSystem* vksystem);
    bool get_queue(VkSystem* vksystem);
    bool create_virtual_device(VkSystem* vksystem);
    bool create_cmd_pool(VkSystem* vksystem);
    bool create_cmd_buffer(VkSystem* vksystem);
    bool create_render_buffer(VkSystem* vksystem);
    bool create_render_resource(VkSystem* vksytem);
    bool create_staging_buffer(VkSystem* vksystem);
    bool create_shader_module(VkSystem* vksystem);
    bool create_graphics_pipeline(VkSystem *vksystem);
    //  void destroy_graphics_pipeline(VkSystem *vksystem);
    //  void destroy_render_resource(VkSystem *vksystem);
    //  void destroy_render_buffer(VkSystem *vksystem);
    //  void destroy_render_pass(VkSystem *vksystem);
    //  void destroy_cmd_buffer(VkSystem *vksystem);
    //  void destroy_cmd_pool(VkSystem *vksystem);
    //  void destroy_queue_ptr(VkSystem *vksystem);
    //  void destroy_virtual_device(VkSystem *vksystem);
    //  void destroy_instance(VkSystem *vksystem);
    void destroy_render_resource(VkSystem* vksystem);
    void destroy_render_buffer(VkSystem* vksystem);
    void exit_vulkan(enum exitlevel level, VkSystem* vksystem);

    bool get_memory_requirements(VkSystem *vksystem);

    void transition_layout(VkSystem *vksystem);
    void toggle_state(VkSystem *vksystem);
    void render_frame(VkSystem* vksystem);
    void lets_see(VkSystem* vksystem);


    void start_cmd_buffer(VkSystem *vksystem);
    void end_cmd_buffer(VkSystem *vksystem);
};
