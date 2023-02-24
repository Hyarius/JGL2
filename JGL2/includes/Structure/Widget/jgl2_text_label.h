#pragma once

#include "jgl2_widget_core.h"
#include "Component\jgl2_box_component.h"
#include "Component\jgl2_label_component.h"

namespace jgl
{
	namespace Widget
	{
		class Label : public Abstract::Widget::Core
		{
		public:
			static inline jgl::Widget::Component::Box::DefaultValues boxDefaultValue;
			static inline jgl::Widget::Component::Label::DefaultValues labelDefaultValue;

		private:
			jgl::Widget::Component::Box _box;
			jgl::Widget::Component::Label _label;

			jgl::Bool _onUpdate()
			{
				return (false);
			}
			void _onRender()
			{
				_label.render();
				_box.render();
			}
			void _onGeometryChange()
			{
				_box.setGeometry(anchor(), size());
				_box.setDepth(depth());

				_label.setGeometry(_box.usableAnchor(), _box.usableSize());
				_label.setDepth(depth() + 0.5f);
			}

		public:
			Label(std::string p_name) : Abstract::Widget::Core(p_name)
			{
				_box.setDefaultValues(boxDefaultValue);
				_label.setDefaultValues(labelDefaultValue);

				setText("");
			}

			void setText(std::string p_text)
			{
				_label.setText(p_text);
			}

			jgl::Widget::Component::Box& box() { return (_box); }
			jgl::Widget::Component::Label& label() { return (_label); }

			const jgl::Widget::Component::Box& box() const { return (_box); }
			const jgl::Widget::Component::Label& label() const { return (_label); }
		};
	}
}