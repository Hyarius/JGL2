#pragma once

#include "jgl2_includes.h"
#include "jgl2_structures.h"

#include "GameEngine/jgl2_core.h"
#include "GameEngine/Component/jgl2_component.h"

namespace jgl
{
	class GameObject
	{
	protected:
		Core _core;

		GameObject* _parent;
		std::vector<GameObject*> _childrens;
		std::vector<Component*> _components;

		void _addChildren(GameObject* p_gameObject);
		void _removeChildren(GameObject* p_gameObject);

	public:
		GameObject(std::string p_name, GameObject* p_parent = nullptr);

		Core& core() { return (_core); }

		void setParent(GameObject* p_parent);

		void setActive(jgl::Bool p_state);
		void activate();
		void deactivate();

		template<typename TComponentType>
		TComponentType* addComponent()
		{
			TComponentType* newComponent = new TComponentType(this);
			_components.push_back(newComponent);
			return (newComponent);
		}

		template<typename TComponentType>
		TComponentType* getComponent()
		{
			auto result = std::find_if(
				_components.begin(),
				_components.end(),
				[](const Component* item) { return (dynamic_cast<const TComponentType*>(item) != nullptr); }
			);

			return (dynamic_cast<TComponentType*>(*result));
		}

		void update();
		void render();
	};
}