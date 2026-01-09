/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Device.cpp
    
    Content : Implementation of Device. A part of Vulkan backend

=================================================*/

#include "Device.hpp"

#include "Source/Common/Resource.hpp"

#include "CommandList.hpp"
#include "Swapchain.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <unordered_set>

#include "nvrhi/validation.h"

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace rhi::vulkan {
    DefaultMessageCallback& DefaultMessageCallback::getInstance() {
        static DefaultMessageCallback instance;
        return instance;
    }

    void DefaultMessageCallback::message(nvrhi::MessageSeverity severity, const char* message_text) {
        switch (severity) {
            case nvrhi::MessageSeverity::Info:
                std::cerr << "NVRHI INFO : ";
                break;
            case nvrhi::MessageSeverity::Warning:
                std::cerr << "NVRHI WARNING : ";
                break;
            case nvrhi::MessageSeverity::Error:
                std::cerr << "NVRHI ERROR : ";
                break;
            case nvrhi::MessageSeverity::Fatal:
                std::cerr << "NVRHI FATAL : ";
                break;
        }

        std::cerr << message_text << std::endl
                  << std::endl;
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* p_create_info, const VkAllocationCallbacks* p_allocator, VkDebugUtilsMessengerEXT* p_debug_messenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, p_create_info, p_allocator, p_debug_messenger);
        }

        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks* p_allocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debug_messenger, p_allocator);
        }
    }
} // namespace rhi::vulkan

rhi::vulkan::Device::Device() {
    this->Initialize();
}

rhi::vulkan::Device::~Device() {
    DestroyDebugUtilsMessengerEXT(m_Context.instance, m_DebugMessenger, nullptr);
    vkDestroySurfaceKHR(m_Context.instance, m_Surface, nullptr);
    vkDestroyInstance(m_Context.instance, nullptr);
}

void rhi::vulkan::Device::Initialize() {
    this->CreateInstance();
    this->SetupDebugMessenger();
}

void rhi::vulkan::Device::InitializeForPresentation(void* window_handle) {
    this->CreateSurface(window_handle);
    this->PickPhysicalDevice();
    this->CreateLogicalDevice();
    this->CreateCommandPool();
    this->CreateCommandBuffers();
    this->CreateSyncObjects();
    this->CreateNVRHIDevice();
}

std::unique_ptr<rhi::CommandList> rhi::vulkan::Device::CreateCommandList() {
    auto cmd = m_NVRHIDevice->createCommandList();
    return std::make_unique<rhi::vulkan::CommandList>(cmd);
}

RHI_NODISCARD std::unique_ptr<rhi::Swapchain> rhi::vulkan::Device::CreateSwapchain() {
    return std::make_unique<rhi::vulkan::Swapchain>(*this);
}

void rhi::vulkan::Device::Submit(rhi::CommandList* cmd) {
    auto& frame  = m_Frames[m_FrameIndex];
    auto* vk_cmd = static_cast<rhi::vulkan::CommandList*>(cmd);

    nvrhi::ICommandList* lists[] = {
        vk_cmd->getNVRHICommandListHandle()
    };

    m_NVRHIDevice->executeCommandLists(
        lists,
        1,
        nvrhi::CommandQueue::Graphics);

    m_FrameIndex = (m_FrameIndex + 1) % m_Frames.size();
}

RHI_NODISCARD void* rhi::vulkan::Device::CreateBackendTexture(const rhi::TextureDesc& desc) {
    nvrhi::TextureHandle handle = m_NVRHIDevice->createTexture(rhi::to_nvrhi(desc));
    return static_cast<void*>(handle.Get());
}

void rhi::vulkan::Device::DestroyBackendTexture(void* backend_handle) {
    if (backend_handle == nullptr) {
        return;
    }

    static_cast<nvrhi::ITexture*>(backend_handle)->Release();
    backend_handle = nullptr;
}

