#pragma once

#include "structure/Widget/jgl2_widget_core.h"

namespace jgl
{
	class WidgetManager
	{
	private:
		std::vector<WidgetCore*> _rootWidgets;

		void _applyRatioOnWidget(WidgetCore* p_widget, jgl::Vector2 p_ratio)
		{
			for (auto children : p_widget->_parentingManager.childrens())
			{
				_applyRatioOnWidget(children, p_ratio);
			}
			p_widget->setGeometry(
				jgl::Vector2(p_widget->anchor().x, p_widget->anchor().y) * p_ratio,
				jgl::Vector2(p_widget->size().x, p_widget->size().y) * p_ratio
			);
		}

		void _recalcViewportWidget(WidgetCore* p_widget, jgl::Vector2 p_ratio)
		{
			p_widget->_viewport.configure(p_widget->anchor(), p_widget->size());
			for (auto children : p_widget->_parentingManager.childrens())
			{
				_recalcViewportWidget(children, p_ratio);
			}
		}

	public:
		WidgetManager()
		{

		}

		void scaleWidgets(jgl::Vector2 p_ratio)
		{
			for (auto rootWidget : _rootWidgets)
			{
				_applyRatioOnWidget(rootWidget, p_ratio);
			}
			for (auto rootWidget : _rootWidgets)
			{
				_recalcViewportWidget(rootWidget, p_ratio);
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