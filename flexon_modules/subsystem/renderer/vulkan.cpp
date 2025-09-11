#include "./shaders/shader_code.hpp"
#include "./vulkan.hpp"
#include <shaderc/shaderc.hpp>
#include <wayland-client.h>

bool vksystem_down = false;

const uint32_t indexed_data[] = {0, 1, 2, 2, 3, 0};
const uint32_t indexed_data1[] = {4, 5, 6, 6, 7, 4};

static VkDeviceSize align_offset(VkDeviceSize alignment, VkDeviceSize value)
{
    if (alignment == 0)
        return value; // avoid division by zero
    return (value + alignment - 1) & ~(alignment - 1);
}

void flexon_vulkan_renderer::allocate_required(VkSystem* vksystem)
{
   
    vksystem->render_pool.frame_buffer = new render_frame_buffer[vksystem->render_pool.count];
    vksystem->render_pool.image_cmd_buffer = new VkCommandBuffer[vksystem->render_pool.count];
};

void flexon_vulkan_renderer::begin_shader_recording(VkSystem* vksystem, enum shader_type type)
{

    if (vksystem->shader_record_state == nullptr) {
        vksystem->shader_record_state = new vk_shader;
    } else {
        vksystem->shader_record_state->next = new vk_shader;
        vksystem->shader_record_state = vksystem->shader_record_state->next;
    }
    vksystem->shader_record_state->shader_type = type;

    return;
};

void flexon_vulkan_renderer::begin_pipeline_recording(VkSystem* vksystem, pipeline_info *info)
{

   static struct {
    VkBuffer *vtx_buf;
    VkBuffer *frg_buf;
    VkBuffer *idx_buf;
    void *vtx_data;
    void *frg_data;
    void *idx_data;
  }aux;

    
    if (vksystem->pipeline_record_state == nullptr) {
        vksystem->pipeline_record_state = new vk_pipeline;
    } else {
        vksystem->pipeline_record_state->next = new vk_pipeline;
        vksystem->pipeline_record_state = vksystem->pipeline_record_state->next;
    }
   
    vksystem->pipeline_record_state->type = info->ptype;
    begin_shader_recording(vksystem, info->stype);

    if (!load_shader(vksystem, info->vtx_code, info->frag_code)) {
        //   return false;
    }
    if (!create_shader_module(vksystem)) {
        // return false;
    }

    vksystem->pipeline_record_state->shader = end_shader_recording(vksystem);

    aux.vtx_buf = &vksystem->pipeline_record_state->shader->input.vertex_buffer;
    aux.frg_buf = &vksystem->pipeline_record_state->shader->input.uniform_buffer;
    aux.idx_buf = &vksystem->pipeline_record_state->shader->input.index_buffer;
    

    memcpy(&vksystem->shader_record_state->input, &info->input, sizeof(shader_input));    

    start_memory_recording(vksystem,VK_MEMORY_TYPE_SHADER_MAPPED);

    buffer_create_info buffer_info_vertex = {
     .layout_type = MEM_PART_VERTEX_BUF,
     .usage_mode = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
     .flags = 0,
     .size = vksystem->pipeline_record_state->shader->input.vertex_size
    };

    *aux.vtx_buf = create_buffer(vksystem,&buffer_info_vertex);

    buffer_create_info buffer_info_indexed = {
     .layout_type = MEM_PART_INDEXED_BUF,
     .usage_mode = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
     .flags = 0,
     .size = vksystem->pipeline_record_state->shader->input.vertex_size
    };

    *aux.idx_buf = create_buffer(vksystem,&buffer_info_indexed);


    buffer_create_info buffer_info_uniform = {
     .layout_type = MEM_PART_UNIFORM_BUF,
     .usage_mode = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
     .flags = 0,
     .size = vksystem->pipeline_record_state->shader->input.uniform_size
    };
    create_buffer(vksystem,&buffer_info_uniform);
    end_memory_recording(vksystem);
    
    char *mapped_data;
    vkMapMemory(
                vksystem->virtual_device,
                vksystem->mem_record_state->memory,
                0,
                VK_WHOLE_SIZE,
                0,
                (void**)&mapped_data
              );

   for(auto *itr = vksystem->mem_record_state->memory_layout ; itr != nullptr ; itr = itr->next){
        switch(itr->part_type){
          case MEM_PART_VERTEX_BUF:
          aux.vtx_data = mapped_data + itr->memory_offset;
          break;
          case MEM_PART_UNIFORM_BUF:
          aux.frg_data = mapped_data + itr->memory_offset;
          break;
          case MEM_PART_INDEXED_BUF:
          aux.idx_data = mapped_data + itr->memory_offset;
         break;
       }
    }

    vksystem->pipeline_record_state->shader->input.vertex_data = aux.vtx_data;
    vksystem->pipeline_record_state->shader->input.uniform_data = aux.frg_data;
    vksystem->pipeline_record_state->shader->input.index_data = aux.idx_data;


};

vk_pipeline* flexon_vulkan_renderer::end_pipeline_recording(VkSystem* vksystem)
{
    return vksystem->pipeline_record_state;
};

