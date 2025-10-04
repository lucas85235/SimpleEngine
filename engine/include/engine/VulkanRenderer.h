#pragma once
#include <engine/Renderer.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

namespace se {

class VulkanRenderer : public se::Renderer {
  public:
    void Initialize(se::Window& window) override;
    void Render(float delta_time) override;
    void Cleanup() override;
    void OnResize(int width, int height) override;

  private:
    se::Window* currentWindow = nullptr;
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    size_t currentFrame = 0;
    const int MAX_FRAMES_IN_FLIGHT = 2;

    inline void checkVk(VkResult result, const char* msg) {
        if (result != VK_SUCCESS) {
            throw std::runtime_error(std::string(msg) + " (VkResult=" + std::to_string(result) + ")");
        }
    }

    void initWindow();
    void initVulkan();

    void createInstance();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createCommandPool();
    void createCommandBuffers();
    void createSyncObjects();
    void drawFrame();
};

} // namespace se
