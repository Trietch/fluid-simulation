#pragma once

#include "ECS.h"

#include "TransformComponent.h"

class CameraComponent : public Component
{
private:
	float _yaw;
	float _pitch;
	float _speed;
	float _FOV;
	glm::vec3 _front = {1.0f, 0.0f, 0.0f};
	glm::vec3 _up = {0.0f, 1.0f, 0.0f};
	glm::mat4 _projection;

public:
	CameraComponent(float yaw, float pitch, float speed,
		float FOV)
	: Component{}
	{
		_yaw = yaw;
		_pitch = pitch;
		_speed = speed;
		_FOV = FOV;
		_projection = glm::mat4{1.0f};
		// need width and height :(
		
	}

	glm::mat4 view() const
	{
		if (entity->hasComponent<TransformComponent>())
		{
			auto position = entity->getComponent<TransformComponent>().position();
			return glm::lookAt(position, position+_front, _up);	
		}
		else
		{
			std::cout << "ERROR: CameraComponent.h : The entity does not have a TransformComponent" << std::endl;
			exit(1);
		}
	}

	glm::mat4 projection() const
	{
		return _projection;
	}

	void setProjection(int width, int height)
	{
		// Not sure about the "infinite"
		// 																		(near value
		// 																		should be variable)
		_projection = glm::infinitePerspective(glm::radians(_FOV), (float)width/(float)height, 0.1f);
	}

	void init() override
	{
		Message askGLSizeMsg {ASK_GLWIDGET_SIZE};
		entity->entityPostMessage(askGLSizeMsg);
	}

	void draw() override
	{
	}

	void update() override
	{
	}

	~CameraComponent() override
	{
	}
};