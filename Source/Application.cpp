#include "Application.hpp"

// Libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// std
#include <stdexcept>
#include <array>

// ImGUI
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

namespace DaisyEngine
{
	static void check_vk_result(VkResult err)
	{
		if (err == 0)
		{
			return;
		}

		fprintf(stderr, "[Vulkan] Error : VkResult = %d\n", err);
		
		if (err < 0)
		{
			abort();
		}
	}

	Application::Application()
	{
		LoadGameObjects();
	}

	Application::~Application() {}

	void Application::Run()
	{
		SimpleRenderSystem simpleRenderSystem{ _device, _renderer.GetSwapChainRenderPass() };

		while (!_window.ShouldClose())
		{
			glfwPollEvents();

			if (VkCommandBuffer commandBuffer = _renderer.BeginFrame())
			{
				_renderer.BeginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.RenderGameObjects(commandBuffer, _gameObjects);
				_renderer.EndSwapChainRenderPass(commandBuffer);
				_renderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(_device.GetDevice());
	}

	// temporary helper function, creates a 1x1x1 cube centered at offset
	std::unique_ptr<Model> CreateCubeModel(Device& device, glm::vec3 offset)
	{
		std::vector<Model::Vertex> vertices{

			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

		};

		for (Model::Vertex& v : vertices) 
		{
			v.position += offset;
		}

		return std::make_unique<Model>(device, vertices);
	}

	void Application::LoadGameObjects()
	{
		std::shared_ptr<Model> cubeModel = CreateCubeModel(_device, { 0.f, 0.f, 0.f });
		GameObject cubeObject = GameObject::Instantiate();
		cubeObject.model = cubeModel;
		cubeObject.transform.translation = { 0.f, 0.f, 0.5f };
		cubeObject.transform.scale = { .5f, .5f, .5f};
		_gameObjects.push_back(std::move(cubeObject));
	}
} // namespace DaisyEngine