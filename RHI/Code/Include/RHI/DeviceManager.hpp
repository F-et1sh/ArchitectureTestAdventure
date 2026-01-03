#pragma once

#include <memory>

namespace rhi {
    enum class GraphicsAPI : uint8_t {
        //D3D11,
        //D3D12,
        VK
    };

    class DeviceManager {
    public:
        DeviceManager()  = default;
        virtual ~DeviceManager() = default;

        static DeviceManager* Create(GraphicsAPI backend);

    private:
        // TODO : static DeviceManager* CreateD3D11();
        // TODO : static DeviceManager* CreateD3D12();
        static DeviceManager* CreateVK();
    };
} // namespace rhi
