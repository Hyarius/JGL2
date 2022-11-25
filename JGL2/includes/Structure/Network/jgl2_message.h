#pragma once


#include "jgl2_basic_types.h"
#include "Structure/jgl2_iostream.h"
#include "Structure/jgl2_thread.h"

namespace jgl
{
	/*
		Message header :

		Id		enum class (mandatory)	4 bytes
		Size	uint16_t				4 bytes
	*/
	template <typename TServerMessageEnum >
	struct MessageHeader
	{
		TServerMessageEnum id{};
		jgl::Size_t size = 0;
		jgl::Size_t readed = 0;

		MessageHeader()
		{

		}
		MessageHeader(TServerMessageEnum p_value)
		{
			id = p_value;
			size = 0;
			readed = 0;
		}
	};

	template <typename TServerMessageEnum >
	struct Message
	{
		MessageHeader<TServerMessageEnum> header{};
		std::vector<jgl::UChar> content;

		TServerMessageEnum type() const
		{
			return (header.id);
		}

		jgl::UInt size() const
		{
			return (static_cast<jgl::UInt>(header.size - header.readed));
		}

		jgl::Bool empty()
		{
			return (header.size <= header.readed);
		}

		Message(TServerMessageEnum p_type = {})
		{
			header = MessageHeader<TServerMessageEnum>(p_type);
			content.clear();
		}

		void clear()
		{
			header.size = 0;
			header.readed = 0;
			content.clear();
		}

		void skip(jgl::Size_t p_nb_to_skip)
		{
			header.readed += p_nb_to_skip;
		}

		friend std::ostream& operator << (std::ostream& p_os, const Message<TServerMessageEnum>& p_msg)
		{
			p_os << "ID:" << int(p_msg.header.id) << " Size:" << p_msg.header.size;
			return p_os;
		}

		void addInArray(jgl::UChar* p_array, jgl::Size_t p_length)
		{
			jgl::Size_t old_size = content.size();

			content.resize(content.size() + p_length);

			std::memcpy(content.data() + old_size, p_array, p_length);

			header.size = content.size();
		}

		void loadFromArray(void* p_address, jgl::Size_t p_length)
		{
			jgl::Size_t next_size = header.readed;// content.size() - sizeof(DataType);

			std::memcpy(p_address, content.data() + next_size, p_length);

			header.readed += p_length;
		}

		void print(std::string p_text, jgl::Bool p_binary_mode = true)
		{
			jgl::cout << p_text << " : ";
			for (jgl::Size_t i = 0; i < content.size(); i++)
			{
				if (i != 0)
					jgl::cout << "-";
				if (p_binary_mode == true)
				{
					std::bitset<8> y(content[i]);
					jgl::cout << "[" << y << "]";
				}
				else
				{
					jgl::cout << "[" << content[i] << "]";
				}
			}
			jgl::cout << jgl::endl;
		}

		template<typename DataType>
		Message<TServerMessageEnum>& operator << (const DataType& p_data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

			size_t old_size = (*this).content.size();

			(*this).content.resize((*this).content.size() + sizeof(DataType));

			std::memcpy((*this).content.data() + old_size, &p_data, sizeof(DataType));

			(*this).header.size = static_cast<jgl::Size_t>((*this).content.size());

			return (*this);
		}

		template<typename DataType>
		Message<TServerMessageEnum>& operator >> (DataType& p_data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

			jgl::Size_t next_size = (*this).header.readed;// (*this).content.size() - sizeof(DataType);

			std::memcpy(&p_data, (*this).content.data() + next_size, sizeof(DataType));

			(*this).header.readed += sizeof(DataType);

			return (*this);
		}

		template<>
		Message<TServerMessageEnum>& operator << <std::string>	(const std::string& p_text)
		{
			*this << p_text.size();
			for (jgl::Size_t i = 0; i < p_text.size(); i++)
				*this << p_text[i];
			return *this;
		}

		template<>
		Message<TServerMessageEnum>& operator >> <std::string>	(std::string& p_text)
		{
			jgl::Size_t size;
			jgl::UInt i = 0;

			*this >> size;
			p_text.clear();
			while (empty() == false && i < size)
			{
				jgl::Char c;
				*this >> c;
				p_text.push_back(c);
				i++;
			}

			return *this;
		}
	};

	template <typename TServerMessageEnum>
	class Connection;

	template <typename TServerMessageEnum>
	struct InputMessage
	{
		Connection<TServerMessageEnum>* remote = nullptr;
		Message<TServerMessageEnum> msg;
		jgl::ULong time;
	};
}