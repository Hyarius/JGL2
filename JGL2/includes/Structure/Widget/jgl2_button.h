#pragma once

#include "jgl2_widget_core.h"
#include "Component\jgl2_box_component.h"
#include "Component\jgl2_label_component.h"

namespace jgl
{
	namespace Widget
	{
		class Button : public Abstract::Widget::Core
		{
		public:
			static inline jgl::Widget::Component::Box::DefaultValues unselectedBoxDefaultValue = {
				jgl::Color(120, 120, 120),
				jgl::Color(150, 150, 150),
				jgl::Vector2Int(5, 5)
			};
			static inline jgl::Widget::Component::Label::DefaultValues unselectedLabelDefaultValue = {
				40,
				5,
				jgl::Color(255, 255, 255),
				jgl::Color(0, 0, 0),
				jgl::HorizontalAlignment::Centred,
				jgl::VerticalAlignment::Centred
			};

			static inline jgl::Widget::Component::Box::DefaultValues selectedBoxDefaultValue = {
				jgl::Color(70, 70, 70),
				jgl::Color(100, 100, 100),
				jgl::Vector2Int(5, 5)
			};
			static inline jgl::Widget::Component::Label::DefaultValues selectedLabelDefaultValue = {
				40,
				5,
				jgl::Color(205, 205, 205),
				jgl::Color(0, 0, 0),
				jgl::HorizontalAlignment::Centred,
				jgl::VerticalAlignment::Centred
			};

		private:
			jgl::Bool _pressed = false;
			std::function< void() > _funct;

			jgl::Widget::Component::Box _boxes[2];
			jgl::Widget::Component::Label _labels[2];

			jgl::Bool _onUpdate()
			{
				if (_pressed == true && isPointed() == false)
					_pressed = false;
				if (jgl::Application::Graphical::instance()->mouse()->getButton(jgl::Mouse::Button::Left) == jgl::InputStatus::Down &&
					isPointed() == true)
				{
					_pressed = true;
					return (true);
				}
				if (jgl::Application::Graphical::instance()->mouse()->getButton(jgl::Mouse::Button::Left) == jgl::InputStatus::Released &&
					isPointed() == true)
				{
					_pressed = false;
					_funct();
					return (true);
				}
				return (false);
			}
			void _onRender()
			{
				jgl::Int index = (_pressed == true ? 1 : 0);

				_labels[index].render();
				_boxes[index].render();
			}
			void _onGeometryChange()
			{
				for (jgl::Size_t i = 0; i < 2; i++)
				{
					_boxes[i].setGeometry(anchor(), size());
					_boxes[i].setDepth(depth());

					_labels[i].setGeometry(_boxes[i].usableAnchor(), _boxes[i].usableSize());
					_labels[i].setDepth(depth() + 0.5f);
				}
			}

		public:
			template <typename Func, typename... Args>
			Button(std::string p_name, Func&& p_func, Args&&... p_args) : Abstract::Widget::Core(p_name)
			{
				setFunct(std::forward<Func>(p_func), std::forward<Args>(p_args)...);

				_boxes[0].setDefaultValues(unselectedBoxDefaultValue);
				_labels[0].setDefaultValues(unselectedLabelDefaultValue);

				_boxes[1].setDefaultValues(selectedBoxDefaultValue);
				_labels[1].setDefaultValues(selectedLabelDefaultValue);
			}

			template <typename Func, typename... Args>
			void setFunct(Func&& p_func, Args&&... p_args)
			{
				_funct = std::bind(std::forward<Func>(p_func), std::forward<Args>(p_args)...);
			}

			void setText(std::string p_text)
			{
				for (jgl::Size_t i = 0; i < 2; i++)
				{
					_labels[i].setText(p_text);
				}
			}

			jgl::Widget::Component::Box* boxes() { return (_boxes); }
			jgl::Widget::Component::Label* labels() { return (_labels); }

			const jgl::Widget::Component::Box* boxes() const { return (_boxes); }
			const jgl::Widget::Component::Label* labels() const { return (_labels); }

			jgl::Widget::Component::Box& unselectedBox() { return (_boxes[0]); }
			jgl::Widget::Component::Label& unselectedLabel() { return (_labels[0]); }

			const jgl::Widget::Component::Box& unselectedBox() const { return (_boxes[0]); }
			const jgl::Widget::Component::Label& unselectedLabel() const { return (_labels[0]); }

			jgl::Widget::Component::Box& selectedBox() { return (_boxes[1]); }
			jgl::Widget::Component::Label& selectedLabel() { return (_labels[1]); }

			const jgl::Widget::Component::Box& selectedBox() const { return (_boxes[1]); }
			const jgl::Widget::Component::Label& selectedLabel() const { return (_labels[1]); }
		};
	}
}