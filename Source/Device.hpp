#pragma once
#include "Window.hpp"

#include <vector>

namespace DaisyEngine
{
	/// <summary>
	/// The SwapChainSupportDetails struct is used to store the capabilities of the swap chain, the supported formats, and the supported present modes.
	/// </summary>
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	/// <summary>
	/// The QueueFamilyIndices struct is used to store the indices of the graphics and present queues.
	/// </summary>
	struct QueueFamilyIndices
	{
		uint32_t graphicsFamily{};
		uint32_t presentFamily{};
		bool graphicsFamilyHasValue = false;
		bool presentFamilyHasValue = false;

		bool IsComplete()
		{
			return graphicsFamilyHasValue && presentFamilyHasValue;
		}
	};

	/// <summary>
	/// The Device class is used to create the Vulkan instance, the logical device, and the command pool.
	/// </summary>
	class Device
	{
	public:
		// --- Constants ---
#ifndef  NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif // ! NDEBUG

		// --- Constructor/ Destructor ---
		Device(Window& window);
		~Device();

		// Not copyable or movable
		Device(const Device&) = delete;
		Device& operator=(const Device&) = delete;
		Device(Device&&) = delete;
		Device& operator=(Device&&) = delete;

		inline VkCommandPool GetCommandPool() { return _commandPool; }
		inline VkDevice GetDevice() { return _device; }
		inline VkSurfaceKHR GetSurface() { return _surface; }
		inline VkQueue GetGraphicsQueue() { return _graphicsQueue; }
		inline VkQueue GetPresentQueue() { return _presentQueue; }

		inline SwapChainSupportDetails GetSwapChainSupport() { return QuerySwapChainSupport(_physicalDevice); }
		inline QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(_physicalDevice); }
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		// Buffer helper functions
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);
		void CreateImageWithInfo(const VkImageCreateInfo& imageInfo, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

		VkPhysicalDeviceProperties _properties;

	private:
		// --- Methods ---
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateCommandPool();

		// --- Helper Methods ---
		bool IsDeviceSuitable(VkPhysicalDevice device);
		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void HasGlfwRequiredInstanceExtensions();
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

		// --- Variables ---
		VkInstance _instance;
		VkDebugUtilsMessengerEXT _debugMessenger;
		VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
		Window& _window;
		VkCommandPool _commandPool;

		VkDevice _device;
		VkSurfaceKHR _surface;
		VkQueue _graphicsQueue;
		VkQueue _presentQueue;

		// --- Constants ---
		const std::vector<const char*> _validationLayers = {"VK_LAYER_KHRONOS_validation"};
		const std::vector<const char*> _deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	};
} // namespace DaisyEngine