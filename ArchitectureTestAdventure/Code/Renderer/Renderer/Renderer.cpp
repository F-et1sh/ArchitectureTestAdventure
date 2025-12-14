#include "pch.hpp"
#include "Renderer.hpp"

void ata::Renderer::Initialize() {
    const char* device_extensions[] = {
        "VK_KHR_acceleration_structure",
        "VK_KHR_deferred_host_operations",
        "VK_KHR_ray_tracing_pipeline",
        // list the extensions that were requested when the device was created
    };

    ata::vulkan::DeviceDesc deviceDesc;
    deviceDesc.errorCB             = g_MyMessageCallback;
    deviceDesc.physicalDevice      = vulkanPhysicalDevice;
    deviceDesc.device              = vulkanDevice;
    deviceDesc.graphicsQueue       = vulkanGraphicsQueue;
    deviceDesc.graphicsQueueIndex  = vulkanGraphicsQueueFamily;
    deviceDesc.deviceExtensions    = deviceExtensions;
    deviceDesc.numDeviceExtensions = std::size(deviceExtensions);

    ata::DeviceHandle ataDevice = ata::vulkan::createDevice(deviceDesc);

    auto textureDesc = ata::TextureDesc()
                           .setDimension(ata::TextureDimension::Texture2D)
                           .setFormat(ata::Format::RGBA8_UNORM)
                           .setWidth(swapChainWidth)
                           .setHeight(swapChainHeight)
                           .setIsRenderTarget(true)
                           .setDebugName("Swap Chain Image");

    // In this line, <type> depends on the GAPI and should be one of: D3D11_Resource, D3D12_Resource, VK_Image.
    ata::TextureHandle swapChainTexture = ataDevice->createHandleForNativeTexture(ata::ObjectTypes::VK_Image, nativeTextureOrImage, textureDesc);
}
