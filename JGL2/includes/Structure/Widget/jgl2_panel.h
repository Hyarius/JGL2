#pragma once

#include "Structure/Widget/jgl2_widget_core.h"

namespace jgl
{
	namespace Abstract
	{
		namespace Widget
		{
			class Panel : public jgl::Abstract::Widget::Core
			{
			private:
				static inline std::vector<Panel*> _panels;

				void _onActivation()
				{
					for (jgl::Size_t i = 0; i < _panels.size(); i++)
					{
						if (_panels[i] != this)
						{
							_panels[i]->deactivate();
						}
					}
				}

			public:
				Panel(std::string p_name) : jgl::Abstract::Widget::Core(p_name)
				{
					_addOnActivationCallback([&]() {Panel::_onActivation(); });
					_panels.push_back(this);
				}
				~Panel()
				{
					auto tmp = std::find(_panels.begin(), _panels.end(), this);

					if (tmp != _panels.end())
					{
						_panels.erase(tmp);
					}
				}
			};
		}
	}
}