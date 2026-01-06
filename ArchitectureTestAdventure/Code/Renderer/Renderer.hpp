/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Renderer.hpp
    
    Content : Renderer aka RenderSystem of the engine

=================================================*/

#pragma once
#include "RHI/RHI.hpp"
#include "Window/Window.hpp"

namespace ata {
    class Renderer {
    public:
        Renderer(Window& window) : m_Window(window) {}
        ~Renderer() { this->Release(); }

        void Release();
        void Initialize();

        void TestLoop();

    private:
        Window& m_Window;

        std::unique_ptr<rhi::Device>          m_Device{};
        std::unique_ptr<rhi::Swapchain>       m_Swapchain{};
        std::unique_ptr<rhi::ResourceManager> m_ResourceManager{};
    };
} // namespace ata
