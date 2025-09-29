#include "./vulkan.hpp"
#include "./shaders/shader_code.hpp"
#include <shaderc/shaderc.hpp>


struct DeletionQueue
{
	std::vector<VkImage> image;
  std::vector<VkImageView> imgview;
  std::vector<VkCommandBuffer> cmdbuf;
  std::vector<VkFence> fence;
  std::vector<VkShaderModule> shader;
  std::vector<VkBuffer> buffer;
  std::vector<VkDeviceMemory> memory;
  std::vector<VkPipeline> pipeline;
  VkDevice mine;

  void initlise(VkDevice dev){
    mine = dev;
    image.reserve(100);
    imgview.reserve(100);
    cmdbuf.reserve(100);
    fence.reserve(100);
    shader.reserve(100);
    buffer.reserve(100);
    memory.reserve(10);
    pipeline.reserve(10);
  };

  void pushshader(VkShaderModule mod){
    shader.push_back(mod);
  };
  void pushfence(VkFence fen){
    fence.push_back(fen);
  };
  void pushcmdbuf(VkCommandBuffer buf){
    cmdbuf.push_back(buf);
  }
  void pushimage(VkImage img){
    image.push_back(img);
  };
  void pushimageview(VkImageView view){
    imgview.push_back(view);
  };
  
  void pushbuffer(VkBuffer tmp){
    buffer.push_back(tmp);
  };
  
  void pushmemory(VkDeviceMemory tmp){
    memory.push_back(tmp);
  };
  
	void flush() {
   for(auto &itr: shader)
     vkDestroyShaderModule(mine,itr,VK_NULL_HANDLE);
	
   for(auto &itr: fence)
     vkDestroyFence(mine,itr,VK_NULL_HANDLE);
  
    for(auto &itr: imgview)
     vkDestroyImageView(mine , itr, VK_NULL_HANDLE);
    
    for(auto &itr: image)
     vkDestroyImage(mine,itr,VK_NULL_HANDLE);

    for(auto &itr: buffer)
     vkDestroyBuffer(mine,itr,VK_NULL_HANDLE);
  
  
    for(auto &itr:memory)
      vkFreeMemory(mine, itr, VK_NULL_HANDLE);

  }

};

struct DeletionQueue delQue;

static VkDeviceSize align_offset(VkDeviceSize alignment, VkDeviceSize value)
{
    if (alignment == 0)
        return value; // avoid division by zero
    return (value + alignment - 1) & ~(alignment - 1);
}


#ifndef NDEBUG
static bool checkValidationLayerSupport(const char** layerName, const int* count)
{

    uint32_t layersCount { 0 };

    vkEnumerateInstanceLayerProperties(&layersCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layersCount);
    vkEnumerateInstanceLayerProperties(&layersCount, availableLayers.data());
    for (int i = 0; i < *count; i++) {
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName[i], layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) { 
            return false;
        }
    }
    return true;
}

#endif

struct sdev flexonrenderer::selectPhysicalDevice(VkInstance instance , auto &devices){

  std::multimap<int,sdev> sortedmap;
  struct sdev dev = {0};
  for(auto &device : devices){
      dev.dev = device;
      int score = scoreDevice(device,dev);
      std::cout<<"[DEVICE] Score of device "<<score<<std::endl;
      sortedmap.insert({score,dev});
   };
 
  if (sortedmap.rbegin()->first > 0){
     return sortedmap.rbegin()->second;
  }

  return dev;
};

