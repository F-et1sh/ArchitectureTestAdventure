/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Window.cpp
    
    Content : Window of the application

=================================================*/

#include "pch.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.h>

struct ata::Window::Impl {
    GLFWwindow* p_GLFWwindow = nullptr;
};

ata::Window::Window() {
    m_Impl = new Impl();
}

ata::Window::~Window() {
    delete m_Impl;
}

void ata::Window::Release() {
    glfwDestroyWindow(m_Impl->p_GLFWwindow);
    glfwTerminate();
}

void ata::Window::Initialize(const WindowDesc& desc) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_Impl->p_GLFWwindow = glfwCreateWindow(desc.width, desc.height, desc.title.c_str(), NULL, NULL);
    if (!m_Impl->p_GLFWwindow) {
        ata::logging::error("Failed to create GLFW window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Impl->p_GLFWwindow); // if opengl
    glfwSwapInterval(desc.vsync);
}

void ata::Window::Loop() {
    while (!glfwWindowShouldClose(m_Impl->p_GLFWwindow)) {
        glfwPollEvents();
    }
}

ATA_NODISCARD void* ata::Window::getNativeHandle() const noexcept {
    return static_cast<void*>(m_Impl->p_GLFWwindow);
}
