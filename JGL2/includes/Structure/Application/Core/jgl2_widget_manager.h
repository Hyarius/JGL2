#pragma once

#include "structure/Widget/jgl2_widget_core.h"

namespace jgl
{
	class WidgetManager
	{
	private:
		std::vector<WidgetCore*> _rootWidgets;

	public:
		WidgetManager()
		{

		}

		void setRootWidgetGeometry(jgl::Vector2 p_ratio)
		{
			for (auto rootWidget : _rootWidgets)
			{
				jgl::Vector2 newAnchor = jgl::Vector2(rootWidget->anchor().x, rootWidget->anchor().y) * p_ratio;
				jgl::Vector2 newSize = jgl::Vector2(rootWidget->size().x, rootWidget->size().y) * p_ratio;

				rootWidget->setGeometry(newAnchor.round(), newSize.round());
			}
		}

		void castUpdateMethod()
		{
			for (auto widget : _rootWidgets)
			{
				if (widget->_isActive == true)
					widget->_update();
			}
		}

		void castRenderMethod()
		{
			for (auto widget : _rootWidgets)
			{
				if (widget->_isActive == true)
					widget->_render();
			}
		}

		template<typename TWidgetType, typename... Args>
		TWidgetType* addRootWidget(std::string p_name, Args&&... p_args)
		{
			TWidgetType* result = new TWidgetType(p_name, std::forward<Args>(p_args)...);
			result->_parentingManager.setParent(result, nullptr);
			_rootWidgets.push_back(result);
			return (result);
		}

		template<typename TWidgetType>
		std::vector<TWidgetType*> getRootWidgetList()
		{
			std::vector<TWidgetType*> result;

			for (auto children : _rootWidgets)
			{
				TWidgetType* castedChildren = dynamic_cast<TWidgetType*>(children);
				if (castedChildren != nullptr)
					result.push_back(castedChildren);
			}
			return (result);
		}

		std::vector<WidgetCore*>& getAllRootWidgets()
		{
			return (_rootWidgets);
		}

		template<typename TWidgetType>
		TWidgetType* getRootWidget(std::string p_name)
		{
			for (auto children : _rootWidgets)
			{
				TWidgetType* castedChildren = dynamic_cast<TWidgetType*>(children);
				if (castedChildren != nullptr && (p_name == "" || castedChildren->name() == p_name))
					return (castedChildren);
			}
			return (nullptr);
		}
	};
}