bool flexon_vulkan_renderer::create_shader_module(VkSystem* vksystem)
{

    create_struct(vert_module_create_info, VkShaderModuleCreateInfo,
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = NULL,
        .codeSize = vksystem->shader_record_state->code_size_vert,
        .pCode = vksystem->shader_record_state->compiled_code_vert);

    create_struct(frag_module_create_info, VkShaderModuleCreateInfo,
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = NULL,
        .codeSize = vksystem->shader_record_state->code_size_frag,
        .pCode = vksystem->shader_record_state->compiled_code_frag);

    vkCreateShaderModule(vksystem->virtual_device, &vert_module_create_info,
        NULL, &vksystem->shader_record_state->shader_vert);

    vkCreateShaderModule(vksystem->virtual_device, &frag_module_create_info,
        NULL, &vksystem->shader_record_state->shader_frag);
    std::cout << vksystem->shader_record_state->code_size_frag << std::endl;

    return true;
}

vk_shader* flexon_vulkan_renderer::end_shader_recording(VkSystem* vksystem)
{

    delete vksystem->shader_record_state->compiled_code_vert;
    delete vksystem->shader_record_state->compiled_code_frag;

    return vksystem->shader_record_state;
};


void flexon_vulkan_renderer::start_memory_recording(VkSystem* vksystem, enum memory_type usage)
{
    if (vksystem->mem_record_state == nullptr) {
        vksystem->mem_record_state = new vk_memory;
    } else {
        vksystem->mem_record_state->next = new vk_memory;
        vksystem->mem_record_state = vksystem->mem_record_state->next;
    }
    vksystem->mem_record_state->memory_usage = usage;
};

void flexon_vulkan_renderer::end_memory_recording(VkSystem* vksystem)
{

   static struct{
    VkDeviceMemory *mem;
   }info;
   info.mem = &vksystem->mem_record_state->memory;

    int32_t mem_type_index = find_suitable_memory_properties(vksystem,
        vksystem->physical_device.required_memory_type,
        vksystem->mem_record_state->memory_type_bit);
    
    if (mem_type_index < 0)
        std::cout << "Memory index not found failed" << std::endl;

    create_struct(mem_layout_size_info, VkMemoryAllocateInfo,
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = NULL,
        .allocationSize = vksystem->mem_record_state->memory_size,
        .memoryTypeIndex = (uint32_t)mem_type_index);

    vkAllocateMemory(vksystem->virtual_device, &mem_layout_size_info,
        VK_NULL_HANDLE, info.mem);

    for (auto *itr = vksystem->mem_record_state->memory_layout; itr != nullptr; itr = itr->next) {
        switch (itr->part_type) {
         case MEM_PART_UNIFORM_BUF:
         case MEM_PART_VERTEX_BUF:
         case MEM_PART_INDEXED_BUF:
         case MEM_PART_NOM_BUF:
            std::cout << "[INFO] MEM buffer binding request found" << std::endl;
            vkBindBufferMemory(vksystem->virtual_device,
                (VkBuffer)itr->bind_member,
                *info.mem,
                itr->memory_offset);
            break;
        case MEM_PART_IMAGE_BUF:
            std::cout << "[INFO] MEM image binding request found" << std::endl;
            vkBindImageMemory(vksystem->virtual_device,
                (VkImage)itr->bind_member,
                 *info.mem,
                itr->memory_offset);

         break;
        }
    }

    return;
};

void flexon_vulkan_renderer::initlise()
{
    allocate_required(&vksystem);

#ifndef NDEBUG
    if (!checkValidationLayerSupport(validationLayer, &enableLayerCount)) {
        std::cout << "Validation Layer not supported aborting" << std::endl;
        std::abort();
    }
#endif

    if (vkEnumerateInstanceVersion(&vksystem.api_version) != VK_SUCCESS) {
        std::cout << "Error probing api verision" << std::endl;
        exit_vulkan(EXIT_LEVEL_NONE, &vksystem);
        return;
    }

    if (!create_instance(&vksystem)) {
        std::cout << "instance Creation Failed" << std::endl;
        exit_vulkan(EXIT_LEVEL_NONE, &vksystem);
        return;
    }

    if (!select_physical_device(&vksystem)) {
        std::cout << "physicalDevice selection failed: " << std::endl;
        exit_vulkan(EXIT_LEVEL_INSTANCE, &vksystem);
        return;
    }

    if (!create_virtual_device(&vksystem)) {
        std::cout << "[LOGICAL DEVICE] Creation of logical device failed.\n";
        exit_vulkan(EXIT_LEVEL_INSTANCE, &vksystem);
        return;
    }

    if (!get_queue(&vksystem)) {
        std::cout << "[QUEUE] Failed to get the queue. \n";
        exit_vulkan(EXIT_LEVEL_QUEUE, &vksystem);
        return;
    }

    if (vksystem.render_pool.count == 0) {
        std::cout << "[INVALID COUNT] Invalid buffer creation count. \n";
        exit_vulkan(EXIT_LEVEL_QUEUE, &vksystem);
        return;
    }
    if (!create_rendersync_resource(&vksystem)) {
        std::cout << "[RENDER RESOURCE] Creation of render resources failed\n";
        exit_vulkan(EXIT_LEVEL_QUEUE, &vksystem);
        return;
    }

    if (!create_cmd_pool(&vksystem)) {
        std::cout << "[COMMAND POOL] Creation of Commandpool failed.\n";
        exit_vulkan(EXIT_LEVEL_QUEUE, &vksystem);
        return;
    }

    if (!create_cmd_buffer(&vksystem)) {
        std::cout << "[COMMAND POOL] Creation of Commandpool failed.\n";
        exit_vulkan(EXIT_LEVEL_CMD_POOL, &vksystem);
        return;
    }

    if (!create_render_buffer(&vksystem)) {
        std::cout << "[RENDER BUFFER] Creation of render buffer failed\n";
        exit_vulkan(EXIT_LEVEL_CMD_BUFFER, &vksystem);
        return;
    }

    if (!create_pipeline(&vksystem)) {
        std::cout << "[RENDER BUFFER] Creation of render buffer failed\n";
        exit_vulkan(EXIT_LEVEL_CMD_BUFFER, &vksystem);
        return;
    }

    return;
}

