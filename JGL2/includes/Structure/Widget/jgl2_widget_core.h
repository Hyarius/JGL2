#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "jgl2_widget_parenting_manager.h"
#include "jgl2_basic_functions.h"
#include "structure/jgl2_vector2.h"
#include "structure/jgl2_viewport.h"

namespace jgl
{
	class WidgetCore
	{
		friend class WidgetManager;
		friend class WidgetParentingManager;

	private:
		WidgetParentingManager _parentingManager;
		
		jgl::Float _depth;
		jgl::Vector2Int _anchor;
		jgl::Vector2Int _size;

		Viewport _viewport;

		std::string _name = "";
		jgl::Bool _isActive = false;
		jgl::Bool _geometryEdited = false;

		virtual jgl::Bool _onUpdate() = 0;
		virtual void _onGeometryChange() = 0;
		virtual void _onRender() = 0;

		void _render();

		jgl::Bool _update();

	public:
		WidgetCore(std::string p_name);
		~WidgetCore();

		void setDepth(jgl::Float p_depth);

		const WidgetCore* parent() const { return (_parentingManager.parent()); }

		const jgl::Vector2Int& anchor() const { return (_anchor); }
		const jgl::Vector2Int& size() const { return (_size); }
		const jgl::Vector2Int& usableAnchor() const { return (_viewport.anchorOffset()); }
		const jgl::Vector2Int& usableSize() const { return (_size - _viewport.sizeOffset()); }

		const jgl::Float& depth() const { return (_depth); }
		const Viewport* viewport() const { return (&_viewport); }
		Viewport* viewport() { return (&_viewport); }

		template<typename TWidgetType, typename... Args>
		TWidgetType* addChildren(std::string p_name, Args&&... p_args)
		{
			TWidgetType* result = new TWidgetType(p_name, std::forward<Args>(p_args)...);
			result->_parentingManager.setParent(result, this);
			result->_depth = this->depth() + 1;
			return (result);
		}

		template<typename TWidgetType>
		TWidgetType* getChildren(std::string p_name = "")
		{
			for (auto children : _parentingManager.childrens())
			{
				TWidgetType* castedChildren = dynamic_cast<TWidgetType*>(children);
				if (castedChildren != nullptr && (p_name == "" || p_name == castedChildren->name()))
					return (castedChildren);
			}
			return (nullptr);
		}

		template<typename TWidgetType>
		std::vector<TWidgetType*> getChildrenList()
		{
			std::vector<TWidgetType*> result;

			for (auto children : _parentingManager.childrens())
			{
				TWidgetType* castedChildren = dynamic_cast<TWidgetType*>(children);
				if (castedChildren != nullptr)
					result.push_back(castedChildren);
			}
			return (result);
		}

		void activate();
		void deactivate();
		void setActivationStatus(jgl::Bool p_state);

		void setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size);

		const std::string& name() const { return (_name); }
	};
}