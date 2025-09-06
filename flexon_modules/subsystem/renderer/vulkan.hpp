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


enum memory_type{
 VK_MEMORY_TYPE_PRIMARY = 1,
 VK_MEMORY_TYPE_SHADER,
 VK_MEMORY_TYPE_TEXTURE,
};

enum memory_layout_type{
  MEM_PART_NOM_BUF = 1,
  MEM_PART_IMAGE_BUF,
  MEM_PART_UNIFORM_BUF,
  MEM_PART_VERTEX_BUF
};

enum shader_type{
 VK_SHADER_VIEW = 1,
 VK_SHADER_CIRCLE,
};


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


typedef struct vk_shader{

  struct vk_shader *next = nullptr;
  enum shader_type shader_type;

  VkShaderModule shader_vert = VK_NULL_HANDLE;
  VkShaderModule shader_frag = VK_NULL_HANDLE;

  size_t code_size_vert = 0;
  size_t code_size_frag = 0;

  uint32_t *compiled_code_frag = nullptr;
  uint32_t *compiled_code_vert = nullptr;

}vk_shader;


typedef struct render_state{
  
  uint32_t current_index = {0};
  uint32_t *raw_pixel = nullptr;
  uint32_t *surface_pixel = nullptr;

  render_frame_buffer fb;
  vk_shader *shader = nullptr;
  VkPipeline pipeline = VK_NULL_HANDLE;
  VkBuffer cpy_buffer = VK_NULL_HANDLE;
  VkCommandBuffer current_cmd_buffer;

  VkPipelineStageFlags src_stage_mask;
  VkPipelineStageFlags dst_stage_mask;

  VkImageLayout initial_layout;
  VkImageLayout final_layout;

  VkAccessFlags src_access_flag;
  VkAccessFlags dst_access_flag;

}render_state;

typedef struct vk_mem_layout{

  struct vk_mem_layout *next = nullptr;
  enum memory_layout_type part_type;
  VkDeviceSize memory_offset = 0;
  VkDeviceSize part_size = 0;
  void *bind_member = nullptr;

}vk_mem_layout;

typedef struct vk_memory{

  // define the purpose for which the memory will be used
  enum memory_type memory_usage;
  // vk_memory will be a linked list;
  struct vk_memory *next = nullptr;  
  // memory handle of the system
  VkDeviceMemory memory = VK_NULL_HANDLE;
  // defines the total memory size required by resource;
  VkDeviceSize memory_size = 0;
    // memory type bits of all entity combined
  uint32_t memory_type_bit = 0;
  //used to track the current layout and attach next to it;
  vk_mem_layout *current = nullptr;
  // memory layout depends on how much sub-allocation we want;
  vk_mem_layout *memory_layout = nullptr;

}vk_memory;

enum pipeline_type{
    VK_PIPELINE_TYPE_PRIMARY,
    VK_PIPELINE_TYPE_TEXT,
    VK_PIPELINE_TYPE_SVG
};

typedef struct vk_pipeline{

  enum pipeline_type type;
  struct vk_pipeline *next = nullptr;
  VkPipeline pipeline = VK_NULL_HANDLE;
  VkPipelineLayout layout = VK_NULL_HANDLE;

}vk_pipeline;

typedef struct render {

    uint32_t count {3};
 
    VkBuffer staging_buffer;

    vk_memory *memory = nullptr;
    vk_shader *shader = nullptr;
    vk_pipeline *pipeline = nullptr;

    render_frame_buffer *frame_buffer = nullptr;
    VkCommandBuffer *image_cmd_buffer = nullptr;
    
    VkPipeline graphics_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout graphics_pipeline_layout = VK_NULL_HANDLE;
    
  
    VkFormat image_format = VK_FORMAT_R8G8B8A8_SRGB;
  
      

 } render;

typedef struct physicalDeviceConf {

    int device_score { 0 };
    const float queue_priority = 1.0f;

    uint32_t queue_family_index { 0 };
    uint32_t queue_count = 1;
    uint32_t memory_type_index = 0;
    VkMemoryPropertyFlags required_memory_type = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    VkPhysicalDevice device = VK_NULL_HANDLE;

    VkQueue* Queue = nullptr;

} physical_device_conf;