int flexonrenderer::scoreDevice(VkPhysicalDevice device , sdev &store){
  
   int evaluated_score = 0;
   uint32_t deviceSupportedExtension = 0;
 
    create_struct(dev_properties, VkPhysicalDeviceProperties);
    create_struct(physical_device_feature, VkPhysicalDeviceFeatures);

    vkGetPhysicalDeviceProperties(device, &dev_properties);
    vkGetPhysicalDeviceFeatures(device, &physical_device_feature);

    vkEnumerateDeviceExtensionProperties(device, NULL, &deviceSupportedExtension, NULL);
    std::vector<VkExtensionProperties> ext(deviceSupportedExtension);
    vkEnumerateDeviceExtensionProperties(device, NULL, &deviceSupportedExtension, ext.data());


  for (int i = 0; i < extensionCount; i++) {
        bool layerFound = false;
        for (const auto& itr : ext) {
            if (strcmp(extensionName[i], itr.extensionName) == 0) {
                layerFound = true;
                std::cout<<"[INFO] Extension supported : "<<extensionName[i]<<std::endl;
                break;
            }
        }

   };
    store.type = dev_properties.deviceType;
    evaluated_score += 1000;

    if (!physical_device_feature.geometryShader) {
        evaluated_score -= 10000;
        return evaluated_score;
    }

  
    switch (dev_properties.deviceType) {

    case VK_PHYSICAL_DEVICE_TYPE_OTHER:
        evaluated_score -= 1000;
        return evaluated_score;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        evaluated_score += 500;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        evaluated_score += 1000;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
        evaluated_score += 250;
        return 0;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
        evaluated_score += 100;
        return 0;
        break;
    }
  
   if (getQueueFamilyIndex(device,store) == -1) {
         return 0;
     };   

    evaluated_score += 1000;

  return evaluated_score;

};

int flexonrenderer::getQueueFamilyIndex(VkPhysicalDevice dev,sdev &store){

    uint32_t queue_family_count = 0;
    uint32_t queue_family_index = 0;
    bool success = false;

    vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_family_count, nullptr);
    if (queue_family_count == (uint32_t)0) {
        std::cout << "[QUEUE FAMILY] Queue family count is 0.\n";
        return -1;
    }
    
    std::vector<VkQueueFamilyProperties> queue_family_struct(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_family_count, queue_family_struct.data());

    for (const auto& queue_iterator : queue_family_struct) {

        if (queue_iterator.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            success = true;
            return 0;
            
        }
        queue_family_index++;
    }
    if(success == false)
     return -1;

    store.queueFamilyIndex = queue_family_index;

    return 0;
};


int flexonrenderer::getMemoryIndex(VkPhysicalDevice device , uint32_t typefilter){

   create_struct(memProperties,VkPhysicalDeviceMemoryProperties);
   vkGetPhysicalDeviceMemoryProperties(device,&memProperties);
  
   VkMemoryPropertyFlags rmemt {0};
   switch(vkphyDev.type){
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        rmemt = imemtype;
        break;
     case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        rmemt = dmemtype;
    break;
   };

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typefilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & rmemt ) == rmemt) {
            return i; // Found a valid memory type
        }
    }


  return -1;

};

/*
*
*   ___________________________________________________
*   |                                                 |
*   |   CODE TO INITLISE VULKAN START                 |
*   |                                                 |
*   |                                                 |
*   --------------------------------------------------- 
*/

int flexonrenderer::initlise(){
  
vkvirtDev = (struct vdev){
     .dev = VK_NULL_HANDLE,
     .queue = VK_NULL_HANDLE,
     .vkcmdpool = VK_NULL_HANDLE,
     .imgCount = 3,
     .qCount = 1,  
     .qPriority = 1.0f,
     .imgformat = VK_FORMAT_R8G8B8A8_SRGB
    };

#ifndef NDEBUG
    if (!checkValidationLayerSupport(validationLayer, &enableLayerCount)) {
        std::cout << "Validation Layer not supported aborting" << std::endl;
        std::abort();
    }
#endif

    if (vkEnumerateInstanceVersion(&vkapiversion) != VK_SUCCESS) {
        std::cout << "Error probing api verision" << std::endl;
        return -1;
    }
    if(!createInstance()){
         std::cout << "Error instance creation failed" << std::endl;
         return -1;
    }
    if(!getPhysicalDevice()){
         std::cout << "Error physical device selection failed" << std::endl;
         return -1;
    }
    if(!createVirtualDevice()){
         std::cout << "Error creation of virtual device failed" << std::endl;
         return -1;
    }
    delQue.initlise(vkvirtDev.dev);
    if(!getQueue()){
         std::cout << "Queue retrival failed" << std::endl;
         return -1;
    }
    if(!createCmdPool()){
         std::cout << "Command pool creation failed" << std::endl;
         return -1;
    }
   if(!createShader()){
         std::cout<<"Command shader  Creation Failed"<<std::endl;
    } 
  return 0;
};