bool flexon_vulkan_renderer::load_shader(VkSystem* vksystem, const char* vtx_shader_code, const char* frag_shader_code)
{

    // unoptimised code
    // TODO: Optimise this code and remove depedency on class based abstracted function
    // just for normal compilation

    size_t vertex_code_length = strlen(vtx_shader_code);
    size_t fragment_code_length = strlen(frag_shader_code);

    shaderc::CompileOptions shader_compiler_options;
    shaderc::Compiler shader_compiler;
    shaderc::SpvCompilationResult vertex_shader_result = shader_compiler.CompileGlslToSpv(vtx_shader_code,
        vertex_code_length,
        shaderc_glsl_vertex_shader,
        "shader.vert",
        shader_compiler_options);
    int shader_status_vert = vertex_shader_result.GetCompilationStatus();
    if (shader_status_vert != shaderc_compilation_status_success) {
        std::cout << "[COMPILATION] Compilation of Vertex shader failed.\n";
    }

    size_t vert_size = 0; // size w.r.t uint32_t
    for (auto& iterator : vertex_shader_result) {
        vert_size += 1;
    }

    uint32_t* itr_vert = new uint32_t[vert_size];

    vksystem->shader_record_state->code_size_vert = vert_size * sizeof(uint32_t); // convert size to bytes;
    vksystem->shader_record_state->compiled_code_vert = itr_vert;

    size_t index = 0;
    for (auto& iterator : vertex_shader_result) {
        itr_vert[index] = iterator;
        index += 1;
    }

    shaderc::SpvCompilationResult frag_shader_result = shader_compiler.CompileGlslToSpv(frag_shader_code,
        fragment_code_length,
        shaderc_glsl_fragment_shader,
        "shader.frag",
        shader_compiler_options);
    int shader_status_frag = frag_shader_result.GetCompilationStatus();
    if (shader_status_frag != shaderc_compilation_status_success) {
        std::cout << "[COMPILATION] Compilation of Vertex shader failed.\n";
    }

    size_t frag_size = 0; // size w.r.t uint32_t
    for (auto& iterator : frag_shader_result) {
        frag_size += 1;
    }

    uint32_t* itr_frag = new uint32_t[frag_size];

    vksystem->shader_record_state->code_size_frag = frag_size * sizeof(uint32_t); // convert size to bytes;
    vksystem->shader_record_state->compiled_code_frag = itr_frag;

    size_t idx = 0;
    for (auto& iterator : frag_shader_result) {
        itr_frag[idx] = iterator;
        idx += 1;
    }

    return true;
}

bool flexon_vulkan_renderer::create_instance(VkSystem* vksystem)
{
    // TODO: incoporate physical device feature 2;

    create_struct(app_info, VkApplicationInfo,
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "hello",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "hello",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = vksystem->api_version, );

    create_struct(app_instance, VkInstanceCreateInfo,
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = (uint32_t)0,
        .ppEnabledLayerNames = NULL,
        .enabledExtensionCount = (uint32_t)0,
        .ppEnabledExtensionNames = NULL, );
#ifndef NDEBUG
    app_instance.enabledLayerCount = static_cast<uint32_t>(enableLayerCount);
    app_instance.ppEnabledLayerNames = validationLayer;
#endif

    if (vkCreateInstance(&app_instance, NULL, &vksystem->instance) != VK_SUCCESS) {
        std::cout << "instance creation failed" << std::endl;
        return false;
    }
    return true;
}

bool flexon_vulkan_renderer::select_physical_device(VkSystem* vksystem)
{

    uint32_t device_count { 0 };

    vkEnumeratePhysicalDevices(vksystem->instance, &device_count, nullptr);
    if (device_count == 0) {
        std::cout << "[DEVICE ERROR] Failed to find a GPU with the vulkan support. \n";
        return false;
    }

    std::vector<VkPhysicalDevice> available_devices(device_count);
    vkEnumeratePhysicalDevices(vksystem->instance, &device_count, available_devices.data());

    if (!filter_physical_device(vksystem, available_devices)) {
        std::cout << "[DEVICE ERROR] Failed to find a GPU with suitable requirements \n";
        return false;
    }
    std::cout << "[DEVICE SUCCESS] selected a physical Device\n";

    return true;
};

bool flexon_vulkan_renderer::create_virtual_device(VkSystem* vksystem)
{

    create_struct(queue_family_struct, VkDeviceQueueCreateInfo,
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = NULL,
        .queueCount = vksystem->physical_device.queue_count,
        .pQueuePriorities = &vksystem->physical_device.queue_priority);

    queue_family_struct.queueFamilyIndex = vksystem->physical_device.queue_family_index;

    create_struct(dynamic_rendering_feature, VkPhysicalDeviceDynamicRenderingFeaturesKHR,
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
        .dynamicRendering = VK_TRUE, );

    create_struct(device_features, VkPhysicalDeviceFeatures);

    create_struct(device_create_info, VkDeviceCreateInfo,
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &dynamic_rendering_feature,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queue_family_struct,
        .pEnabledFeatures = &device_features);

    device_create_info.enabledExtensionCount = (uint32_t)vksystem->device_extension_count;
    device_create_info.ppEnabledExtensionNames = vksystem->extension_name;

    if (vkCreateDevice(vksystem->physical_device.device, &device_create_info,
            nullptr, &vksystem->virtual_device)
        != VK_SUCCESS) {
        std::cout << "[DEVICE] Creation of logical Device failed. \n";
    }

    std::cout << "[DEVICE] Creation of logical Device success. \n";
    return true;
};

