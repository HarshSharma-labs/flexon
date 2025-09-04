#include "vulkan.hpp"

#ifndef NDEBUG
bool vulkan_renderer::checkValidationLayerSupport(const char** layerName, const int* count)
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
            std::cout << "[VALIDATION LAYER] Error requested layer doesn't exist : " << validationLayer[i] << std::endl;
            return false;
        }
    }
    return true;
}

#endif

bool vulkan_renderer::filter_physical_device(VkSystem* vksystem, std::vector<VkPhysicalDevice>& devices_list)
{

    std::multimap<int, physical_device_conf> sorted_device;
    std::vector<physical_device_conf> device_conf(devices_list.size());
    int i = 0;
    for (const auto& device : devices_list) {
        int score = score_device(vksystem, &device_conf[i], device);
        std::cout << "[DEVICE SCORE] score of devices : " << score << '\n';
        sorted_device.insert(std::make_pair(score, device_conf[i]));
        i += 1;
    }
    if (sorted_device.rbegin()->first > 0) {
        memcpy(&vksystem->physical_device, &sorted_device.rbegin()->second.device_score, sizeof(physical_device_conf));
        return true;
    }
    return false;
}

int vulkan_renderer::score_device(VkSystem* vksystem, physical_device_conf* device_conf, VkPhysicalDevice which_device)
{

    int evaluated_score = 0;
    uint32_t deviceSupportedExtension = 0;

    device_conf->device = which_device;

    create_struct(dev_properties, VkPhysicalDeviceProperties);
    create_struct(physical_device_feature, VkPhysicalDeviceFeatures);

    vkGetPhysicalDeviceProperties(which_device, &dev_properties);
    vkGetPhysicalDeviceFeatures(which_device, &physical_device_feature);

    vkEnumerateDeviceExtensionProperties(which_device, NULL, &deviceSupportedExtension, NULL);
    std::vector<VkExtensionProperties> ext(deviceSupportedExtension);
    vkEnumerateDeviceExtensionProperties(which_device, NULL, &deviceSupportedExtension, ext.data());

   
       for (int i = 0; i < vksystem->device_extension_count; i++) {
        bool layerFound = false;
        for (const auto& itr : ext) {
            if (strcmp(vksystem->extension_name[i], itr.extensionName) == 0) {
                layerFound = true;
                std::cout<<"[INFO] Extension supported : "<<vksystem->extension_name[i]<<std::endl;
                break;
            }
        }
        if (!layerFound) {
            std::cout << "[VALIDATION LAYER] Error requested layer doesn't exist : " << validationLayer[i] << std::endl;
            return false;
        }
    }

    evaluated_score += 1000;

    if (!physical_device_feature.geometryShader) {
        evaluated_score -= 1000;
        return evaluated_score;
    }
    if (!find_suitable_queue_family(vksystem, device_conf, which_device)) {
        evaluated_score -= 10000;
    };

    evaluated_score += 1000;

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
        break;
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
        evaluated_score += 100;
        break;
    }
    device_conf->device_score = evaluated_score;
    return evaluated_score;
}

bool vulkan_renderer::find_suitable_queue_family(VkSystem* vksystem, physical_device_conf* device_config, VkPhysicalDevice& phy_device)
{

    uint32_t queue_family_count = 0;
    uint32_t queue_family_index = 0;
    bool success = false;

    vkGetPhysicalDeviceQueueFamilyProperties(phy_device, &queue_family_count, nullptr);
    if (queue_family_count == (uint32_t)0) {
        std::cout << "[QUEUE FAMILY] Queue family count is 0.\n";
        return success;
    }
    std::vector<VkQueueFamilyProperties> queue_family_struct(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(phy_device, &queue_family_count, queue_family_struct.data());

    for (const auto& queue_iterator : queue_family_struct) {
        if (queue_iterator.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            success = true;
            break;
        }
        queue_family_index++;
    }
    device_config->queue_family_index = queue_family_index;
    return success;
};

bool vulkan_renderer::find_suitable_memory_properties(VkSystem *vksystem,VkMemoryPropertyFlags optimal_memory_type,uint32_t memory_type_bit_requirements){

   create_struct(device_memory_info,VkPhysicalDeviceMemoryProperties);
   vkGetPhysicalDeviceMemoryProperties(vksystem->physical_device.device ,&device_memory_info);
  
  
  for(uint32_t i = 0 ; i < device_memory_info.memoryTypeCount ; i++){
   const uint32_t memory_type_mask = (1 << i);
   bool is_memory_found = memory_type_bit_requirements & memory_type_mask;

   const VkMemoryPropertyFlags properties = device_memory_info.memoryTypes[i].propertyFlags;
   const bool is_properties_found = (properties & optimal_memory_type) == optimal_memory_type;



    if(is_properties_found & is_memory_found){
       vksystem->physical_device.memory_type_index = i;
       return true;
    }
  }
  return false;
};

bool vulkan_renderer::get_memory_requirements(VkSystem *vksystem){
    create_struct(image_memory_info, VkMemoryRequirements); // size //alignments //memoryTypeBits
    vkGetImageMemoryRequirements(vksystem->virtual_device,vksystem->render_pool.frame_buffer[0].image, &image_memory_info);

    if (!find_suitable_memory_properties(vksystem, vksystem->physical_device.optimal_memory_flags, image_memory_info.memoryTypeBits)) {
        std::cout << "[MEMORY] Device doesn't support the required memory flags Retrying..." << std::endl;
        if (!find_suitable_memory_properties(vksystem, vksystem->physical_device.fallback_memory_type, image_memory_info.memoryTypeBits)) {
            std::cout << "[MEMORY] Device support memory type required for working fallbacking to it " << std::endl;
              return false;
        } else {
            std::cout << "[MEMORY] Device support memory type required for working fallbacking to it " << std::endl;
            vksystem->render_pool.image_memory_size = image_memory_info.size;
            vksystem->render_pool.image_memory_alignment = image_memory_info.alignment;
         }
    }


  return true;
};
