#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "Device.hpp"

namespace DaisyEngine
{
	struct PipelineConfigInfo
	{
		PipelineConfigInfo() = default;

		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class Pipeline
	{
	public:
		Pipeline(
			Device& device,
			const std::string& vertexFilepath,
			const std::string& fragFilepath,
			const PipelineConfigInfo& configInfo);
		~Pipeline();

		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

	private:
		// --- Methods ---
		static std::vector<char> ReadFile(const std::string& filepath);

		void CreateGraphicPipeline(const std::string& vertexFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
		void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		// --- Variables ---
		Device& _device;
		VkPipeline _graphicsPipeline;
		VkShaderModule _vertexShaderModule;
		VkShaderModule _fragShaderModule;
	};
}