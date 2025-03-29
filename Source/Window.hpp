#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace DaisyEngine
{
	class Window
	{
	public:
		Window(int width, int height, std::string name);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		inline bool ShouldClose() const { return glfwWindowShouldClose(_window); }
		inline bool WasWindowResized() const { return _framebufferResized; }
		inline VkExtent2D GetExtent() const { return { static_cast<uint32_t>(_width), static_cast<uint32_t>(_height) }; }

		void ResetWindowResizedFlag() { _framebufferResized = false; }
		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		// ------- Methods -------
		static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

		// ------- Variables -------
		int _width;
		int _height;
		bool _framebufferResized = false;

		std::string _name;
		GLFWwindow* _window;

		// ------- Methods -------
		void InitWindow();
	};
}