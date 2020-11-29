#include "Renderer.h"

void Renderer::init(std::shared_ptr<Window> window)
{
	_window = window;
	createInstance();
	setupDebugMessenger();
}

Renderer::~Renderer()
{
	destroyDebugMessenger();
	vkDestroyInstance(_vkInstance, nullptr);
}

void Renderer::createInstance()
{
	ASSERT(_window, "_window pointer should not be nullptr");

	if (enableValidationLayers && !checkValidationLayerSupport())
	{
		ERROR("Validation layers requested but not available.");
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName = "cowboy-engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "cowboy-engine";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.flags = 0;
	createInfo.pApplicationInfo = &appInfo;
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<std::uint32_t>(vkValidationLayers.size());
		createInfo.ppEnabledLayerNames = vkValidationLayers.data();
		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;
		createInfo.pNext = nullptr;
	}
	std::vector<const char*> vkExtensions = getVkRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<std::uint32_t>(vkExtensions.size());
	createInfo.ppEnabledExtensionNames = vkExtensions.data();


	if (vkCreateInstance(&createInfo, nullptr, &_vkInstance))
	{
		ERROR("Cannot create Vulkan instance.");
	}
}

std::vector<const char*> Renderer::getVkRequiredExtensions()
{
	std::pair<const char**, std::uint32_t> windowRequiredInstanceExtensions = _window->windowGetRequiredInstanceExtensions();
	std::vector<const char*> vkExtensions(windowRequiredInstanceExtensions.first, windowRequiredInstanceExtensions.first + windowRequiredInstanceExtensions.second);
	if (enableValidationLayers)
	{
		vkExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	return vkExtensions;
}

void Renderer::setupDebugMessenger()
{
	if (!enableValidationLayers)
	{
		return;
	}
	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_vkInstance, "vkCreateDebugUtilsMessengerEXT");
	if (vkCreateDebugUtilsMessenger == nullptr)
	{
		ERROR("Unable to get vkCreateDebugUtilsMessengerEXT function pointer.");
	}
	vkCreateDebugUtilsMessenger(_vkInstance, &createInfo, nullptr, &_debugMessenger);
}

void Renderer::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = vkDebugCallback;
	createInfo.pUserData = nullptr;
}

void Renderer::destroyDebugMessenger()
{
	PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_vkInstance, "vkDestroyDebugUtilsMessengerEXT");
	if (vkDestroyDebugUtilsMessenger == nullptr)
	{
		ERROR("Unable to get vkDestroyDebugUtilsMessengerEXT function pointer.");
	}
	vkDestroyDebugUtilsMessenger(_vkInstance, _debugMessenger, nullptr);
}

bool Renderer::checkValidationLayerSupport()
{
	std::uint32_t vkLayerCount;
	if (vkEnumerateInstanceLayerProperties(&vkLayerCount, nullptr))
	{
		ERROR("Cannot enumerate instance layer properties.");
	}

	std::vector<VkLayerProperties> vkAvailableLayers{ vkLayerCount };
	if (vkEnumerateInstanceLayerProperties(&vkLayerCount, vkAvailableLayers.data()))
	{
		ERROR("Cannot enumerate instance layer properties.");
	}

	// Search if all of the layers in vkValidationLayers exist in vkAvailableLayers
	for (const char* layerName : vkValidationLayers)
	{
		bool layerFound = false;
		for (const VkLayerProperties& layerProperties : vkAvailableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName))
			{
				layerFound = true;
				break;
			}
		}
		if (!layerFound)
		{
			return false;
		}
	}
	return true;
}