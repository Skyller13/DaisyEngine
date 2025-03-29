#pragma once

// Includes
#include "Device.hpp"

// Libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// Std
#include <vector>

namespace DaisyEngine
{
	class Model
	{
	public:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		};

		// --- Constructor & Destructor --- //
		Model(Device& device, const std::vector<Vertex>& vertices);
		~Model();

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

	private:
		// --- Methods --- //
		void CreateVertexBuffers(const std::vector<Vertex>& vertices);

		// --- Variables --- //
		Device& _device;
		VkBuffer _vertexBuffer;
		VkDeviceMemory _vertexBufferMemory;
		uint32_t _vertexCount;
	};
}