bool flexon_vulkan_renderer::get_queue(VkSystem* vksystem)
{

    vksystem->physical_device.Queue = new VkQueue[vksystem->physical_device.queue_count];

    vkGetDeviceQueue(vksystem->virtual_device, vksystem->physical_device.queue_family_index,
        0, vksystem->physical_device.Queue);

    if (vksystem->physical_device.Queue == nullptr)
        return false;

    std::cout << "[QUEUE] Retrived queue successfull. \n";
    return true;
}

bool flexon_vulkan_renderer::create_cmd_pool(VkSystem* vksystem)
{

    create_struct(cmd_pool_info, VkCommandPoolCreateInfo,
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = NULL,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = vksystem->physical_device.queue_family_index);

    if (vkCreateCommandPool(vksystem->virtual_device, &cmd_pool_info,
            nullptr, &vksystem->cmd_pool)
        != VK_SUCCESS)
        return false;

    std::cout << "[COMMANDPOOL] Creation of command pool OK \n";
    return true;
}

bool flexon_vulkan_renderer::create_cmd_buffer(VkSystem* vksystem)
{

    create_struct(cmd_buffer_allocation_info, VkCommandBufferAllocateInfo,
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = NULL,
        .commandPool = vksystem->cmd_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = vksystem->render_pool.count);

    if (vkAllocateCommandBuffers(vksystem->virtual_device, &cmd_buffer_allocation_info,
            vksystem->render_pool.image_cmd_buffer)
        != VK_SUCCESS)
        return false;

    return true;
}

bool flexon_vulkan_renderer::create_rendersync_resource(VkSystem* vksystem)
{
    create_struct(fenceinfo, VkFenceCreateInfo,
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = NULL,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT);

    create_struct(semaphoreInfo, VkSemaphoreCreateInfo,
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = NULL);

    for (int i = 0; i < vksystem->render_pool.count; i++) {

        if (vkCreateFence(vksystem->virtual_device, &fenceinfo,
                NULL, &vksystem->render_pool.frame_buffer[i].sync.image_fence)
            != VK_SUCCESS) {
            destroy_rendersync_resource(vksystem);
            std::cout << "[FENCE ALLOCATION] Allocation of fence failed \n";
            return false;
        }
        if (vkCreateSemaphore(vksystem->virtual_device, &semaphoreInfo,
                NULL, &vksystem->render_pool.frame_buffer[i].sync.image_start_semaphore)
            != VK_SUCCESS) {

            destroy_rendersync_resource(vksystem);
            std::cout << "[SEMAPHORE END] Allocation of semaphore failed \n";
            return false;
        }

        if (vkCreateSemaphore(vksystem->virtual_device, &semaphoreInfo,
                NULL, &vksystem->render_pool.frame_buffer[i].sync.image_end_semaphore)
            != VK_SUCCESS) {

            destroy_rendersync_resource(vksystem);
            std::cout << "[SEMAPHORE END] Allocation of semaphore failed \n";
            return false;
        }
    }

    return true;
};

bool flexon_vulkan_renderer::create_render_buffer(VkSystem* vksystem)
{

    start_memory_recording(vksystem, VK_MEMORY_TYPE_PRIMARY_MAPPED);

    buffer_create_info buffer_info = {
     .layout_type = MEM_PART_NOM_BUF,
     .usage_mode =  VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
     .flags = 0,
     .size = (vksystem->surface_width * vksystem->surface_height) * sizeof(uint32_t)
    };

    vksystem->render_pool.staging_buffer = create_buffer(vksystem,&buffer_info);

    for (int i = 0; i < vksystem->render_pool.count; i++) {
        VkImage tmp_img = create_image(vksystem, vksystem->surface_height, vksystem->surface_width);
        vksystem->render_pool.frame_buffer[i].image = tmp_img;
    };
  
   
    end_memory_recording(vksystem);

    vkMapMemory(vksystem->virtual_device,
    vksystem->mem_record_state->memory,
    0,
    buffer_info.size,
    0,
    (void **)&vksystem->state.raw_pixel);

    for (int i = 0; i < vksystem->render_pool.count; i++) {
        VkImageView tmp_view = create_image_view(vksystem, vksystem->render_pool.frame_buffer[i].image);
        vksystem->render_pool.frame_buffer[i].image_view = tmp_view;
    };

    vksystem->render_pool.memory = vksystem->mem_record_state;

    return true;
};


void flexon_vulkan_renderer::destroy_rendersync_resource(VkSystem* vksystem)
{

    if (vksystem->render_pool.frame_buffer == nullptr)
        return;

    for (int i = 0; i < vksystem->render_pool.count; i++)

        vkDestroyFence(vksystem->virtual_device,
            vksystem->render_pool.frame_buffer[i].sync.image_fence, NULL);

    for (int i = 0; i < vksystem->render_pool.count; i++)
        vkDestroySemaphore(vksystem->virtual_device,
            vksystem->render_pool.frame_buffer[i].sync.image_start_semaphore, NULL);

    for (int i = 0; i < vksystem->render_pool.count; i++)
        vkDestroySemaphore(vksystem->virtual_device,
            vksystem->render_pool.frame_buffer[i].sync.image_end_semaphore, NULL);

    return;
}

