#pragma once

#include "Window.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "SimpleRenderSystem.hpp"

// std
#include <memory>
#include <vector>

namespace DaisyEngine
{
	class Application
	{
	public:
		// --- Constants ---
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		// --- Constructors / Destructors ---
		Application();
		~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		// --- Methods ---
		void Run();

	private:
		// --- Methods ---
		void LoadGameObjects();

		// --- Variables ---
		Window _window{ WIDTH, HEIGHT, "Daisy Engine" };
		Device _device{ _window };
		Renderer _renderer{ _window, _device };

		std::vector<GameObject> _gameObjects;
	};
}