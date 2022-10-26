#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"
#include "Structure/jgl2_iostream.h"

namespace jgl
{
	template <typename TElement>
	class Pool
	{
	private:
		TElement* _defaultObject;
		std::vector<TElement*> _content;

	public:
		Pool()
		{
			_defaultObject = nullptr;
		}

		jgl::Size_t size()
		{
			return (static_cast<jgl::Size_t>(_content.size()));
		}

		void setDefaultObject(TElement* p_defaultObject)
		{
			_defaultObject = p_defaultObject;
		}

		void reserve(jgl::Size_t p_nbReservedObject)
		{
			_content.resize(p_nbReservedObject);

			for (jgl::Size_t i = 0; i < p_nbReservedObject; i++)
			{
				_content[i] = new TElement(*_defaultObject);
			}
		}

		TElement* obtain()
		{
			if (_defaultObject == nullptr)
			{
				throw jgl::Exception(1, "Can't allocate a new object for the pool");
			}

			if (_content.size() == 0)
			{
				_content.push_back(new TElement(*_defaultObject));
			}

			TElement* result = _content.back();

			*result = *_defaultObject;

			_content.pop_back();

			return (result);
		}

		void release(TElement* p_newContent)
		{
			_content.push_back(p_newContent);
		}
	};
}