void rhi::vulkan::Device::CreateInstance() { // TODO : Add normal logging
    if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport()) {
        std::cerr << "ERROR : Validation layers requested, but not available" << std::endl;
    }

    VkApplicationInfo app_info{};
    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName   = "Hello Triangle";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName        = "No Engine";
    app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion         = VK_API_VERSION_1_3;

    auto extensions = getRequiredExtensions();

    VkInstanceCreateInfo create_info{};
    create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo        = &app_info;
    create_info.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
    create_info.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
    if (ENABLE_VALIDATION_LAYERS) {
        create_info.enabledLayerCount   = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();

        populateDebugMessengerCreateInfo(debug_create_info);
        create_info.pNext = &debug_create_info;
    }
    else {
        create_info.enabledLayerCount = 0;
        create_info.pNext             = nullptr;
    }

    if (vkCreateInstance(&create_info, nullptr, &m_Context.instance) != VK_SUCCESS) {
        std::cerr << "ERROR : Failed to create instance" << std::endl;
    }
}

void rhi::vulkan::Device::SetupDebugMessenger() { // TODO : Add normal logging
    if (!ENABLE_VALIDATION_LAYERS) {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT create_info;
    populateDebugMessengerCreateInfo(create_info);

    VkResult result = CreateDebugUtilsMessengerEXT(m_Context.instance, &create_info, nullptr, &m_DebugMessenger);

    if (result != VK_SUCCESS) {
        std::cerr << "ERROR : Failed to set up debug messenger" << std::endl;
    }
}

void rhi::vulkan::Device::CreateSurface(void* window_handle) {
    GLFWwindow* glfw_window = static_cast<GLFWwindow*>(window_handle);

    // TODO : Add normal logging

    VkResult result = glfwCreateWindowSurface(m_Context.instance, glfw_window, nullptr, &m_Surface);
    if (result != VK_SUCCESS) {
        std::cerr << "ERROR : Surface" << std::endl;
    }

    int width  = 0;
    int height = 0;

    glfwGetFramebufferSize(glfw_window, &width, &height);

    m_SurfaceWidth  = width;
    m_SurfaceHeight = height;
}

void rhi::vulkan::Device::PickPhysicalDevice() {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(m_Context.instance, &device_count, nullptr);

    if (device_count == 0) {
        std::cerr << "ERROR : Failed to find GPUs with Vulkan support" << std::endl;
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(m_Context.instance, &device_count, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            m_Context.physical_device = device;
            m_MSAA_Samples            = getMaxUsableSampleCount();
            break;
        }
    }

    if (m_Context.physical_device == VK_NULL_HANDLE) {
        std::cerr << "ERROR : Failed to find a suitable GPU" << std::endl;
    }
}

void rhi::vulkan::Device::CreateLogicalDevice() {
    bool found = findQueueFamilies(m_Context.physical_device);

    assert(found && "Failed to create logical device");

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos{};
    std::unordered_set<uint32_t>         unique_queue_families{};

    unique_queue_families.insert(m_QueueFamilyIndices.graphics_family.value());
    unique_queue_families.insert(m_QueueFamilyIndices.present_family.value());

    if (m_QueueFamilyIndices.compute_family)
        unique_queue_families.insert(*m_QueueFamilyIndices.compute_family);

    if (m_QueueFamilyIndices.transfer_family)
        unique_queue_families.insert(*m_QueueFamilyIndices.transfer_family);

    float queue_priority = 1.0f;
    for (uint32_t queue_family : unique_queue_families) {
        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount       = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(queue_create_info);
    }

    VkPhysicalDeviceFeatures device_features{};
    device_features.samplerAnisotropy = VK_TRUE; // TODO : CHECK

    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    create_info.pQueueCreateInfos    = queue_create_infos.data();

    create_info.pEnabledFeatures = &device_features;

    create_info.enabledExtensionCount   = static_cast<uint32_t>(m_EnabledExtensions.device.size());
    create_info.ppEnabledExtensionNames = m_EnabledExtensions.device.data();

    if (ENABLE_VALIDATION_LAYERS) {
        create_info.enabledLayerCount   = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    }
    else {
        create_info.enabledLayerCount = 0;
    }

    VkResult result = vkCreateDevice(m_Context.physical_device, &create_info, nullptr, &m_Context.device);
    if (result != VK_SUCCESS) {
        std::cerr << "ERROR : Failed to create logical device" << std::endl;
    }

    vkGetDeviceQueue(m_Context.device, m_QueueFamilyIndices.graphics_family.value(), 0, &m_Context.graphics_queue);
    vkGetDeviceQueue(m_Context.device, m_QueueFamilyIndices.present_family.value(), 0, &m_Context.present_queue);
}

void rhi::vulkan::Device::CreateCommandPool() {
    VkCommandPoolCreateInfo pool_info{};
    pool_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = m_QueueFamilyIndices.graphics_family.value();

    VkResult result = vkCreateCommandPool(m_Context.device, &pool_info, nullptr, &m_CommandPool);

    if (result != VK_SUCCESS) {
        std::cerr << "ERROR : Failed to create graphics command pool" << std::endl;
    }
}

void rhi::vulkan::Device::CreateCommandBuffers() {
    m_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool        = m_CommandPool;
    alloc_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = (uint32_t) m_CommandBuffers.size();

    VkResult result = vkAllocateCommandBuffers(m_Context.device, &alloc_info, m_CommandBuffers.data());

    if (result != VK_SUCCESS) {
        std::cerr << "ERROR : Failed to allocate command buffers" << std::endl;
    }
}

void rhi::vulkan::Device::CreateSyncObjects() {
    m_Frames.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphore_info{};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info{};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        VkResult image_available_semaphore_result = vkCreateSemaphore(m_Context.device, &semaphore_info, nullptr, &m_Frames[i].image_available);
        VkResult render_finished_semaphore_result = vkCreateSemaphore(m_Context.device, &semaphore_info, nullptr, &m_Frames[i].render_finished);
        VkResult in_flight_fence_result           = vkCreateFence(m_Context.device, &fence_info, nullptr, &m_Frames[i].in_flight);

        // TODO : Add m_Frames[i].frame_complete creation

        if (image_available_semaphore_result != VK_SUCCESS ||
            render_finished_semaphore_result != VK_SUCCESS ||
            in_flight_fence_result != VK_SUCCESS) {

            std::cerr << "ERROR : Failed to create synchronization objects for a frame" << std::endl;
        }
    }
}