bool flexonrenderer::createInstance(){
    create_struct(app_info, VkApplicationInfo,
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "hello",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "hello",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = vkapiversion
        );

    create_struct(app_instance, VkInstanceCreateInfo,
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = (uint32_t)0,
        .ppEnabledLayerNames = NULL,
        .enabledExtensionCount = (uint32_t)0,
        .ppEnabledExtensionNames = NULL
        );
#ifndef NDEBUG
    app_instance.enabledLayerCount = static_cast<uint32_t>(enableLayerCount);
    app_instance.ppEnabledLayerNames = validationLayer;
#endif

    if (vkCreateInstance(&app_instance, NULL, &vkinstance) != VK_SUCCESS) {
        std::cout << "instance creation failed" << std::endl;
        return false;
    }
    
  return true;
};

bool flexonrenderer::getPhysicalDevice(){
    uint32_t device_count { 0 };

    vkEnumeratePhysicalDevices(vkinstance, &device_count, nullptr);
    if (device_count == 0) {
        std::cout << "[DEVICE ERROR] Failed to find a GPU with the vulkan support. \n";
        return false;
    }

    std::vector<VkPhysicalDevice> available_devices(device_count);
    vkEnumeratePhysicalDevices(vkinstance, &device_count, available_devices.data());

    vkphyDev = selectPhysicalDevice(vkinstance,available_devices);
    if(vkphyDev.dev == VK_NULL_HANDLE)
      return false;

    std::cout << "[DEVICE SUCCESS] selected a physical Device\n";

    return true;

};

bool flexonrenderer::createVirtualDevice(){

  create_struct(queue_family_struct, VkDeviceQueueCreateInfo,
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = NULL,
        .queueCount = vkvirtDev.qCount,
        .pQueuePriorities = &vkvirtDev.qPriority
        );

     queue_family_struct.queueFamilyIndex = (uint32_t)vkphyDev.queueFamilyIndex;


    create_struct(dynamic_rendering_feature, VkPhysicalDeviceDynamicRenderingFeaturesKHR,
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
        .dynamicRendering = VK_TRUE);

    create_struct(device_features, VkPhysicalDeviceFeatures);

    create_struct(device_create_info, VkDeviceCreateInfo,
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &dynamic_rendering_feature,
        .queueCreateInfoCount = (uint32_t)1,
        .pQueueCreateInfos = &queue_family_struct,
        .pEnabledFeatures = &device_features);

    device_create_info.enabledExtensionCount = (uint32_t)extensionCount;
    device_create_info.ppEnabledExtensionNames = extensionName;

    if (vkCreateDevice(vkphyDev.dev, &device_create_info,
            nullptr, &vkvirtDev.dev)
        != VK_SUCCESS) {
        std::cout << "[DEVICE] Creation of logical Device failed. \n";
    }

        std::cout << "[DEVICE] Creation of logical Device success. \n";
    return true;
}

bool flexonrenderer::getQueue(){

vkGetDeviceQueue(vkvirtDev.dev, vkphyDev.queueFamilyIndex,
        0, &vkvirtDev.queue);

    if (vkvirtDev.queue == nullptr)
        return false;
      std::cout << "[QUEUE] Retrived queue successfull. \n";
  
  return true;

};

bool flexonrenderer::createCmdPool(){

  create_struct(cmd_pool_info, VkCommandPoolCreateInfo,
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = NULL,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = vkphyDev.queueFamilyIndex);

    if (vkCreateCommandPool(vkvirtDev.dev, &cmd_pool_info,nullptr, &vkvirtDev.vkcmdpool)!= VK_SUCCESS)
        return false;

    std::cout << "[COMMANDPOOL] Creation of command pool OK \n";
    

  return true;
};

VkCommandBuffer flexonrenderer::createcmdbuffer(){

   VkCommandBuffer tmp;

   create_struct(cmd_buffer_allocation_info, VkCommandBufferAllocateInfo,
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = NULL,
        .commandPool = vkvirtDev.vkcmdpool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = (uint32_t)1
        );

    if (vkAllocateCommandBuffers(vkvirtDev.dev, &cmd_buffer_allocation_info,&tmp) != VK_SUCCESS)
        return VK_NULL_HANDLE;

    delQue.pushcmdbuf(tmp);

    std::cout << "[COMMAND BUFFER] Creation of command buffer OK \n";

    return tmp;
};

