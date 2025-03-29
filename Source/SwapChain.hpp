#pragma once

#include "Device.hpp"

// Vulkan includes
#include <vulkan/vulkan.h>

// Std lib headers
#include <memory>
#include <string>
#include <vector>

namespace DaisyEngine
{
	class SwapChain
	{
	public:
		// --- Constants ---
		static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

		// --- Constructors ---
		SwapChain(Device& device, VkExtent2D windowExtent);
		SwapChain(Device& device, VkExtent2D windowExtent, std::shared_ptr<SwapChain> previous);
		~SwapChain();

		SwapChain(const SwapChain&) = delete;
		SwapChain& operator=(const SwapChain&) = delete;

		// --- Methods ---
		VkFramebuffer GetFrameBuffer(int index) { return _swapChainFramebuffers[index]; }
		VkRenderPass GetRenderPass() { return _renderPass; }
		VkImageView GetImageView(int index) { return _swapChainImageViews[index]; }
		size_t GetImageCount() { return _swapChainImages.size(); }
		VkFormat GetSwapChainImageFormat() { return _swapChainImageFormat; }
		VkExtent2D GetSwapChainExtent() { return _swapChainExtent; }
		uint32_t GetWidth() { return _swapChainExtent.width; }
		uint32_t GetHeight() { return _swapChainExtent.height; }

		float ExtentAspectRatio()
		{
			return static_cast<float>(_swapChainExtent.width) / static_cast<float>(_swapChainExtent.height);
		}

		VkFormat FindDepthFormat();

		VkResult AcquireNextImage(uint32_t* imageIndex);
		VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

		bool CompareSwapFormat(const SwapChain& swapChain) const
		{
			return _swapChainDepthFormat == swapChain._swapChainDepthFormat
				&& _swapChainImageFormat == swapChain._swapChainImageFormat;
		}

	private:
		// --- Methods ---
		void Init();
		void CreateSwapChain();
		void CreateImageViews();
		void CreateDepthResources();
		void CreateRenderPass();
		void CreateFramebuffers();
		void CreateSyncObjects();

		// Helper Functions
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		// --- Variables ---
		VkFormat _swapChainImageFormat;
		VkFormat _swapChainDepthFormat;
		VkExtent2D _swapChainExtent;

		std::vector<VkFramebuffer> _swapChainFramebuffers;
		VkRenderPass _renderPass;

		std::vector<VkImage> _depthImages;
		std::vector<VkDeviceMemory> _depthImageMemories;
		std::vector<VkImageView> _depthImageViews;
		std::vector<VkImage> _swapChainImages;
		std::vector<VkImageView> _swapChainImageViews;

		Device& _device;
		VkExtent2D _windowExtent;

		VkSwapchainKHR _swapChain;
		std::shared_ptr<SwapChain> _oldSwapChain;

		std::vector<VkSemaphore> _imageAvailableSemaphores;
		std::vector<VkSemaphore> _renderFinishedSemaphores;
		std::vector<VkFence> _inFlightFences;
		std::vector<VkFence> _imagesInFlight;
		size_t _currentFrame = 0;
	};
} // namespace DaisyEngine