/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Window.hpp
    
    Content : Window of the application

=================================================*/

#pragma once

#include <GLFW/glfw3.h>

namespace ata {
    class Window {
    public:
        Window() = default;
        ~Window() { this->Release(); }

        void Release();
        void Initialize();

        void Loop();

        //inline ATA_NODISCARD GLFWwindow* getGLFWwindow() const noexcept { return p_GLFWwindow; }
        inline ATA_NODISCARD void*       getNativeHandle() const noexcept { return p_GLFWwindow; }

    private:
        GLFWwindow* p_GLFWwindow = nullptr;
    };
} // namespace ata