void flexon_vulkan_renderer::destroy_renderer()
{
    exit_vulkan(EXIT_LEVEL_ALL, &vksystem);
};


void flexon_vulkan_renderer::render()
{
    render_frame(&vksystem);
    return;
};

void flexon_vulkan_renderer::render_frame(VkSystem* vksystem)
{
    toggle_state(vksystem);
    start_cmd_buffer(vksystem);
    // TODO: Render Real content;

    end_cmd_buffer(vksystem);

    if (vkWaitForFences(vksystem->virtual_device, 1, &vksystem->state.fb.sync.image_fence, 1, UINT64_MAX) == VK_SUCCESS) {
    }

  /*
    for (int i = 0; i < (500 * 500); i++) {
        vksystem->state.surface_pixel[i] = vksystem->state.raw_pixel[i];
    }
    wl_surface_damage(vksystem->vulkan_wayland_surface_ptr, 0, 0, 500, 500);
    wl_surface_commit(vksystem->vulkan_wayland_surface_ptr);
  */
};

void flexon_vulkan_renderer::toggle_state(VkSystem* vksystem)
{
    int current_index = vksystem->state.current_index + 1;

    bool smaller = (current_index < vksystem->render_pool.count);

    current_index = current_index * smaller;

    vksystem->state.current_index = current_index;
    memcpy(&vksystem->state.fb,
        &vksystem->render_pool.frame_buffer[current_index], sizeof(render_frame_buffer));
    vksystem->state.current_cmd_buffer = vksystem->render_pool.image_cmd_buffer[current_index];
    vksystem->state.cpy_buffer = vksystem->render_pool.staging_buffer;
    return;
};

void flexon_vulkan_renderer::transition_layout_start(VkSystem* vksystem)
{
  static struct {
    uint32_t queueidx;
   }info;

   info.queueidx = vksystem->physical_device.queue_family_index; 


    create_struct(barrier_info, VkImageMemoryBarrier,
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .srcQueueFamilyIndex = info.queueidx,
        .dstQueueFamilyIndex = info.queueidx,
        .image = vksystem->state.fb.image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = (uint32_t)0,
            .levelCount = (uint32_t)1,
            .baseArrayLayer = (uint32_t)0,
            .layerCount = (uint32_t)1 }
            );

    barrier_info.srcAccessMask = 0;
    barrier_info.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    vkCmdPipelineBarrier(
        vksystem->state.current_cmd_buffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier_info);

    return;
};

void flexon_vulkan_renderer::transition_layout_end(VkSystem* vksystem)
{
  static struct {
    uint32_t queueidx;
   }info;

   info.queueidx = vksystem->physical_device.queue_family_index; 

    create_struct(barrier_info, VkImageMemoryBarrier,
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        .srcQueueFamilyIndex = info.queueidx,
        .dstQueueFamilyIndex = info.queueidx,
        .image = vksystem->state.fb.image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = (uint32_t)0,
            .levelCount = (uint32_t)1,
            .baseArrayLayer = (uint32_t)0,
            .layerCount = (uint32_t)1 }
            );

    barrier_info.srcAccessMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    barrier_info.dstAccessMask = VK_PIPELINE_STAGE_TRANSFER_BIT;

    vkCmdPipelineBarrier(
        vksystem->state.current_cmd_buffer,
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier_info);

    return;
};

