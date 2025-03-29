#pragma once

#include "Window.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"

// std
#include <memory>
#include <vector>

namespace DaisyEngine
{
	class Renderer
	{
	public:
		// --- Constructors / Destructors ---
		Renderer(Window& window, Device& device);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		inline bool IsFrameInProgress() const { return _isFrameStarted; }
		VkRenderPass GetSwapChainRenderPass() const { return _swapChain->GetRenderPass(); }
		VkCommandBuffer GetCurrentCommandBuffer() const;

		// --- Methods ---
		VkCommandBuffer BeginFrame();
		void EndFrame();
		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

		int GetFrameIndex() const;

	private:
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void RecreateSwapChain();

		// --- Variables ---
		uint32_t _currentImageIndex{ 0 };
		int _currentFrameIndex{ 0 };
		bool _isFrameStarted{ false };

		Window& _window;
		Device& _device;
		std::unique_ptr<SwapChain> _swapChain;
		std::vector<VkCommandBuffer> _commandBuffers;
	};
} // namespace DaisyEngine