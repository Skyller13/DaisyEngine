#include "Renderer.hpp"

#include <stdexcept>
#include <array>
#include <cassert>
#include <iostream>

namespace DaisyEngine
{
	Renderer::Renderer(Window& window, Device& device)
		: _window(window), _device(device)
	{
		RecreateSwapChain();
		CreateCommandBuffers();
	}

	Renderer::~Renderer()
	{
		FreeCommandBuffers();
	}

	VkCommandBuffer Renderer::GetCurrentCommandBuffer() const
	{
		assert(_isFrameStarted && "Cannot get command buffer when frame is not in progress.");
		return _commandBuffers[_currentFrameIndex];
	}

	VkCommandBuffer Renderer::BeginFrame()
	{
		assert(!_isFrameStarted && "Cannot call BeginFrame while frame is already in progress.");

		VkResult result = _swapChain->AcquireNextImage(&_currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS
			&& result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire next image!");
		}

		_isFrameStarted = true;

		VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

		return commandBuffer;
	}

	void Renderer::EndFrame()
	{
		assert(_isFrameStarted && "Cannot call EndFrame while frame is not in progress.");

		VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to record command buffer!");
		}

		VkResult result = _swapChain->SubmitCommandBuffers(&commandBuffer, &_currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR
			|| result == VK_SUBOPTIMAL_KHR
			|| _window.WasWindowResized())
		{
			_window.ResetWindowResizedFlag();
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to submit command buffer!");
		}

		_isFrameStarted = false;
		_currentFrameIndex = (_currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(_isFrameStarted && "Cannot begin render pass when frame is not in progress.");
		assert(commandBuffer == GetCurrentCommandBuffer(), "Can only begin render pass for command buffer that was acquired this frame.");

		// Begin Render Pass
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = _swapChain->GetRenderPass();
		renderPassInfo.framebuffer = _swapChain->GetFrameBuffer(_currentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = _swapChain->GetSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(_swapChain->GetSwapChainExtent().width);
		viewport.height = static_cast<float>(_swapChain->GetSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, _swapChain->GetSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void Renderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(_isFrameStarted && "Cannot begin render pass when frame is not in progress.");
		assert(commandBuffer == GetCurrentCommandBuffer(), "Can only begin render pass for command buffer that was acquired this frame.");

		vkCmdEndRenderPass(commandBuffer);
	}

	int Renderer::GetFrameIndex() const
	{
		assert(_isFrameStarted && "Cannot get frame index when frame is not in progress.");
		return _currentFrameIndex;
	}

	void Renderer::RecreateSwapChain()
	{
		VkExtent2D extent = _window.GetExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = _window.GetExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(_device.GetDevice());

		if (_swapChain == nullptr)
		{
			_swapChain = std::make_unique<SwapChain>(_device, extent);
		}
		else
		{
			std::shared_ptr<SwapChain> oldSwapChain = std::move(_swapChain);
			_swapChain = std::make_unique<SwapChain>(_device, extent, oldSwapChain);

			if (!oldSwapChain->CompareSwapFormat(*_swapChain.get()))
			{
				throw std::runtime_error("Swap chain image (color or depth) format has changed!");
			}
		}

		// TODO
	}

	void Renderer::CreateCommandBuffers()
	{
		size_t imageCount = _swapChain->GetImageCount();
		_commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandPool = _device.GetCommandPool();
		allocateInfo.commandBufferCount = static_cast<uint32_t>(imageCount);

		if (vkAllocateCommandBuffers(_device.GetDevice(), &allocateInfo, _commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers!");
		}
	}

	void Renderer::FreeCommandBuffers()
	{
		vkFreeCommandBuffers(_device.GetDevice(), _device.GetCommandPool(), static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
		_commandBuffers.clear();
	}
} // namespace DaisyEngine