void flexon_vulkan_renderer::start_cmd_buffer(VkSystem* vksystem)
{
  vksystem->state.shader = vksystem->render_pool.pipeline->shader;

  static struct {
    VkCommandBuffer cmd;
    VkImageView view;
    VkPipeline pipeline;
  }info;

  info.cmd = vksystem->state.current_cmd_buffer;
  info.view = vksystem->state.fb.image_view;
  info.pipeline = vksystem->render_pool.pipeline->pipeline;

    create_struct(cmd_buffer_info, VkCommandBufferBeginInfo,
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = NULL,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    vkBeginCommandBuffer(info.cmd, &cmd_buffer_info);
    transition_layout_start(vksystem);

    create_struct(color_attachment_info, VkRenderingAttachmentInfo,
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .pNext = NULL,
        .imageView = info.view,
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .resolveMode = VK_RESOLVE_MODE_NONE,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue = { { 0.0f, 1.0f, 0.0f, 1.0f } });

    create_struct(rendering_info, VkRenderingInfoKHR,
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR,
        .pNext = NULL,
        .renderArea = {
            .offset = {
                .x = 0,
                .y = 0 },
            .extent = { .width = vksystem->surface_width, .height = vksystem->surface_height } },
        .layerCount = (uint32_t)1,
        .viewMask = (uint32_t)0,
        .colorAttachmentCount = (uint32_t)1, 
        .pColorAttachments = &color_attachment_info
                  );

    vkCmdBeginRendering(info.cmd, &rendering_info);
    vkCmdBindPipeline(info.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipeline);
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(info.cmd, 0, 1,&vksystem->state.shader->input.vertex_buffer, offsets);
    vkCmdBindIndexBuffer(info.cmd,vksystem->state.shader->input.index_buffer, 0,VK_INDEX_TYPE_UINT32);

    VkViewport viewport {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(vksystem->surface_width);
    viewport.height = static_cast<float>(vksystem->surface_height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(info.cmd, 0, 1, &viewport);

    VkRect2D scissor {};
    scissor.offset = { 0, 0 };
    scissor.extent = { 500, 500 };
    vkCmdSetScissor(info.cmd, 0, 1, &scissor);
    vkCmdDrawIndexed(info.cmd,12, 1, 0, 0, 0);
    return;

};

void flexon_vulkan_renderer::end_cmd_buffer(VkSystem* vksystem)
{
    vkCmdEndRendering(vksystem->state.current_cmd_buffer);

    transition_layout_end(vksystem);

    VkBufferImageCopy region {};
    region.bufferOffset = 0;
    region.bufferRowLength = 500; // Tightly packed
    region.bufferImageHeight = 500;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = { 500, 500, 1 };

    vkCmdCopyImageToBuffer(
        vksystem->state.current_cmd_buffer,
        vksystem->state.fb.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        vksystem->state.cpy_buffer,
        1, &region);

    vkEndCommandBuffer(vksystem->state.current_cmd_buffer);
    create_struct(submit_info, VkSubmitInfo,
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = NULL,
        .waitSemaphoreCount = (uint32_t)0,
        .pWaitSemaphores = nullptr,
        .pWaitDstStageMask = (uint32_t)0,
        .commandBufferCount = (uint32_t)1,
        .pCommandBuffers = &vksystem->state.current_cmd_buffer,
        .signalSemaphoreCount = (uint32_t)0,
        .pSignalSemaphores = nullptr);

    vkResetFences(vksystem->virtual_device, 1, &vksystem->state.fb.sync.image_fence);
    vkQueueSubmit(vksystem->physical_device.Queue[0], 1, &submit_info,
        vksystem->state.fb.sync.image_fence);

    return;
};

bool flexon_vulkan_renderer::create_pipeline(VkSystem* vksystem)
{

    pipeline_info info;

    info.stype = VK_SHADER_VIEW; 
    info.ptype = VK_PIPELINE_TYPE_PRIMARY;
    info.vtx_code = (char*)vertex_shader_code;
    info.frag_code = (char*)fragment_shader_code;

    info.input.vertex_stride = (uint32_t)sizeof(glm::vec2);
    info.input.uniform_stride = (uint32_t)sizeof(uint32_t);
    info.input.vertex_size = sizeof(glm::vec2) * 8;
    info.input.uniform_size = sizeof(glm::vec2);
    
    info.input.indexbuffer_size =  sizeof(indexed_data) * 2;
    info.input.indexbuffer_stride = sizeof(uint32_t);
 

    begin_pipeline_recording(vksystem, &info);

    if (!create_graphics_pipeline(vksystem, vksystem->shader_record_state)) {
        return false;
    }

    vksystem->render_pool.pipeline = end_pipeline_recording(vksystem);
    return true;
};

bool flexon_vulkan_renderer::create_graphics_pipeline(VkSystem* vksystem, vk_shader* which_shader)
{

    create_struct(pipeline_layout_info, VkPipelineLayoutCreateInfo,
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = NULL,
        .setLayoutCount = (uint32_t)0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = (uint32_t)0,
        .pPushConstantRanges = nullptr, );
    if (vkCreatePipelineLayout(vksystem->virtual_device, &pipeline_layout_info, nullptr, &vksystem->pipeline_record_state->layout) != VK_SUCCESS) {
        std::cout << "[PIPELINE] Layout Creation Failed\n";
        return false;
    }

    VkPipelineShaderStageCreateInfo shader_stage[2];
    shader_stage[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    shader_stage[0].pNext = NULL;
    shader_stage[0].flags = 0;
    shader_stage[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shader_stage[0].module = which_shader->shader_vert;
    shader_stage[0].pName = "main";
    shader_stage[0].pSpecializationInfo = NULL;

    shader_stage[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    shader_stage[1].pNext = NULL;
    shader_stage[1].flags = 0,
    shader_stage[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shader_stage[1].module = which_shader->shader_frag;
    shader_stage[1].pName = "main";
    shader_stage[1].pSpecializationInfo = NULL;
 
    create_struct(vertex_binding_info,VkVertexInputBindingDescription,
                  .binding = 0,
                  .stride = which_shader->input.vertex_stride,
                  .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
                  );


   create_struct(vertex_attribute_info , VkVertexInputAttributeDescription,
                 .location =(uint32_t)0,
                 .binding = (uint32_t)0,
                 .format = VK_FORMAT_R32G32_SFLOAT,
                 .offset = (uint32_t)0,
                 );
   create_struct(vertex_input_stage,VkPipelineVertexInputStateCreateInfo,
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = (uint32_t)0,
        .vertexBindingDescriptionCount = (uint32_t)1,
        .vertexAttributeDescriptionCount = (uint32_t)1,
        );

    vertex_input_stage.pVertexAttributeDescriptions = &vertex_attribute_info; 
    vertex_input_stage.pVertexBindingDescriptions = &vertex_binding_info;

    create_struct(input_assembly_state, VkPipelineInputAssemblyStateCreateInfo,
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = (uint32_t)0,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE);
    create_struct(viewport_state_info, VkPipelineViewportStateCreateInfo,
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = (uint32_t)0,
        .viewportCount = (uint32_t)1,
        .pViewports = NULL,
        .scissorCount = (uint32_t)1,
        .pScissors = NULL);

    create_struct(rasterization_state_info, VkPipelineRasterizationStateCreateInfo,
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = NULL,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 1.0f, );
    create_struct(multisample_state_info, VkPipelineMultisampleStateCreateInfo,
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = (uint32_t)0,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 1.0f,
        .pSampleMask = NULL,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE);
    create_struct(color_blend_attachment_info, VkPipelineColorBlendAttachmentState,
        .blendEnable = VK_TRUE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT
            | VK_COLOR_COMPONENT_G_BIT
            | VK_COLOR_COMPONENT_B_BIT
            | VK_COLOR_COMPONENT_A_BIT, );

    create_struct(color_blend_state_info, VkPipelineColorBlendStateCreateInfo,
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = NULL,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = (uint32_t)1,
        .pAttachments = &color_blend_attachment_info,
        .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f });
    VkDynamicState pipeline_dynamic_state_name[2] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    create_struct(pipeline_dynamic_state_info, VkPipelineDynamicStateCreateInfo,
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .dynamicStateCount = (uint32_t)2,
        .pDynamicStates = pipeline_dynamic_state_name, );
    create_struct(pipeline_rendering_info, VkPipelineRenderingCreateInfoKHR,
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR,
        .pNext = NULL,
        .colorAttachmentCount = (uint32_t)1,
        .pColorAttachmentFormats = &vksystem->render_pool.image_format,
        .depthAttachmentFormat = VK_FORMAT_UNDEFINED,
        .stencilAttachmentFormat = VK_FORMAT_UNDEFINED, );

    create_struct(graphics_pipeline_create_info, VkGraphicsPipelineCreateInfo,
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = &pipeline_rendering_info,
        .stageCount = (uint32_t)2,
        .pStages = shader_stage,
        .pVertexInputState = &vertex_input_stage,
        .pInputAssemblyState = &input_assembly_state,
        .pTessellationState = NULL,
        .pViewportState = &viewport_state_info,
        .pRasterizationState = &rasterization_state_info,
        .pMultisampleState = &multisample_state_info,
        .pDepthStencilState = nullptr,
        .pColorBlendState = &color_blend_state_info,
        .pDynamicState = &pipeline_dynamic_state_info,
        .layout = vksystem->pipeline_record_state->layout,
        .renderPass = VK_NULL_HANDLE, );

    vkCreateGraphicsPipelines(vksystem->virtual_device, VK_NULL_HANDLE, (uint32_t)1,
        &graphics_pipeline_create_info, VK_NULL_HANDLE, &vksystem->pipeline_record_state->pipeline);
    return true;
};

VkBuffer flexon_vulkan_renderer::create_buffer(VkSystem* vksystem, buffer_create_info *info)
{

    VkBuffer tmp_buffer;
    vk_mem_layout* playout = new vk_mem_layout;
    playout->part_type = info->layout_type;

    if (vksystem->mem_record_state->memory_layout == nullptr) {
        vksystem->mem_record_state->memory_layout = playout;
        vksystem->mem_record_state->current = playout;

    } else {
        vksystem->mem_record_state->current->next = playout;
        vksystem->mem_record_state->current = playout;
    }

    create_struct(buffer_create_info, VkBufferCreateInfo,
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = NULL,
        .flags = info->flags,
        .size = info->size,
        .usage = info->usage_mode,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = vksystem->physical_device.queue_count,
        .pQueueFamilyIndices = &vksystem->physical_device.queue_family_index, );

    if (vkCreateBuffer(vksystem->virtual_device, &buffer_create_info, NULL, &tmp_buffer) != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    };

    create_struct(memory_requirement, VkMemoryRequirements);
    vkGetBufferMemoryRequirements(vksystem->virtual_device, tmp_buffer, &memory_requirement);
    VkDeviceSize aligned_offset = align_offset(memory_requirement.alignment, vksystem->mem_record_state->memory_size);

    playout->part_size = memory_requirement.size + (aligned_offset - vksystem->mem_record_state->memory_size);
    vksystem->mem_record_state->memory_size += aligned_offset;
    playout->memory_offset = aligned_offset;
    vksystem->mem_record_state->memory_size += memory_requirement.size;
    vksystem->mem_record_state->memory_type_bit |= memory_requirement.memoryTypeBits;
    playout->bind_member = tmp_buffer;

    return tmp_buffer;
};

VkImage flexon_vulkan_renderer::create_image(VkSystem* vksystem, uint32_t height, uint32_t width)
{
    VkImage tmp_image = VK_NULL_HANDLE;

    vk_mem_layout* playout = new vk_mem_layout;
    playout->part_type = MEM_PART_IMAGE_BUF;

    if (vksystem->mem_record_state->memory_layout == nullptr) {
        vksystem->mem_record_state->memory_layout = playout;
        vksystem->mem_record_state->current = playout;

    } else {
      
        vksystem->mem_record_state->current->next = playout;
        vksystem->mem_record_state->current = playout;
    }

    create_struct(vk_image_info, VkImageCreateInfo,
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_SRGB,
        .extent = { .width = width, .height = height, .depth = (uint32_t)1 },
        .mipLevels = (uint32_t)1,
        .arrayLayers = (uint32_t)1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
            | VK_IMAGE_USAGE_TRANSFER_SRC_BIT
            | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED);

    if (vkCreateImage(vksystem->virtual_device, &vk_image_info,
            NULL, &tmp_image)
        != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }

    create_struct(memory_requirement, VkMemoryRequirements);
    vkGetImageMemoryRequirements(vksystem->virtual_device, tmp_image, &memory_requirement);
    VkDeviceSize aligned_offset = align_offset(memory_requirement.alignment, vksystem->mem_record_state->memory_size);

    playout->part_size = memory_requirement.size + (aligned_offset - vksystem->mem_record_state->memory_size);
    vksystem->mem_record_state->memory_size += aligned_offset;
    playout->memory_offset = aligned_offset;
    vksystem->mem_record_state->memory_size += memory_requirement.size;

    vksystem->mem_record_state->memory_type_bit |= memory_requirement.memoryTypeBits;
    playout->bind_member = tmp_image;

    return tmp_image;
};

VkImageView flexon_vulkan_renderer::create_image_view(VkSystem* vksystem, VkImage which_image)
{

    VkImageView tmp_image_view = VK_NULL_HANDLE;

    create_struct(image_view_create_info, VkImageViewCreateInfo,
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = NULL,
        .flags = (uint32_t)0,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_SRGB,
        .components = { .r = VK_COMPONENT_SWIZZLE_R,
            .g = VK_COMPONENT_SWIZZLE_G,
            .b = VK_COMPONENT_SWIZZLE_B,
            .a = VK_COMPONENT_SWIZZLE_A },
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = (uint32_t)0,
            .levelCount = (uint32_t)1,
            .baseArrayLayer = (uint32_t)0,
            .layerCount = (uint32_t)1 });

    image_view_create_info.image = which_image;

    if (vkCreateImageView(vksystem->virtual_device, &image_view_create_info,
            NULL, &tmp_image_view)
        != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }

    return tmp_image_view;
};


void flexon_vulkan_renderer::destroy_image_view(VkSystem* vksystem){
  render_frame_buffer *view = vksystem->render_pool.frame_buffer;
  for(int i = 0 ; i < vksystem->render_pool.count ; i++){
   vkDestroyImageView(vksystem->virtual_device,view[i].image_view,NULL);
  };
};


void flexon_vulkan_renderer::free_memory_resource(VkSystem* vksystem)
{
    for (auto *itr_mem = vksystem->render_pool.memory; itr_mem != nullptr; itr_mem = itr_mem->next) {
        for (auto *itr_lay = itr_mem->memory_layout; itr_lay != nullptr; itr_lay = itr_lay->next) {
            switch (itr_lay->part_type) {
            case MEM_PART_UNIFORM_BUF:
            case MEM_PART_VERTEX_BUF:
            case MEM_PART_INDEXED_BUF:
      
            case MEM_PART_NOM_BUF:
                vkDestroyBuffer(vksystem->virtual_device, (VkBuffer)itr_lay->bind_member, NULL);
                break;
            case MEM_PART_IMAGE_BUF:
                vkDestroyImage(vksystem->virtual_device, (VkImage)itr_lay->bind_member, NULL);
                break;
            };
        }
        if(itr_mem->memory_usage == VK_MEMORY_TYPE_PRIMARY_MAPPED){
        vkUnmapMemory(vksystem->virtual_device,itr_mem->memory);
        }
        vkFreeMemory(vksystem->virtual_device, itr_mem->memory, NULL);
    }

};

void flexon_vulkan_renderer::free_pipeline_resource(VkSystem *vksystem){

  for(auto *itr_pipe = vksystem->render_pool.pipeline ; itr_pipe != nullptr ; itr_pipe = itr_pipe->next){
    for(auto *itr_shade = itr_pipe->shader ; itr_shade != nullptr ; itr_shade = itr_shade->next){
      vkDestroyShaderModule(vksystem->virtual_device,itr_shade->shader_vert,NULL); 
      vkDestroyShaderModule(vksystem->virtual_device,itr_shade->shader_frag,NULL); 
    }
      vkDestroyPipelineLayout(vksystem->virtual_device,itr_pipe->layout,NULL);
      vkDestroyPipeline(vksystem->virtual_device,itr_pipe->pipeline,NULL);
  }
};


void flexon_vulkan_renderer::exit_vulkan(enum exitlevel level, VkSystem* vksystem)
{

    if (vksystem_down == false) {
        switch (level) {
        case EXIT_LEVEL_ALL:
        case EXIT_LEVEL_GRAPHICS_PIPELINE:
             free_pipeline_resource(vksystem);
        case EXIT_LEVEL_RENDER_BUFFER:
            destroy_image_view(vksystem);
            free_memory_resource(vksystem);
        case EXIT_LEVEL_RENDER_PASS:
        case EXIT_LEVEL_CMD_BUFFER:
        case EXIT_LEVEL_CMD_POOL:
            vkDestroyCommandPool(vksystem->virtual_device,
                vksystem->cmd_pool, nullptr);
       case EXIT_LEVEL_RESOURCE:
            destroy_rendersync_resource(vksystem);
        case EXIT_LEVEL_QUEUE:
            delete vksystem->physical_device.Queue;
        case EXIT_LEVEL_VIRTUAL_DEVICE:
            vkDestroyDevice(vksystem->virtual_device, nullptr);
        case EXIT_LEVEL_INSTANCE:
            vkDestroyInstance(vksystem->instance, nullptr);
        case EXIT_LEVEL_NONE:
            break;
        }
       free_required(vksystem);
    }

      vksystem_down = true;

    return;
};

template<typename type>
void free_local(type *which){
    type *entity = which; 
    type *next = nullptr;
    while(entity != nullptr){
       next = entity->next;
       delete entity;
       std::cout<<"FREED : "<<entity<<std::endl;
       entity = next;
     }
};

enum memory_type test_adeng;

void flexon_vulkan_renderer::free_required(VkSystem* vksystem)
{
     delete vksystem->render_pool.frame_buffer;
     delete vksystem->render_pool.image_cmd_buffer;
     free_local(vksystem->render_pool.pipeline->shader);
     free_local(vksystem->render_pool.pipeline);
     free_local(vksystem->render_pool.memory->memory_layout);
     free_local(vksystem->render_pool.memory);

};