void flexonrenderer::setExtents(uint32_t *pixel , vec2<uint32_t> extents){

   imageExtents = extents; 
   struct memory tmp = createImage(&vkvirtDev.img,&vkvirtDev.imgView,
                                   vkvirtDev.imgCount, vkvirtDev.imgCount,imageExtents); 
   if(tmp.layout == nullptr)
     return;

   vkmemory[IMAGE_MEMORY] = tmp;
    
    buffercreateinfo info = {
     .usage_mode = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
     .flags = 0,
     .size = (imageExtents.x * imageExtents.y) * sizeof(uint32_t) * 2
    };

    vkmemory[STAGING_DISPLAY_MEMORY] = info.mem;
    vkvirtDev.winBuffer = createBuffer(info);
    vkMapMemory(vkvirtDev.dev,info.mem.mem,0,VK_WHOLE_SIZE,0,(void**)&vkvirtDev.vkpixel);
    
   
  return;
};


struct memory flexonrenderer::createImage(VkImage **store , VkImageView **viewstore , 
                                            uint32_t cimg, uint32_t cview, vec2<uint32_t> dimension){

  
  VkImage *imgtmp = new VkImage[cimg];
  struct memory tmpmem = {0};
  tmpmem.layout = new struct memlayout[cimg];
  tmpmem.layoutCount = cimg;

   if(cimg == 0)
       return {0};

  create_struct(vk_image_info, VkImageCreateInfo,
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_SRGB,
        .extent = { .width = dimension.x, .height = dimension.y, .depth = (uint32_t)1 },
        .mipLevels = (uint32_t)1,
        .arrayLayers = (uint32_t)1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
            | VK_IMAGE_USAGE_TRANSFER_SRC_BIT
            | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED);

  create_struct(memreq, VkMemoryRequirements);
  

 
  for(uint32_t i = 0 ; i < cimg  ; i++){
    if (vkCreateImage(vkvirtDev.dev, &vk_image_info,
            NULL, (imgtmp + i))
        != VK_SUCCESS) {
        return {0};
    }
    delQue.pushimage(imgtmp[i]);

    vkGetImageMemoryRequirements(vkvirtDev.dev , imgtmp[i], &memreq);
      VkDeviceSize alignedoffset = align_offset(tmpmem.memsize,memreq.alignment);
   
      tmpmem.memsize +=  alignedoffset + memreq.size;
      tmpmem.layout[i].offset = alignedoffset; 
      tmpmem.memsize += memreq.size;
};

  int memidx = getMemoryIndex( vkphyDev.dev , memreq.memoryTypeBits);  

  create_struct(mem_layout_size_info, VkMemoryAllocateInfo,
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = NULL,
        .allocationSize = tmpmem.memsize,
        .memoryTypeIndex = (uint32_t)memidx);

   if(vkAllocateMemory(vkvirtDev.dev, &mem_layout_size_info,
        VK_NULL_HANDLE, &tmpmem.mem) != VK_SUCCESS){
     return {0};
     }
     delQue.pushmemory(tmpmem.mem);

    for(int i = 0 ; i < cimg ; i++)
      vkBindImageMemory(vkvirtDev.dev, imgtmp[i] , tmpmem.mem , tmpmem.layout[i].offset);
 
     *store = imgtmp;

  if(viewstore != nullptr){
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
      
      VkImageView *tmpView = new VkImageView[cview];
      *viewstore = tmpView;

      for(int i = 0 ; i < cview ; i++){
       image_view_create_info.image = imgtmp[i];
       if (vkCreateImageView(vkvirtDev.dev, &image_view_create_info,
             NULL, (tmpView+i))
         != VK_SUCCESS) {
        return {0};
       }
      delQue.pushimageview(tmpView[i]);

      };

    };
      return tmpmem;
};


VkBuffer flexonrenderer::createBuffer(buffercreateinfo &info){

  
  VkBuffer tmp = VK_NULL_HANDLE;

   create_struct(buffer_create_info, VkBufferCreateInfo,
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = NULL,
        .flags = info.flags,
        .size = info.size,
        .usage = info.usage_mode,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = vkvirtDev.qCount,
        .pQueueFamilyIndices = &vkphyDev.queueFamilyIndex
       );

  
    vkCreateBuffer(vkvirtDev.dev,&buffer_create_info,NULL,&tmp);

    create_struct(memreq, VkMemoryRequirements);
    vkGetBufferMemoryRequirements(vkvirtDev.dev, tmp, &memreq);
    int memidx = getMemoryIndex( vkphyDev.dev , memreq.memoryTypeBits);  

   create_struct(memAllocateInfo, VkMemoryAllocateInfo,
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = NULL,
        .allocationSize = memreq.size,
        .memoryTypeIndex = (uint32_t)memidx);

  if(vkAllocateMemory(vkvirtDev.dev, &memAllocateInfo,
          VK_NULL_HANDLE, &info.mem.mem) != VK_SUCCESS){
       return {0};
    }
   vkBindBufferMemory(vkvirtDev.dev,tmp,info.mem.mem,0);
   delQue.pushbuffer(tmp);
   delQue.pushmemory(info.mem.mem);


   return tmp;

};
/*
*
*
*   CODE TO INITLISE VULKNA END
*
*
*/

