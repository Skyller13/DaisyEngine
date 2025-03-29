#pragma once

#include "Model.hpp"

// Libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>

namespace DaisyEngine
{
	struct Transform
	{
		glm::vec3 translation{}; // position offset
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
		glm::vec3 rotation{}; // Euler angles

		// Matrix coresponds to T * Ry * Rx * Rz * S
		// Rotation convention uses tait-bryan angles with axis order YXZ
        glm::mat4 mat4() 
        {
            const float c3 = glm::cos(rotation.z);
            const float s3 = glm::sin(rotation.z);
            const float c2 = glm::cos(rotation.x);
            const float s2 = glm::sin(rotation.x);
            const float c1 = glm::cos(rotation.y);
            const float s1 = glm::sin(rotation.y);
            return glm::mat4{
                {
                    scale.x * (c1 * c3 + s1 * s2 * s3),
                    scale.x * (c2 * s3),
                    scale.x * (c1 * s2 * s3 - c3 * s1),
                    0.0f,
                },
                {
                    scale.y * (c3 * s1 * s2 - c1 * s3),
                    scale.y * (c2 * c3),
                    scale.y * (c1 * c3 * s2 + s1 * s3),
                    0.0f,
                },
                {
                    scale.z * (c2 * s1),
                    scale.z * (-s2),
                    scale.z * (c1 * c2),
                    0.0f,
                },
                {translation.x, translation.y, translation.z, 1.0f} };
        }
	};

	class GameObject
	{
	public:
		using uniqueId = unsigned int;

		static GameObject Instantiate();

		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		uniqueId GetId() const { return _id; }

		std::shared_ptr<Model> model;
		glm::vec3 color{};
		Transform transform{};

	private:
		GameObject(uniqueId objId) : _id{ objId } {}
		uniqueId _id;
	};
} // namespace DaisyEngine