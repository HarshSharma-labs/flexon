// #define VK_ONLY_EXPORTED_PROTOTYPES
#ifndef __FLEXON_VULKAN_IMPLEMENTATION
#define __FLEXON_VULKAN_IMPLEMENTATION
#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>
#include <map>
#include <unordered_map>
#include <set>
#include <deque>
#include <functional>
#include <vector>
#include <vulkan/vulkan.h>
#include <wayland-client.h>
#include "../../components/base.hpp"

#ifndef NDEBUG
#define VALIDATION_LAYER_MAX 1
#endif

#define TOTAL_QUEUE_COUNT 1
#define IMAGE_COUNT 3


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
//defines index in queuearray;




enum SHADER_ID{
  NORMAL_SHADER_ID = 1,
  IMAGE_SHADER_ID,
  BOX_SHADER_ID,
};
enum SHADER_TYPES{
 NORMAL_SHADER = 1,
 IMAGE_SHADER = -1,
 BOX_SHADER = -1,
// SHADER_QUEUE_TOTAL,
  
};
enum MEMORY_HOLDER{
  IMAGE_MEMORY = 1,
  STAGING_DISPLAY_MEMORY,
  NORMAL_SHADER_MEMORY,
  IMAGER_SHADER_MEMORY,
  BOX_SHADER_MEMORY,
};

struct sdev{
 VkPhysicalDevice dev;
 VkPhysicalDeviceType type;
 uint32_t memoryTypeIndex;
 uint32_t queueFamilyIndex;
};

struct vdev{
 VkDevice dev;
 VkQueue queue;
 VkCommandPool vkcmdpool;
 VkImage *img;
 VkImageView *imgView;
 VkBuffer winBuffer;
 VkBuffer vertexBuffer;
 VkBuffer uniformBuffer;
 VkBuffer indexBuffer;
 VkDescriptorPool descriptorpool;
 uint32_t *vkpixel = nullptr;
 uint32_t imgCount;
 uint32_t qCount;
 float qPriority;
 VkFormat imgformat;
};

struct memlayout{
   VkDeviceSize partsize;
   VkDeviceSize offset;
 };

struct memory{
 VkDeviceMemory mem;
 VkDeviceSize memsize;
 struct memlayout *layout;
 uint32_t layoutCount;
};

typedef struct buffercreateinfo{ 
  VkBufferUsageFlags usage_mode {0};
  VkBufferCreateFlags flags {0};
  VkDeviceSize size {0};
  struct memory mem;
}buffercreateinfo;

typedef struct shadercompile{
uint32_t vcodesize;
uint32_t *vtxCode;
uint32_t fcodesize;
uint32_t *frgCode;
}shadercompile;

typedef struct modulereturn{
 VkShaderModule vertex;
 VkShaderModule fragment;
}modulereturn;

typedef struct shaderWrapper{
 uint32_t shaderType;
 VkPipeline shaderPipeline;
 VkPipelineLayout pipelineLayout;
 modulereturn shaderModule;
 VkDescriptorSetLayout descriptorlayout;
 VkSampler sampler;
 VkCommandBuffer cmdbuffer;
 void *vertpointer;
 void *uniformPtr;
 uint32_t vstride;
 uint32_t fstride;
 uint32_t bindingloc;
 uint32_t vertbufSize;
 VkFence fence;
}shaderWrapper;

class flexonrenderer {
public:

  int initlise();
  void setExtents(uint32_t *pixel , vec2<uint32_t> extents);
  void destroy();
private:
 uint32_t vkapiversion;

 bool vksysdown = false;

 const int extensionCount = 1;
 const char* extensionName[1] = {VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME};

 struct sdev vkphyDev;
 struct vdev vkvirtDev;
 uint32_t *pixel;

 uint32_t idximage = 0;

 vec2<uint32_t> imageExtents;

 VkInstance vkinstance = VK_NULL_HANDLE;
 std::unordered_map<uint32_t,memory> vkmemory;
 std::unordered_map<uint32_t,shaderWrapper> shaders;

 VkMemoryPropertyFlags imemtype = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
                                 | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                                 | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

 VkMemoryPropertyFlags dmemtype = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
                                 

 struct sdev selectPhysicalDevice(VkInstance instance , auto &devices);
 int getQueueFamilyIndex(VkPhysicalDevice dev , sdev &store);
 int scoreDevice(VkPhysicalDevice device ,sdev &store);
 int getMemoryIndex(VkPhysicalDevice device , uint32_t typefilter);
 struct memory createImage(VkImage **store , VkImageView **viewstore , 
                           uint32_t cimg,uint32_t cview,
                           vec2<uint32_t> dimension);

 VkBuffer createBuffer(buffercreateinfo &info);
 VkCommandBuffer createcmdbuffer();

 shadercompile compileshader(const char* vertCode, const char* fragCode);
 bool createPipeline(shaderWrapper &shader);
 modulereturn createShaderModule(shadercompile &rawcode);
 VkDescriptorSetLayout createDescriptorSet(shaderWrapper &shader);

#ifndef NDEBUG
    const int enableLayerCount = VALIDATION_LAYER_MAX;
    const char* validationLayer[VALIDATION_LAYER_MAX] = {
        "VK_LAYER_KHRONOS_validation"
    };  
#endif

bool createInstance();
bool getPhysicalDevice();
bool createVirtualDevice();
bool getQueue();
bool createCmdPool();
bool createImage();
bool createShader();

void togglestate();
void renderframe();
void transitionlayoutstart(shaderWrapper &shaderinfo);
void transitionlayoutend(shaderWrapper &shaderinfo);
void getRenderedBuffer(shaderWrapper &shaderinfo,vec4<uint32_t> extents);
void startcmdbuffer(shaderWrapper &shaderinfo);
void endcmdbuffer(shaderWrapper &shaderinfo);
void exit_vulkan(enum exitlevel level);

};

#endif