void rhi::vulkan::Device::CreateNVRHIDevice() {
    nvrhi::vulkan::DeviceDesc device_desc;
    device_desc.errorCB             = &DefaultMessageCallback::getInstance();
    device_desc.physicalDevice      = m_Context.physical_device;
    device_desc.device              = m_Context.device;
    device_desc.graphicsQueue       = m_Context.graphics_queue;
    device_desc.graphicsQueueIndex  = m_QueueFamilyIndices.graphics_family.value();
    device_desc.deviceExtensions    = m_EnabledExtensions.device.data();
    device_desc.numDeviceExtensions = m_EnabledExtensions.device.size();

    m_NVRHIDevice = nvrhi::vulkan::createDevice(device_desc);

    if (ENABLE_VALIDATION_LAYERS) {
        nvrhi::DeviceHandle nvrhiValidationLayer = nvrhi::validation::createValidationLayer(m_NVRHIDevice);
        m_ValidationLayer                        = nvrhiValidationLayer; // TODO : make the rest of the application go through the validation layer
    }
}

RHI_NODISCARD bool rhi::vulkan::Device::checkValidationLayerSupport() {
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    for (const char* layer_name : VALIDATION_LAYERS) {
        bool layer_found = false;

        for (const auto& layer_properties : available_layers) {
            if (strcmp(layer_name, layer_properties.layerName) == 0) {
                layer_found = true;
                break;
            }
        }

        if (!layer_found) {
            return false;
        }
    }

    return true;
}

