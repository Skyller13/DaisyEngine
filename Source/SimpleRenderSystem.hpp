#pragma once

#include "Pipeline.hpp"
#include "Device.hpp"
#include "GameObject.hpp"

// std
#include <memory>
#include <vector>

namespace DaisyEngine
{
	class SimpleRenderSystem
	{
	public:
		// --- Constructors / Destructors ---
		SimpleRenderSystem(Device& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects);

	private:
		// --- Methods ---
		void CreatePipelineLayout();
		void CreatePipeline(VkRenderPass renderPass);

		// --- Variables ---
		Device& _device;

		std::unique_ptr<Pipeline> _pipeline;
		VkPipelineLayout _pipelineLayout;
	};
} // namespace DaisyEngine