#pragma once

#include "jgl2_widget_core.h"
#include "Component\jgl2_box_component.h"
#include "Component\jgl2_entry_component.h"

namespace jgl
{
	namespace Widget
	{
		class Entry : public Abstract::Widget::Core
		{
		public:
			static inline jgl::Widget::Component::Box::DefaultValues boxDefaultValue;
			static inline jgl::Widget::Component::Entry::DefaultValues entryDefaultValue;

		private:
			jgl::Widget::Component::Box _box;
			jgl::Widget::Component::Entry _entry;

			jgl::Bool _onUpdate()
			{
				return (false);
			}
			void _onRender()
			{
				_entry.render();
				_box.render();
			}
			void _onGeometryChange()
			{
				_box.setGeometry(anchor(), size());
				_box.setDepth(depth());

				_entry.setGeometry(_box.usableAnchor(), _box.usableSize());
				_entry.setDepth(depth() + 0.5f);
			}

		public:
			Entry(std::string p_name) : Abstract::Widget::Core(p_name)
			{
				_box.setDefaultValues(boxDefaultValue);
				_entry.setDefaultValues(entryDefaultValue);

				setText("");
			}

			void setText(std::string p_text)
			{
				_entry.setText(p_text);
			}

			jgl::Widget::Component::Box& box() { return (_box); }
			jgl::Widget::Component::Entry& entry() { return (_entry); }

			const jgl::Widget::Component::Box& box() const { return (_box); }
			const jgl::Widget::Component::Entry& entry() const { return (_entry); }
		};
	}
}