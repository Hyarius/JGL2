#pragma once

#include "GameEngine/jgl2_game_object.h"
#include "GameEngine/jgl2_camera.h"

class Scene
{
public:
	static inline Scene* activeScene = nullptr;
private:
	Camera* _mainCamera;

	std::vector<GameObject*> _objects;

public:
	Scene() :
		_mainCamera(nullptr)
	{

	}

	void setMainCamera(Camera* p_mainCamera) { _mainCamera = p_mainCamera; }
	Camera* mainCamera() { return (_mainCamera); }

	void addGameObject(GameObject* p_gameObject)
	{
		_objects.push_back(p_gameObject);
	}
	void removeGameObject(GameObject* p_gameObject)
	{
		auto tmp = std::find(_objects.begin(), _objects.end(), p_gameObject);

		if (tmp != _objects.end())
			_objects.erase(tmp);
	}

	void activate()
	{
		activeScene = this;
	}

	void update()
	{
		for (jgl::Size_t i = 0; i < _objects.size(); i++)
		{
			_objects[i]->update();
		}
	}

	void render()
	{
		activate();

		for (jgl::Size_t i = 0; i < _objects.size(); i++)
		{
			_objects[i]->render();
		}
	}
};