#pragma once

class GameObject;

class Component
{
	friend class GameObject;

protected:
	GameObject* _owner = nullptr;

	virtual void _onUpdate() = 0;
	virtual void _onRender() = 0;

public:
	Component(GameObject* p_owner) :
		_owner(p_owner)
	{

	}

	GameObject* owner() { return (_owner); }
};
