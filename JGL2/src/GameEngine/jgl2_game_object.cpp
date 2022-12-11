#include "GameEngine/jgl2_game_object.h"

void GameObject::_addChildren(GameObject* p_gameObject)
{
	_childrens.push_back(p_gameObject);
}
void GameObject::_removeChildren(GameObject* p_gameObject)
{
	auto tmp = std::find(_childrens.begin(), _childrens.end(), p_gameObject);

	if (tmp != _childrens.end())
		_childrens.erase(tmp);
}

GameObject::GameObject(std::string p_name, GameObject* p_parent) :
	_core(p_name),
	_parent(nullptr)
{
	setParent(p_parent);
}

void GameObject::setParent(GameObject* p_parent)
{
	if (_parent != nullptr)
		_parent->_removeChildren(this);
	_parent = p_parent;
	if (_parent != nullptr)
		_parent->_addChildren(this);
}

void GameObject::setActive(jgl::Bool p_state)
{
	_core.isActive = p_state;
}

void GameObject::activate()
{
	setActive(true);
}

void GameObject::deactivate()
{
	setActive(false);
}

void GameObject::update()
{
	for (jgl::Size_t i = 0; i < _components.size(); i++)
	{
		_components[i]->_onUpdate();
	}

	for (jgl::Size_t i = 0; i < _childrens.size(); i++)
	{
		_childrens[i]->update();
	}
}

void GameObject::render()
{
	for (jgl::Size_t i = 0; i < _components.size(); i++)
	{
		_components[i]->_onRender();
	}

	for (jgl::Size_t i = 0; i < _childrens.size(); i++)
	{
		_childrens[i]->render();
	}
}