#pragma once

#include <engine/renderer/rhi/IGraphicsDevice.h>
#include <vulkan/vulkan.h>
#include <vector>

struct GLFWwindow;

struct QueueFamilyIndices {
    uint32_t graphicsFamily = UINT32_MAX;
    uint32_t presentFamily = UINT32_MAX;

    bool isComplete() {
        return graphicsFamily != UINT32_MAX && presentFamily != UINT32_MAX;
    }
};

class VulkanDevice : public IGraphicsDevice {
public:
    VulkanDevice();
    ~VulkanDevice() override;

    // VulkanDevice(const VulkanDevice&) = delete;
    // VulkanDevice& operator=(const VulkanDevice&) = delete;

    std::unique_ptr<IBuffer> CreateBuffer(const BufferDesc& desc) override;

    bool IsWindowOpen() const;
    void BeginFrame();
    void EndFrame();

    void run();

    void initVulkan();
    void createInstance();
    void createSurface();
    void pickPhysicalDevice();

    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    void initWindow();
    void createLogicalDevice();
    void mainLoop();
    void cleanup();

private:
    GLFWwindow* window;
    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkSurfaceKHR surface;
    VkQueue presentQueue;

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;



};