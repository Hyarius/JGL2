#pragma once

#include "GameEngine/jgl2_game_object.h"

class Camera : public GameObject
{
public:
	enum class Mode
	{
		Orthographic,
		Perspective
	};
	struct PerspectiveData
	{
		jgl::Vector2Int viewportSize;
	};
	struct OrthographicData
	{
	};
private:
	jgl::Bool _computed = false;
	Mode _mode = Mode::Orthographic;
	glm::mat4 _mvp;
	jgl::Float _fov = 45.0f;
	jgl::Vector2 _viewportSize = jgl::Vector2(20, 20);
	jgl::Float _nearPlane = 0.0f;
	jgl::Float _farPlane = 100.0f;

	void _computeMVP()
	{
		glm::mat4 Projection;

		if (_mode == Mode::Perspective)
			Projection = glm::perspective(
				glm::radians(_fov),
				_viewportSize.x() / _viewportSize.y(), 
				_nearPlane,
				_farPlane);
		else
			Projection = glm::ortho(
				-(_viewportSize.x() / 2), 
				_viewportSize.x() / 2,
				-(_viewportSize.y() / 2),
				_viewportSize.y() / 2,
				_nearPlane,
				_farPlane);

		glm::vec3 position = core().position;
		glm::vec3 target = jgl::Vector3(core().position + core().forward);
		glm::vec3 up = glm::vec3(0, 1, 0);

		glm::mat4 View = glm::lookAt(
			position,
			target,
			up
		);

		glm::mat4 Model = glm::mat4(1.0f);

		_mvp = Projection * View * Model;

		_computed = true;
	}

public:
	Camera(std::string p_cameraName = "Camera") : GameObject(p_cameraName, nullptr)
	{
		_computed = false;
	}

	void setMode(Mode p_mode)
	{
		_mode = p_mode;
		_computed = false;
	}
	void setFov(jgl::Float p_fov)
	{
		_fov = p_fov;
		_computed = false;
	}
	void setViewportSize(jgl::Vector2 p_size)
	{
		_viewportSize = p_size;
		_computed = false;
	}
	void setDepthValues(jgl::Float p_near, jgl::Float p_far)
	{
		_nearPlane = p_near;
		_farPlane = p_far;
		_computed = false;
	}

	void unbake()
	{
		_computed = false;
	}

	glm::mat4 mvp()
	{
		if (_computed == false)
		{
			_computeMVP();
		}
		return (_mvp);
	}
};