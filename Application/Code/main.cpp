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