/*
*
*
*   CODE TO CLEAN SYSTEM BEFORE EXITING ONLY BELOW
*
*
*/

void flexonrenderer::destroy(){
exit_vulkan(EXIT_LEVEL_ALL);
};


void flexonrenderer::exit_vulkan(enum exitlevel level)
{

    if (vksysdown == false) {
        switch (level) {
        case EXIT_LEVEL_ALL:
        case EXIT_LEVEL_GRAPHICS_PIPELINE:
        case EXIT_LEVEL_RENDER_BUFFER:
        case EXIT_LEVEL_RENDER_PASS:
        case EXIT_LEVEL_CMD_BUFFER:
        case EXIT_LEVEL_CMD_POOL:
        case EXIT_LEVEL_RESOURCE:
        case EXIT_LEVEL_QUEUE:
        delQue.flush();
            vkDestroyCommandPool(vkvirtDev.dev,vkvirtDev.vkcmdpool,nullptr);
        case EXIT_LEVEL_VIRTUAL_DEVICE:
            vkDestroyDevice(vkvirtDev.dev, nullptr);
        case EXIT_LEVEL_INSTANCE:
            vkDestroyInstance(vkinstance, nullptr);
        case EXIT_LEVEL_NONE:
            break;
        }
  //   free_required(vksystem);
    }

    vksysdown = true;

    return;
};

/*
*
*
*   CODE TO CLEAN SYSTEM END
*
*
*/


/*
*
*   ___________________________________________________
*   |                                                 |
*   |   CODE TO CREATE SHADER PIPELINE                |
*   |                                                 |
*   |                                                 |
*   --------------------------------------------------- 
*/

