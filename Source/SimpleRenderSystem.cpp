#include "SimpleRenderSystem.hpp"

// Libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>
#include <array>

namespace DaisyEngine
{
	struct SimplePushConstantData
	{
		glm::mat4 transform{ 1.0f };
		alignas(16) glm::vec3 color;
	};

	SimpleRenderSystem::SimpleRenderSystem(Device& device, VkRenderPass renderPass)
		: _device(device)
	{
		CreatePipelineLayout();
		CreatePipeline(renderPass);
	}

	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(_device.GetDevice(), _pipelineLayout, nullptr);
	}

	void SimpleRenderSystem::CreatePipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(_device.GetDevice(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout!");
		}
	}

	void SimpleRenderSystem::CreatePipeline(VkRenderPass renderPass)
	{
		assert(_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig = {};
		Pipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = _pipelineLayout;
		_pipeline = std::make_unique<Pipeline>(_device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
	}

	void SimpleRenderSystem::RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects)
	{
		_pipeline->Bind(commandBuffer);

		int i = 0;
		for (GameObject& object : gameObjects)
		{
			i += 1;
			object.transform.rotation.y = glm::mod<float>(object.transform.rotation.y + 0.01f, glm::two_pi<float>());
			object.transform.rotation.x = glm::mod<float>(object.transform.rotation.x + 0.005f, glm::two_pi<float>());

			SimplePushConstantData push{};
			push.color = object.color;
			push.transform = object.transform.mat4();

			vkCmdPushConstants(
				commandBuffer,
				_pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);

			object.model->Bind(commandBuffer);
			object.model->Draw(commandBuffer);
		}
	}
} // namespace DaisyEngine