/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Renderer.hpp
    
    Content : Renderer aka RenderSystem of the engine

=================================================*/

#pragma once
#include "RHI2/DeviceManager.hpp"
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

        std::unique_ptr<rhi::DeviceManager> m_DeviceManager;
    };
} // namespace ata