shadercompile flexonrenderer::compileshader(const char* vertCode, const char* fragCode)
{

    
    shadercompile retinfo = {0};

    size_t vertex_code_length = strlen(vertCode);
    size_t fragment_code_length = strlen(fragCode);
    
    shaderc::CompileOptions shader_compiler_options;

    shaderc::Compiler shader_compiler;
    shaderc::SpvCompilationResult vertex_shader_result = shader_compiler.CompileGlslToSpv(vertCode,
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

    retinfo.vcodesize = (uint32_t)vert_size * sizeof(uint32_t);
    retinfo.vtxCode = itr_vert;

    size_t index = 0;
    for (auto& iterator : vertex_shader_result) {
        itr_vert[index] = iterator;
        index += 1;
    }

    shaderc::SpvCompilationResult frag_shader_result = shader_compiler.CompileGlslToSpv(fragCode,
        fragment_code_length,
        shaderc_glsl_fragment_shader,
        "shader.frag",
        shader_compiler_options);

    int shader_status_frag = frag_shader_result.GetCompilationStatus();
    if (shader_status_frag != shaderc_compilation_status_success) {
        std::cout << "[COMPILATION] Compilation of fragment shader failed.\n";
        std::cout<<frag_shader_result.GetErrorMessage()<<std::endl;
    }

  
    size_t frag_size = 0; // size w.r.t uint32_t
    for (auto& iterator : frag_shader_result) {
        frag_size += 1;
     }
  
    uint32_t* itr_frag = new uint32_t[frag_size];
   
    retinfo.fcodesize = (uint32_t)frag_size * sizeof(uint32_t); // convert size to bytes;
    retinfo.frgCode = itr_frag;
   
    size_t idx = 0;
    for (auto& iterator : frag_shader_result) {
        itr_frag[idx] = iterator;
        idx += 1;
    }

    return retinfo;
}

modulereturn flexonrenderer::createShaderModule(shadercompile &rawcode){
   modulereturn modu = {0};

   create_struct(vert_module_create_info, VkShaderModuleCreateInfo,
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = NULL,
        .codeSize = rawcode.vcodesize,
        .pCode = rawcode.vtxCode);

    create_struct(frag_module_create_info, VkShaderModuleCreateInfo,
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = NULL,
        .codeSize = rawcode.fcodesize,
        .pCode = rawcode.frgCode);

    vkCreateShaderModule(vkvirtDev.dev, &vert_module_create_info,
        NULL, &modu.vertex);

    vkCreateShaderModule(vkvirtDev.dev, &frag_module_create_info,
        NULL, &modu.fragment);

      delQue.pushshader(modu.vertex);
      delQue.pushshader(modu.fragment);

      delete rawcode.vtxCode;
      delete rawcode.frgCode;
  return modu;
};



bool flexonrenderer::createShader(){
  buffercreateinfo vertexbuffer = {0};
  buffercreateinfo uniformbuffer = {0};
  buffercreateinfo indexbuffer = {0};

  vertexbuffer.usage_mode = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  uniformbuffer.usage_mode = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  indexbuffer.usage_mode = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

  shaderWrapper normalshaderWrapper = {0};
  if(NORMAL_SHADER == 1){
   vertexbuffer.size = sizeof(vertexshaderdata) * NORMAL_SHADER_VERTEX_SIZE;
   uniformbuffer.size = sizeof(normalshaderuniform) * NORMAL_SHADER_VERTEX_SIZE;
   indexbuffer.size = sizeof(vertexshaderdata) * NORMAL_SHADER_VERTEX_SIZE;
   shadercompile normalcompiled = compileshader(normalShaderCodeVertex,normalShaderCodeFragment); 
   normalshaderWrapper.shaderModule = createShaderModule(normalcompiled);
  }
 
  vkvirtDev.vertexBuffer = createBuffer(vertexbuffer); 
  vkvirtDev.uniformBuffer = createBuffer(uniformbuffer); 
  vkvirtDev.indexBuffer = createBuffer(indexbuffer);


 return true;
};




bool flexonrenderer::createPipeline(shaderWrapper &shader){
  

  create_struct(pipeline_layout_info, VkPipelineLayoutCreateInfo,
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = NULL,
        .setLayoutCount = (uint32_t)0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = (uint32_t)0,
        .pPushConstantRanges = nullptr, );
    if (vkCreatePipelineLayout(vkvirtDev.dev, &pipeline_layout_info, nullptr, &shader.pipelineLayout) 
    != VK_SUCCESS) {
        std::cout << "[PIPELINE] Layout Creation Failed\n";
        return false;
    }

    VkPipelineShaderStageCreateInfo shader_stage[2];
    shader_stage[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    shader_stage[0].pNext = NULL;
    shader_stage[0].flags = 0;
    shader_stage[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shader_stage[0].module = shader.shaderModule.vertex;
    shader_stage[0].pName = "main";
    shader_stage[0].pSpecializationInfo = NULL;

    shader_stage[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    shader_stage[1].pNext = NULL;
    shader_stage[1].flags = 0,
    shader_stage[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shader_stage[1].module = shader.shaderModule.fragment;
    shader_stage[1].pName = "main";
    shader_stage[1].pSpecializationInfo = NULL;
 
    create_struct(vertex_binding_info,VkVertexInputBindingDescription,
                  .binding = 0,
                  .stride = shader.vstride,
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
        .pColorAttachmentFormats = &vkvirtDev.imgformat,
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
        .layout = shader.pipelineLayout,
        .renderPass = VK_NULL_HANDLE, );

    vkCreateGraphicsPipelines(vkvirtDev.dev, VK_NULL_HANDLE, (uint32_t)1,
        &graphics_pipeline_create_info, VK_NULL_HANDLE, &shader.shaderPipeline);

   return true;
};


/*
*
*
*   CODE TO CREATE SHADER PIPELINE END
*
*
*/

/*
*
*   ___________________________________________________
*   |                                                 |
*   |   CODE TO HANDLE RENDERING START                |
*   |                                                 |
*   |                                                 |
*   --------------------------------------------------- 
*/


void flexonrenderer::togglestate(){
    int current_index = idximage + 1;
    bool smaller = (current_index < vkvirtDev.imgCount);
    current_index = current_index * smaller;
    idximage = current_index;
};

void flexonrenderer::renderframe(){
};

void flexonrenderer::transitionlayoutstart(shaderWrapper &shaderinfo){

 
  static struct {
    uint32_t queueidx;
    VkImage image;
   }info;

   info.queueidx = vkphyDev.queueFamilyIndex; 
   info.image = vkvirtDev.img[idximage];

    create_struct(barrier_info, VkImageMemoryBarrier,
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .srcQueueFamilyIndex = info.queueidx,
        .dstQueueFamilyIndex = info.queueidx,
        .image = info.image,
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
        shaderinfo.cmdbuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier_info);

  return;

};


void flexonrenderer::transitionlayoutend(shaderWrapper &shaderinfo){

  static struct {
    uint32_t queueidx;
    VkImage image;
   }info;

   info.queueidx = vkphyDev.queueFamilyIndex; 
   info.image = vkvirtDev.img[idximage];

    create_struct(barrier_info, VkImageMemoryBarrier,
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        .srcQueueFamilyIndex = info.queueidx,
        .dstQueueFamilyIndex = info.queueidx,
        .image = info.image,
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
        shaderinfo.cmdbuffer,
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier_info);

    return;

};

void flexonrenderer::startcmdbuffer(shaderWrapper &shaderinfo){

  static struct {
    VkCommandBuffer cmd;
    VkImageView view;
    VkPipeline pipeline;
  }info;

  info.cmd = shaderinfo.cmdbuffer;
  info.view = vkvirtDev.imgView[idximage];

    create_struct(cmd_buffer_info, VkCommandBufferBeginInfo,
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = NULL,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    vkBeginCommandBuffer(info.cmd, &cmd_buffer_info);
    transitionlayoutstart(shaderinfo);

    create_struct(color_attachment_info, VkRenderingAttachmentInfo,
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .pNext = NULL,
        .imageView = info.view,
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .resolveMode = VK_RESOLVE_MODE_NONE,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue = { { 1.0f, 1.0f, 1.0f, 1.0f } });

    create_struct(rendering_info, VkRenderingInfoKHR,
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR,
        .pNext = NULL,
        .renderArea = {
            .offset = {
                .x = 0,
                .y = 0 },
            .extent = { .width = 500 , .height = 500 } },
        .layerCount = (uint32_t)1,
        .viewMask = (uint32_t)0,
        .colorAttachmentCount = (uint32_t)1, 
        .pColorAttachments = &color_attachment_info
                  );

    vkCmdBeginRendering(info.cmd, &rendering_info);
  /*
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
    */
    return;

};
void flexonrenderer::endcmdbuffer(shaderWrapper &shaderinfo){

  struct {
    VkCommandBuffer cmd;
    VkImage image;
    VkPipeline pipeline;
  }info;

  info.cmd = shaderinfo.cmdbuffer;
  info.image = vkvirtDev.img[idximage];

   vkCmdEndRendering(info.cmd);

  transitionlayoutend(shaderinfo);

     create_struct(submit_info, VkSubmitInfo,
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = NULL,
        .waitSemaphoreCount = (uint32_t)0,
        .pWaitSemaphores = nullptr,
        .pWaitDstStageMask = (uint32_t)0,
        .commandBufferCount = (uint32_t)1,
        .pCommandBuffers = &info.cmd,
        .signalSemaphoreCount = (uint32_t)0,
        .pSignalSemaphores = nullptr);

    vkResetFences(vkvirtDev.dev, 1,&shaderinfo.fence);

    vkQueueSubmit(vkvirtDev.queue, 1, &submit_info,shaderinfo.fence);


};

void flexonrenderer::getRenderedBuffer(shaderWrapper &shaderinfo,vec4<uint32_t> extents){
 struct {
    VkCommandBuffer cmd;
    VkImage img;
  }info;

  info.cmd = shaderinfo.cmdbuffer;
  info.img = vkvirtDev.img[idximage];

    VkBufferImageCopy region {};
    region.bufferOffset = 0;
    region.bufferRowLength = extents.z; // Tightly packed
    region.bufferImageHeight = extents.w;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { (int32_t)extents.x, (int32_t)extents.y, 0 };
    region.imageExtent = { extents.z, extents.w, 1 };

    vkCmdCopyImageToBuffer(
        info.cmd,
        info.img, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        vkvirtDev.winBuffer,
        1, &region);

    vkEndCommandBuffer(info.cmd);

}

/*
*
*
*   CODE TO HANDLE RENDERING END
*
*
*/

