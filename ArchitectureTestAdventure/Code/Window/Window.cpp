/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Window.cpp
    
    Content : Window of the application

=================================================*/

#include "pch.hpp"
#include "Window.hpp"

void ata::Window::Release() {
    glfwDestroyWindow(p_GLFWwindow);
    glfwTerminate();
}

void ata::Window::Initialize() {
    glfwInit();
    p_GLFWwindow = glfwCreateWindow(800, 600, "ArchitectureTestAdventure", NULL, NULL);
    if (!p_GLFWwindow) {
        // TODO : Add normal logging
        std::cout << "ERROR : Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(p_GLFWwindow);
}

void ata::Window::Loop() {
    while (!glfwWindowShouldClose(p_GLFWwindow)) {
        glfwPollEvents();
    }
}
