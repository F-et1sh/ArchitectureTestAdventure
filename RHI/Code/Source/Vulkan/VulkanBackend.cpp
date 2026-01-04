/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : VulkanBackend.cpp
    
    Content : Implementation of Vulkan classes.
        Based on Donut, MIT License : ( https://github.com/NVIDIA-RTX/Donut )

=================================================*/

#include "VulkanBackend.hpp"

#include "Source/Common/Resource.hpp"

#include <unordered_set>
#include <string>
#include <queue>

struct rhi::vulkan::Device::Impl {
    struct VulkanExtensionSet {
        std::unordered_set<std::string> instance;
        std::unordered_set<std::string> layers;
        std::unordered_set<std::string> device;
    };

    // minimal set of required extensions
    VulkanExtensionSet m_EnabledExtensions = {
        // instance
        {
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME },
        // layers
        {},
        // device
        {
            VK_KHR_MAINTENANCE1_EXTENSION_NAME },
    };

    // optional extensions
    VulkanExtensionSet m_OptionalExtensions = {
        // instance
        {
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
            VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME,
        },
        // layers
        {},
        // device
        {
            VK_EXT_DEBUG_MARKER_EXTENSION_NAME,
            VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
            VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
            VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME,
            VK_KHR_MAINTENANCE_4_EXTENSION_NAME,
            VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
            VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
            VK_NV_MESH_SHADER_EXTENSION_NAME,
            VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME,
#if RHI_WITH_AFTERMATH
            VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME,
            VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME,
#endif
        },
    };

    std::unordered_set<std::string> m_RayTracingExtensions = {
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
        VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
        VK_KHR_RAY_QUERY_EXTENSION_NAME,
        VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
        VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME
    };

    std::string m_RendererString;

    vk::DebugReportCallbackEXT m_DebugReportCallback;

    VulkanContext m_Context;

    int m_GraphicsQueueFamily = -1;
    int m_ComputeQueueFamily  = -1;
    int m_TransferQueueFamily = -1;
    int m_PresentQueueFamily  = -1;

    nvrhi::vulkan::DeviceHandle m_NVRHIDevice;
    nvrhi::DeviceHandle         m_ValidationLayer;

    std::vector<FrameSync> m_Frames;
    uint32_t               m_FrameIndex;

    bool m_BufferDeviceAddressSupported = false;

#if VK_HEADER_VERSION >= 301
    typedef vk::detail::DynamicLoader VulkanDynamicLoader;
#else
    typedef vk::DynamicLoader VulkanDynamicLoader;
#endif

    std::unique_ptr<VulkanDynamicLoader> m_DynamicLoader;
};

rhi::vulkan::Device::Device() {
    m_Impl = new Impl();
}

rhi::vulkan::Device::~Device() {
    delete m_Impl;
}

std::unique_ptr<rhi::CommandList> rhi::vulkan::Device::CreateCommandList() {
    auto cmd = m_Impl->m_NVRHIDevice->createCommandList();
    return std::make_unique<rhi::vulkan::CommandList>(cmd);
}

RHI_NODISCARD std::unique_ptr<rhi::Swapchain> rhi::vulkan::Device::CreateSwapchain() {
    return std::make_unique<rhi::vulkan::Swapchain>(this);
}

void rhi::vulkan::Device::Submit(rhi::CommandList* cmd) {
    auto& frame = m_Impl->m_Frames[m_Impl->m_FrameIndex];

    //ICommandList* const* pCommandLists, size_t numCommandLists, CommandQueue executionQueue = CommandQueue::Graphics

    //static_cast<rhi::vulkan::CommandList*>(cmd)

    nvrhi::SubmitInfo submit{};
    submit.commandLists = { static_cast<rhi::vulkan::CommandList*>(cmd)->getHandle() };

    submit.waitSemaphores.push_back({
        nvrhi::CommandQueue::Graphics,
        frame.image_available,
        0
    });

    submit.signalSemaphores.push_back(frame.render_finished);
    submit.signalFence = frame.in_flight;

    m_Impl->m_NVRHIDevice->executeCommandLists(submit);

    m_Impl->m_FrameIndex = (m_Impl->m_FrameIndex + 1) % m_Impl->m_Frames.size();
}

RHI_NODISCARD void* rhi::vulkan::Device::CreateBackendTexture(const rhi::TextureDesc& desc) {
    nvrhi::TextureHandle handle = m_Impl->m_NVRHIDevice->createTexture(rhi::to_nvrhi(desc));
    return static_cast<void*>(handle.Get());
}

void rhi::vulkan::Device::DestroyBackendTexture(void* backend_handle) {
    if (!backend_handle) return;

    static_cast<nvrhi::ITexture*>(backend_handle)->Release();
    backend_handle = nullptr;
}

void rhi::vulkan::CommandList::BeginFrame() {
    m_NVRHICommandList->open();
}

void rhi::vulkan::CommandList::EndFrame() {
    m_NVRHICommandList->close();
}

void rhi::vulkan::CommandList::setPipeline(const Pipeline* pipeline) {
}

void rhi::vulkan::CommandList::setVertexBuffer(const Buffer* buffer) {
}

void rhi::vulkan::CommandList::setIndexBuffer(const Buffer* buffer) {
}

void rhi::vulkan::CommandList::setRenderTarget(TextureHandle handle) {
}

void rhi::vulkan::CommandList::DrawIndexed(uint32_t instance_count, uint32_t first_index, uint32_t first_instance, uint32_t first_vertex, uint32_t vertex_count) {
    nvrhi::DrawArguments args{};
    args.setInstanceCount(instance_count);
    args.setStartIndexLocation(first_index);
    args.setStartInstanceLocation(first_instance);
    args.setStartVertexLocation(first_vertex);
    args.setVertexCount(vertex_count);

    m_NVRHICommandList->drawIndexed(args);
}

struct rhi::vulkan::Swapchain::Impl {
    rhi::vulkan::Device* p_Device;

    vk::SurfaceKHR m_WindowSurface;

    VkSurfaceFormatKHR m_SwapchainFormat;
    VkSwapchainKHR     m_Swapchain;
    bool               m_SwapchainMutableFormatSupported = false;

    struct SwapChainImage {
        VkImage              image;
        nvrhi::TextureHandle nvrhi_handle;
        rhi::TextureHandle   rhi_handle;
    };

    std::vector<SwapChainImage> m_Images;
    uint32_t                    m_Index = uint32_t(-1);
};

rhi::vulkan::Swapchain::Swapchain(rhi::vulkan::Device* device) {
    m_Impl           = new Impl();
    m_Impl->p_Device = device;
}

rhi::vulkan::Swapchain::~Swapchain() {
    delete m_Impl;
}

RHI_NODISCARD rhi::TextureHandle rhi::vulkan::Swapchain::Acquire() {
    auto& device_impl = m_Impl->p_Device->m_Impl;
    auto& device      = device_impl->m_Context.device;

    auto& frame = device_impl->m_Frames[device_impl->m_FrameIndex];

    vkWaitForFences(device, 1, &frame.in_flight, VK_TRUE, std::numeric_limits<uint64_t>::max());
    vkResetFences(device, 1, &frame.in_flight);

    VkResult result = vkAcquireNextImageKHR(
        device,
        m_Impl->m_Swapchain,
        std::numeric_limits<uint64_t>::max(),
        frame.image_available,
        VK_NULL_HANDLE,
        &m_Impl->m_Index);

    assert(result == VK_SUCCESS);

    device_impl->m_NVRHIDevice->queueWaitForSemaphore(
        nvrhi::CommandQueue::Graphics,
        frame.image_available,
        0);

    return m_Impl->m_Images[m_Impl->m_Index].rhi_handle;
}

void rhi::vulkan::Swapchain::Present() {
    auto& device_impl = m_Impl->p_Device->m_Impl;
    auto& device      = device_impl->m_Context.device;

    auto& frame = device_impl->m_Frames[device_impl->m_FrameIndex];

    VkPresentInfoKHR info{};
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores    = &frame.render_finished;
    info.swapchainCount     = 1;
    info.pSwapchains        = &m_Impl->m_Swapchain;
    info.pImageIndices      = &m_Impl->m_Index;

    vkQueuePresentKHR(device_impl->m_Context.present_queue, &info);
}

void rhi::vulkan::Swapchain::Resize(uint32_t width, uint32_t height) {
}

RHI_NODISCARD uint32_t rhi::vulkan::Swapchain::getWidth() const {
    return 0;
}

RHI_NODISCARD uint32_t rhi::vulkan::Swapchain::getHeight() const {
    return 0;
}
