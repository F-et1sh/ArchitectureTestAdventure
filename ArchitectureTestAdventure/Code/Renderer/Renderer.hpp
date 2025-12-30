/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This source is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Renderer.hpp
    
    Content : Renderer aka RenderSystem of the engine

=================================================*/

#pragma once
#include "RHI.hpp"

namespace ata {
    class Renderer {
    public:
        Renderer()  = default;
        ~Renderer() = default;

        void Release();
        void Initialize();

    private:
        //rhi::DeviceHandle m_deviceHandle{};
    };
} // namespace ata
