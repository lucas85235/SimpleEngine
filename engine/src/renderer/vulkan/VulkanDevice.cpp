#include <engine/renderer/vulkan/VulkanDevice.h>
#include <engine/renderer/vulkan/VulkanBuffer.h>
#include <engine/Log.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Configuração: Habilitar as validation layers em modo Debug
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

// Lista de validation layers que queremos ativar
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

// Lista de extensões de dispositivo que precisamos (para o swap chain)
const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// --- Implementação do Construtor e Destrutor ---

VulkanDevice::VulkanDevice() {
    glfwInit();
    
    // Não usar OpenGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Exemplo Mínimo", nullptr, nullptr);
}

VulkanDevice::~VulkanDevice() {
    // cleanup();
    std::cout << "VulkanDevice destruído!" << std::endl;
}

// --- Funções do ciclo de vida ---

bool VulkanDevice::IsWindowOpen() const {
    return !glfwWindowShouldClose(window);
}

void VulkanDevice::BeginFrame() {
    glfwPollEvents();
    // TODO: A lógica real do BeginFrame em Vulkan é mais complexa.
    // Envolve adquirir uma imagem do swap chain usando um semáforo.
    // Por enquanto, isso é suficiente para manter a janela aberta.
}

void VulkanDevice::EndFrame() {
    // TODO: A lógica real do EndFrame em Vulkan é mais complexa.
    // Envolve submeter o command buffer para a fila de gráficos e
    // apresentar a imagem na tela.
}


// --- Lógica de Inicialização ---

void VulkanDevice::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void VulkanDevice::initWindow() {
    glfwInit();
    
    // Não usar OpenGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Exemplo Mínimo", nullptr, nullptr);
}

void VulkanDevice::initVulkan() {
    createInstance();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
}

void VulkanDevice::createInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Extensões necessárias para GLFW
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Falha ao criar instância Vulkan!");
    }
}

void VulkanDevice::createSurface() {
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Falha ao criar surface!");
    }
}

void VulkanDevice::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("Nenhum dispositivo com suporte ao Vulkan!");
    }
    else
    {
        SE_LOG_DEBUG("Número de dispositivos com suporte ao Vulkan: {}", deviceCount);
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Nenhum dispositivo adequado encontrado!");
    }
}

bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(device);
    return indices.isComplete();
}

QueueFamilyIndices VulkanDevice::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

void VulkanDevice::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;

    // Extensões do dispositivo (necessário para apresentação)
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    createInfo.enabledLayerCount = 0;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("Falha ao criar dispositivo lógico!");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
}

void VulkanDevice::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

void VulkanDevice::cleanup() {
    vkDestroyDevice(device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();
}

std::unique_ptr<IBuffer> VulkanDevice::CreateBuffer(const BufferDesc& desc) {
    // A implementação real é complexa e envolve a biblioteca VMA
    std::cout << "AVISO: VulkanDevice::CreateBuffer ainda é um esqueleto!" << std::endl;
    return std::make_unique<VulkanBuffer>(desc);
}

// O resto (setupDebugMessenger, etc.) pode ser implementado seguindo tutoriais padrão de Vulkan.
// A lógica acima é o esqueleto principal.
// Por brevidade, setupDebugMessenger foi omitido, mas é crucial para o desenvolvimento.