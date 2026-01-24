/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : main.cpp
    
    Content : Entry point of the application

=================================================*/

#include "Engine/Window.hpp"

int main() {
    ata::WindowDesc desc{};
    desc.title  = "Architecture Test Adventure";
    desc.width  = 1920;
    desc.height = 1080;
    desc.vsync  = 1;

    ata::Window window;
    window.Initialize(desc);
    window.Loop();
    window.Release();

    return 0;
}
