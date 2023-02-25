#pragma once

#include "jgl2_widget_core.h"
#include "Structure/Widget/Component/jgl2_box_component.h"

namespace jgl
{
	namespace Widget
	{
		class Frame : public Abstract::Widget::Core
		{
		public:
			static inline jgl::Widget::Component::Box::DefaultValues boxDefaultValue = {
				jgl::Color(120, 120, 120),
				jgl::Color(150, 150, 150),
				jgl::Vector2Int(5, 5)
			};
		private:
			jgl::Widget::Component::Box _box;

			jgl::Bool _onUpdate()
			{
				return (false);
			}
			void _onRender()
			{
				_box.render();
			}
			void _onGeometryChange()
			{
				_box.setGeometry(anchor(), size());
				_box.setDepth(depth());
			}

		public:
			Frame(std::string p_name) : Abstract::Widget::Core(p_name)
			{
				_box.setDefaultValues(boxDefaultValue);
				viewport()->setOffset(5, 10);
			}

			jgl::Widget::Component::Box& box() { return (_box); }
			const jgl::Widget::Component::Box& box() const { return (_box); }
		};
	}
}