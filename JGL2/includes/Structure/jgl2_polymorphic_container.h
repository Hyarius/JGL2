#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

namespace jgl
{
	class PolymorphicContainer
	{
	private:
		jgl::UInt id;
		jgl::Size_t size;
		jgl::Size_t readed;

		std::vector<jgl::UChar> content;

	public:
		PolymorphicContainer()
		{
			clear();
		}

		void clear()
		{
			id = 0;
			size = 0;
			readed = 0;
			content.clear();
		}

		template<typename DataType>
		PolymorphicContainer& operator << (const DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

			std::size_t old_size = (*this).content.size();

			(*this).content.resize((*this).content.size() + sizeof(DataType));

			std::memcpy((*this).content.data() + old_size, &data, sizeof(DataType));

			(*this).size = static_cast<jgl::Size_t>((*this).content.size());

			return (*this);
		}

		template<typename DataType>
		PolymorphicContainer& operator >> (DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

			jgl::Size_t next_size = (*this).readed;// (*this).content.size() - sizeof(DataType);

			std::memcpy(&data, (*this).content.data() + next_size, sizeof(DataType));

			(*this).readed += sizeof(DataType);

			return (*this);
		}
	};
}