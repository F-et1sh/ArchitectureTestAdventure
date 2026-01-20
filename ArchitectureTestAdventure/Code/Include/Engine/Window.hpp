/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Window.hpp
    
    Content : Window of the application

=================================================*/

#pragma once

#include <functional>

namespace ata {
    struct WindowSpecification {
        std::string Title;
        uint32_t    Width        = 1280;
        uint32_t    Height       = 720;
        bool        IsResizeable = true;
        bool        VSync        = true;

        //using EventCallbackFunc = std::function<void(Event&)>;
        //EventCallbackFunc EventCallback;
    };

    class Window {
    public:
        Window() = default;
        ~Window() { this->Release(); }

        void Release();
        void Initialize();

        void Loop();

        //inline ATA_NODISCARD GLFWwindow* getGLFWwindow() const noexcept { return p_GLFWwindow; }
        //inline ATA_NODISCARD void*       getNativeHandle() const noexcept { return static_cast<void*>(p_GLFWwindow); }

    private:
        //GLFWwindow* p_GLFWwindow = nullptr;
    };
} // namespace ata
