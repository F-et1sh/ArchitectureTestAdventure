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
        std::string title;
        uint32_t    width         = 1280;
        uint32_t    height        = 720;
        bool        is_resizeable = true;
        bool        vsync         = true;

        //using EventCallbackFunc = std::function<void(Event&)>;
        //EventCallbackFunc EventCallback;

        WindowSpecification()  = default;
        ~WindowSpecification() = default;
    };

    class Window {
    public:
        Window();
        ~Window();

        void Release();
        void Initialize();

        void Loop();

        inline ATA_NODISCARD void* getNativeHandle() const noexcept;

    private:
        struct Impl;
        Impl* m_Impl = nullptr;
    };
} // namespace ata
