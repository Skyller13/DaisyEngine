#include "Application.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <exception>

int main()
{
	DaisyEngine::Application application{};

	try
	{
		application.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}