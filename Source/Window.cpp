#include "Window.hpp"
#include <stdexcept>

namespace DaisyEngine
{
	DaisyEngine::Window::Window(int width, int height, std::string name)
		: _width(width),
		_height(height),
		_name(name)
	{
		InitWindow();
	}

	DaisyEngine::Window::~Window()
	{
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface!");
		}
	}

	void Window::FramebufferResizeCallback(GLFWwindow* glfwWindow, int width, int height)
	{
		Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
		window->_framebufferResized = true;
		window->_width = width;
		window->_height = height;
	}

	void Window::InitWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		_window = glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(_window, this);
		glfwSetFramebufferSizeCallback(_window, FramebufferResizeCallback);
	}
} // namespace DaisyEngine