#include "GameObject.hpp"

namespace DaisyEngine
{
	GameObject GameObject::Instantiate()
	{
		static uniqueId currentId = 0;
		return GameObject{ currentId++ };
	}
} // namespace DaisyEngine