/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : main.cpp
    
    Content : Entry pointer of the application.

=================================================*/

#include "pch.hpp"

#include "Window/Window.hpp"
#include "Renderer/Renderer.hpp"

int main() {
    ata::Window window;
    window.Initialize();
    
    ata::Renderer renderer(window);
    renderer.Initialize();

    window.Loop();
    window.Release();
    return 0;
}
