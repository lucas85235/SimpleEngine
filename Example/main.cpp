#include "app.h"
#include "opengl_renderer.h"
#include "vulkan_renderer.h"
#include <iostream>

int main() {
    try {
        App app(800, 600, "Exemplo Abstraído");
        
        // Escolha qual renderer usar
        char choice;
        std::cout << "Escolha o renderer:\n";
        std::cout << "1. OpenGL (o)\n";
        std::cout << "2. Vulkan (v)\n";
        std::cout << "Opção: ";
        std::cin >> choice;
        
        if (choice == 'o' || choice == '1') {
            app.setRenderer(std::make_unique<OpenGLRenderer>());
        } else {
            app.setRenderer(std::make_unique<VulkanRenderer>());
        }
        
        app.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
