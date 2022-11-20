#pragma once

#include "Structure/Widget/jgl2_widget.h"

namespace jgl
{
	template<typename T>
	class SingletonWidget
	{
		/*
		* Don't forget to add 'friend class SingletonWidget<T>;' to T definition
		*/
	protected:
		static inline T* _instance = nullptr;

		SingletonWidget()
		{
			static_assert(std::is_base_of<jgl::Widget, T>::value, "SingletonWidget can only allow jgl::Widget enherenced object");
		}

	public:
		static T* instanciate(jgl::Widget* p_parent)
		{
			if (_instance == nullptr)
			{
				_instance = new T(p_parent);
			}
			return (_instance);
		}
		static T* instance()
		{
			return (_instance);
		}
		static void release()
		{
			if (_instance != nullptr)
				delete _instance;
			_instance = nullptr;
		}
	};
}