/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Window.hpp
    
    Content : Window of the application

=================================================*/

#pragma once

#include "Core/attributes.hpp"

#include <string>
#include <functional>

namespace ata {
    struct WindowDesc {
        std::string title;
        uint32_t    width  = 1280;
        uint32_t    height = 720;
        int         vsync  = true;

        //using EventCallbackFunc = std::function<void(Event&)>;
        //EventCallbackFunc EventCallback;

        WindowDesc()  = default;
        ~WindowDesc() = default;
    };

    class Window {
    public:
        Window();
        ~Window();

        void Release();
        void Initialize(const WindowDesc& desc);

        void Loop();

        inline ATA_NODISCARD void* getNativeHandle() const noexcept;

    private:
        struct Impl;
        Impl* m_Impl = nullptr;
    };
} // namespace ata