RHI_NODISCARD std::vector<const char*> rhi::vulkan::Device::getRequiredExtensions() {
    uint32_t     glfw_extension_count = 0;
    const char** glfw_extensions      = nullptr;
    glfw_extensions                   = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

    if (ENABLE_VALIDATION_LAYERS) {
        extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void rhi::vulkan::Device::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info) {
    create_info                 = {};
    create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = DebugCallback;
}

// TODO : Add Vulkan functions debug checking
// TODO : Add logic for optional extensions
RHI_NODISCARD bool rhi::vulkan::Device::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extension_count = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

    std::unordered_set<std::string> required_extensions(m_EnabledExtensions.device.begin(), m_EnabledExtensions.device.end());

    for (const auto& extension : available_extensions) {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

RHI_NODISCARD bool rhi::vulkan::Device::isDeviceSuitable(VkPhysicalDevice device) {
    bool found = findQueueFamilies(device);

    if (!found) {
        std::cerr << "ERROR : Failed to find queue families" << std::endl;
        return false;
    }

    bool extensions_supported = checkDeviceExtensionSupport(device);

    bool swapchain_adequate = false;
    if (extensions_supported) {
        findSwapchainSupportDetails(device);
        swapchain_adequate = !m_SwapchainSupportDetails.formats.empty() && !m_SwapchainSupportDetails.present_modes.empty();
    }

    VkPhysicalDeviceFeatures supported_features{};
    vkGetPhysicalDeviceFeatures(device, &supported_features);

    return extensions_supported && swapchain_adequate && (supported_features.samplerAnisotropy != 0U); // TODO : change "supported_features.samplerAnisotropy != 0U"
}

RHI_NODISCARD bool rhi::vulkan::Device::findQueueFamilies(VkPhysicalDevice physical_device) {
    QueueFamilyIndices indices{};

    uint32_t properties_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &properties_count, nullptr);

    std::vector<VkQueueFamilyProperties> properties(properties_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &properties_count, properties.data());

    for (uint32_t i = 0; i < properties_count; i++) {
        const auto& queue_family = properties[i];

        if (!indices.graphics_family.has_value()) {
            if (queue_family.queueCount > 0 &&
                ((queue_family.queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) != 0u)) {
                indices.graphics_family = i;
            }
        }

        if (!indices.compute_family.has_value()) {
            if (queue_family.queueCount > 0 &&
                ((queue_family.queueFlags & VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT) != 0u) &&
                ((queue_family.queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) == 0u)) {
                indices.compute_family = i;
            }
        }

        if (!indices.transfer_family.has_value()) {
            if (queue_family.queueCount > 0 &&
                ((queue_family.queueFlags & VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT) != 0u) &&
                ((queue_family.queueFlags & VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT) == 0u) &&
                ((queue_family.queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) == 0u)) {
                indices.transfer_family = i;
            }
        }

        if (!indices.present_family.has_value()) {
            VkBool32 present_support = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, m_Surface, &present_support);

            if (queue_family.queueCount > 0 && (present_support != 0u)) {
                indices.present_family = i;
            }
        }
    }

    if (!indices.is_complete()) { // TODO : Add Initialize Device Parameters
        // if (!indices.graphics_family.has_value() ||
        //     (!indices.present_family.has_value() && !m_DeviceParams.headlessDevice) ||
        //     (!indices.compute_family.has_value() && m_DeviceParams.enableComputeQueue) ||
        //     (!indices.transfer_family.has_value() && m_DeviceParams.enableCopyQueue)) {
        //     return false;
        // }
        return false;
    }

    m_QueueFamilyIndices = indices;
    return true;
}

void rhi::vulkan::Device::findSwapchainSupportDetails(VkPhysicalDevice device) {
    SwapchainSupportDetails details{};

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.capabilities);

    uint32_t format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &format_count, nullptr);

    if (format_count != 0) {
        details.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &format_count, details.formats.data());
    }

    uint32_t present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &present_mode_count, nullptr);

    if (present_mode_count != 0) {
        details.present_modes.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &present_mode_count, details.present_modes.data());
    }

    m_SwapchainSupportDetails = details;
}

RHI_NODISCARD VkSampleCountFlagBits rhi::vulkan::Device::getMaxUsableSampleCount() const {
    VkPhysicalDeviceProperties physical_device_properties;
    vkGetPhysicalDeviceProperties(m_Context.physical_device, &physical_device_properties);

    VkSampleCountFlags counts = physical_device_properties.limits.framebufferColorSampleCounts & physical_device_properties.limits.framebufferDepthSampleCounts;
    if ((counts & VK_SAMPLE_COUNT_64_BIT) != 0U) {
        return VK_SAMPLE_COUNT_64_BIT;
    }
    if ((counts & VK_SAMPLE_COUNT_32_BIT) != 0U) {
        return VK_SAMPLE_COUNT_32_BIT;
    }
    if ((counts & VK_SAMPLE_COUNT_16_BIT) != 0U) {
        return VK_SAMPLE_COUNT_16_BIT;
    }
    if ((counts & VK_SAMPLE_COUNT_8_BIT) != 0U) {
        return VK_SAMPLE_COUNT_8_BIT;
    }
    if ((counts & VK_SAMPLE_COUNT_4_BIT) != 0U) {
        return VK_SAMPLE_COUNT_4_BIT;
    }
    if ((counts & VK_SAMPLE_COUNT_2_BIT) != 0U) {
        return VK_SAMPLE_COUNT_2_BIT;
    }

    return VK_SAMPLE_COUNT_1_BIT;
}

