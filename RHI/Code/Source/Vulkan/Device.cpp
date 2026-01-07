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

namespace rhi::vulkan {
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
    this->CreateInstance();
    this->SetupDebugMessenger();
}

rhi::vulkan::Device::~Device() {
    DestroyDebugUtilsMessengerEXT(m_Context.instance, m_DebugMessenger, nullptr);
    vkDestroySurfaceKHR(m_Context.instance, m_Surface, nullptr);
    vkDestroyInstance(m_Context.instance, nullptr);
}

void rhi::vulkan::Device::InitializeForPresentation(void* window_handle) {
    this->CreateSurface(window_handle);
    this->PickPhysicalDevice();
    this->CreateLogicalDevice();
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

    m_SurfaceWidth = width;
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

bool rhi::vulkan::Device::checkValidationLayerSupport() {
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

std::vector<const char*> rhi::vulkan::Device::getRequiredExtensions() {
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
bool rhi::vulkan::Device::checkDeviceExtensionSupport(VkPhysicalDevice device) {
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

bool rhi::vulkan::Device::isDeviceSuitable(VkPhysicalDevice device) {
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

bool rhi::vulkan::Device::findQueueFamilies(VkPhysicalDevice physical_device) {
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

VkSampleCountFlagBits rhi::vulkan::Device::getMaxUsableSampleCount() const {
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

VkImageView rhi::vulkan::Device::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspect_flags, uint32_t mip_levels) {
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

    VkImageView image_view = nullptr;
    if (vkCreateImageView(m_Context.device, &view_info, nullptr, &image_view) != VK_SUCCESS) {
        std::cerr << "ERROR : Failed to create image view" << std::endl;
    }

    return image_view;
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