typedef struct VkSystem {

    uint32_t api_version { 0 };
    uint32_t default_weight { 720 };
    uint32_t default_width { 1080 };

    uint32_t surface_height { 500 };
    uint32_t surface_width { 500 };
    bool vksystem_down = false;

   
    vk_memory *mem_record_state = nullptr;
    vk_shader *shader_record_state = nullptr;
    vk_pipeline *pipeline_record_state = nullptr;

    wl_surface* vulkan_wayland_surface_ptr = nullptr;
    wl_display* vulkan_wayland_display_ptr = nullptr;


    VkInstance instance = VK_NULL_HANDLE;
    VkDevice virtual_device = VK_NULL_HANDLE;
    physical_device_conf physical_device;
    VkCommandPool cmd_pool = VK_NULL_HANDLE;
    render render_pool;
    render_state state;

    const int device_extension_count = 1;
    const char* extension_name[1] = {
        VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
 };

} VkSystem;

class vulkan_renderer {
public:
    void initlise();
    void mount_surface(wl_surface* surface, wl_display* display,
                       uint32_t *pixels)
    {
        vksystem.vulkan_wayland_surface_ptr = surface;
        vksystem.vulkan_wayland_display_ptr = display;
        vksystem.state.surface_pixel = pixels;
      
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

     int32_t find_suitable_memory_properties(VkSystem* vksystem,
                                         VkMemoryPropertyFlags optimal_memory_type, 
                                         uint32_t memory_type_bit_requirements);
    
    void allocate_required(VkSystem *vksystem);
    void free_required(VkSystem *vksystem);
 
    void start_memory_recording(VkSystem *vksystem, 
                                enum memory_type usage);

    // memory is allocated here;
    void end_memory_recording(VkSystem *vksystem);

    void begin_shader_recording(VkSystem *vksystem,enum shader_type type);
    bool load_shader(VkSystem *vksystem,const char *vtx_shader_code, const char *frag_shader_code);
    bool create_shader_module(VkSystem *vksystem);
    vk_shader* end_shader_recording(VkSystem *vksystem);

    void free_memory_resource(VkSystem *vksystem); 
    VkBuffer create_buffer(VkSystem* vksystem , VkBufferUsageFlags usage_mode,
                           VkBufferCreateFlags flags,VkDeviceSize size);

    VkImage create_image(VkSystem* vksytem,uint32_t height,uint32_t width);
    VkImageView create_image_view(VkSystem *vksystem,VkImage which_image);

    void begin_pipeline_recording(VkSystem *vksystem , enum pipeline_type type);
    bool create_graphics_pipeline(VkSystem *vksystem , vk_shader *which_shader);
    vk_pipeline* end_pipeline_recording(VkSystem *vksystem);
  
    bool create_shader(VkSystem *vksystem); 
    bool create_instance(VkSystem* vksystem);
    bool select_physical_device(VkSystem* vksystem);
    bool get_queue(VkSystem* vksystem);
    bool create_virtual_device(VkSystem* vksystem);
    bool create_cmd_pool(VkSystem* vksystem);
    bool create_cmd_buffer(VkSystem* vksystem);
    bool create_rendersync_resource(VkSystem *vksystem); 
    bool create_render_buffer(VkSystem *vksystem);  
    bool create_pipeline(VkSystem *vksystem);
    void destroy_rendersync_resource(VkSystem* vksystem);

    void destroy_render_buffer(VkSystem* vksystem);
    void exit_vulkan(enum exitlevel level, VkSystem* vksystem);

   
    void transition_layout(VkSystem *vksystem);
    void toggle_state(VkSystem *vksystem);
    void render_frame(VkSystem* vksystem);
    void lets_see(VkSystem* vksystem);


    void start_cmd_buffer(VkSystem *vksystem);
    void end_cmd_buffer(VkSystem *vksystem);
};