RHI_NODISCARD uint32_t rhi::vulkan::Device::findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(m_Context.physical_device, &memory_properties);

    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {
        if (((type_filter & (1 << i)) != 0U) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    assert(false && "ERROR : Failed to find suitable memory type");
}

RHI_NODISCARD VkFormat rhi::vulkan::Device::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties properties{};
        vkGetPhysicalDeviceFormatProperties(m_Context.physical_device, format, &properties);

        if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features) {
            return format;
        }
        if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    assert(false && "ERROR : Failed to find supported format");
}

RHI_NODISCARD VkFormat rhi::vulkan::Device::findDepthFormat() {
    return findSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
                               VK_IMAGE_TILING_OPTIMAL,
                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

void rhi::vulkan::Device::createImage(uint32_t              width,
                                      uint32_t              height,
                                      uint32_t              mip_levels,
                                      VkSampleCountFlagBits num_samples,
                                      VkFormat              format,
                                      VkImageTiling         tiling,
                                      VkImageUsageFlags     usage,
                                      VkMemoryPropertyFlags properties,
                                      VkImage&              image,
                                      VkDeviceMemory&       image_memory) {

    VkImageCreateInfo image_info{};
    image_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType     = VK_IMAGE_TYPE_2D;
    image_info.extent.width  = width;
    image_info.extent.height = height;
    image_info.extent.depth  = 1;
    image_info.mipLevels     = mip_levels;
    image_info.arrayLayers   = 1;
    image_info.format        = format;
    image_info.tiling        = tiling;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage         = usage;
    image_info.samples       = num_samples;
    image_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

    VkResult result = vkCreateImage(m_Context.device, &image_info, nullptr, &image);

    if (result != VK_SUCCESS) {
        std::cerr << "ERROR : Failed to create image" << std::endl;
    }

    VkMemoryRequirements memory_requirements;
    vkGetImageMemoryRequirements(m_Context.device, image, &memory_requirements);

    VkMemoryAllocateInfo allocate_info{};
    allocate_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocate_info.allocationSize  = memory_requirements.size;
    allocate_info.memoryTypeIndex = this->findMemoryType(memory_requirements.memoryTypeBits, properties);

    result = vkAllocateMemory(m_Context.device, &allocate_info, nullptr, &image_memory);

    if (result != VK_SUCCESS) {
        std::cerr << "ERROR : Failed to allocate image memory" << std::endl;
    }

    vkBindImageMemory(m_Context.device, image, image_memory, 0);
}

void rhi::vulkan::Device::createImageView(VkImage            image,
                                          VkFormat           format,
                                          VkImageAspectFlags aspect_flags,
                                          uint32_t           mip_levels,
                                          VkImageView&       image_view) {
    VkImageViewCreateInfo view_info{};
    view_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image                           = image;
    view_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format                          = format;
    view_info.subresourceRange.aspectMask     = aspect_flags;
    view_info.subresourceRange.baseMipLevel   = 0;
    view_info.subresourceRange.levelCount     = mip_levels;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount     = 1;

    VkResult result = vkCreateImageView(m_Context.device, &view_info, nullptr, &image_view);

    if (result != VK_SUCCESS) {
        std::cerr << "ERROR : Failed to create image view" << std::endl;
    }
}

VkBool32 VKAPI_CALL rhi::vulkan::Device::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                                                       VkDebugUtilsMessageTypeFlagsEXT             message_types,
                                                       const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                                                       void*                                       user_data) {
    std::cerr << "Validation layer. ";

    if ((message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) != 0) {
        std::cerr << "VERBOSE : ";
    }
    if ((message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) != 0) {
        std::cerr << "INFO : ";
    }
    if ((message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) != 0) {
        std::cerr << "WARNING : ";
    }
    if ((message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) != 0) {
        std::cerr << "ERROR : ";
    }

    std::cerr << callback_data->pMessage << std::endl
              << std::endl;

    return VK_FALSE;
}
