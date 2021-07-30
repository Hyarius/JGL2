#pragma once

#include "jgl2_basic_types.h"
#include "jgl2_glyph.h"
#include "jgl2_array.h"

#define JGL_STRING_BUFFER_SIZE 100

namespace jgl
{
	class String
	{
	private:
		char* _str;
		// Buffer de taille fixe, opti pour la vitesse d'allocation des petites strings
		Glyph _buffer[JGL_STRING_BUFFER_SIZE];
		// Tableau a deux dimensions pour stocker les glyphs une fois le buffer rempli
		Glyph** _content;
		// Taille actuelle de la string
		Size_t _size;
		// Taille maximun que peut prendre la string dans l'etat actuel
		Size_t _max_size;

		void _clear_str()
		{
			if (_str != nullptr)
			{
				delete _str;
				_str = nullptr;
			}
		}

		/*
			Redefini la taille de la string a new_desired_size
		*/
		void _resize(Size_t new_desired_size);

		/*
			Ajoute une nouvelle ligne de taille JGL_STRING_BUFFER_SIZE
		*/
		void _add_new_line();

		/*
			Desalloue les lignes allouées dans le resize, a l'exception de la premiere, qui est l'adresse du buffer non malloc
			de la string
		*/
		void _delete_array_content();

		jgl::Size_t _count_word(const String& delim);

		jgl::Size_t _count_word(const jgl::Glyph& delim);

		jgl::Size_t _count_word_len(const jgl::Glyph& delim, const jgl::Size_t start);

		jgl::Size_t _count_word_len(const String& delim, const jgl::Size_t start);

	public:
		static jgl::Int npos;
		static Glyph nullchar;

		/*
			Default constructor
		*/
		String();

		/*
			Constructor from const char array
		*/
		String(const char* str);

		/*
			Constructor from a glyph array
		*/
		String(Glyph* str);

		/*
			Constructor by glyph multiplication
		
			Optional
			nb - The number of glyph [elem] you want inside the string
		*/
		String(Glyph elem, Size_t nb = 1);

		/*
			Constructor from std::string
		*/
		String(const std::string& other);

		/*
			Constructor from std::wstring
		*/
		String(const std::wstring& other);

		/*
			Copy constructor
		*/
		String(const String& other);

		/*
			Destructor
		*/
		~String();

		/*
			Return the number of bytes needed to store the whole string
		*/
		const jgl::Size_t total_size() const
		{
			jgl::Size_t result = 0;

			for (jgl::Size_t i = 0; i < _size; i++)
			{
				result += this->operator[](i).size();
			}
			return (result);
		}

		/*
			Return the number of glyph of the string
		*/
		const Size_t size() const;

		/*
			Return the last glyph of the string
		*/
		const Glyph& back() const;

		/*
			Add a glyph at the end of the string
		*/
		void push_back(const Glyph& elem);

		/*
			Add a glyph at the start of the string
		*/
		void push_front(const Glyph& elem);

		/*
			Add a glyph at the end of the string
			This is the same method as push_back
		*/
		void append(const String& other);

		/*
			Clear the content of the string
		*/
		void clear();

		/*
			Allocated enought memory to have [new_size] glyph in the string
		*/
		void resize(Size_t new_size);

		/*
			Returns true if the string is empty, false if not
		*/
		const jgl::Bool empty() const;

		/*
			Search for the glyph [to_find] and return the index of the first iteration
			return jgl::String::npos if not found
		*/
		const Int find(Glyph to_find) const;

		/*
			Search for the string [to_find] and return the index of the first iteration
			return jgl::String::npos if not found
		*/
		const Int find(String to_find) const;

		/*
			Return true if the string is compose only by glyph found in [to_find]. Return false if not
		*/
		const jgl::Bool compose_only(String& to_find) const;

		/*
			Create a substring from [start] with a lenght of [size] glyph, and return it
			If the base string is too short, return the maximum data it can harvest
		*/
		String substr(const Size_t start, const Size_t size) const;

		/*
			Create a substring from [start] with a lenght of [size] glyph, and place it in the string [result]
			If the base string is too short, return the maximum data it can harvest
		*/
		void substr(String& result, const Size_t start, const Size_t size) const;

		/*
			Remove the last element of the string
		*/
		void pop_back();

		/*
			Insert the glyph [to_add] at pos [index]
			If the string isn't long enought, this method will raise an exception
		*/
		void insert(const Glyph to_add, Size_t index);

		/*
			Remove the glyph at pos [index]
			If the string isn't long enought, this method will raise an exception
		*/
		void erase(Size_t index);

		/*
			Return an array of char, representing the whole string
			The resulting const char * will be of a lenght of [total_size()] char
		*/
		const char* c_str();

		/*
			Return an array of unsigned char, representing the whole string
			The resulting const char * will be of a lenght of [total_size()] char
		*/
		const unsigned char* uc_str();

		/*
			Place inside [tab] the subdivision of the base string by [delim]

			Optional
			jgl::Bool regroup - if true, delete the string empty inside tab after spliting it
		*/
		void split(jgl::Array<String>& tab, const String delim, const jgl::Bool regroup = true);

		/*
			return the subdivision of the base string by [delim]

			Optional
			jgl::Bool regroup - if true, delete the string empty inside tab after spliting it
		*/
		jgl::Array<String> split(const String delim, const jgl::Bool regroup = true);

		/*
			Accessor operator
		*/
		Glyph& operator [](const Size_t index);

		/*
			Const accessor operator
		*/
		const Glyph& operator [](const Size_t index) const;

		/*
			Assignement operator for jgl::String
		*/
		String& operator = (const String p_value)
		{
			clear();

			for (uint32_t i = 0; i < p_value.size(); i++)
				push_back(p_value[i]);

			return (*this);
		}

		/*
			Assignement operator for const char *
		*/
		String& operator = (const char *p_value)
		{
			clear();

			for (uint32_t i = 0; p_value[i] != '\0'; i++)
				push_back(p_value[i]);

			return (*this);
		}

		/*
			Operator + with jgl::String
			Will add [other] at the end of the base string and return the result
		*/
		String operator + (const String& other);

		/*
			Operator + with const char* and jgl::String
			Will return the string compose by [str] than [delta]
		*/
		friend String operator + (const char* str, const String& delta);

		/*
			Add at the end of the actual string the content of [other]
		*/
		void operator += (const String& other);

		/*
			Check if two string are fully equal, return true if yes, false if not
		*/
		jgl::Bool operator == (const String other) const;

		/*
			Check if two string are different, return true if yes, false if not
		*/
		jgl::Bool operator != (const String other) const;

		/*
			Check if the base string is smaller than [other], return true if yes, false if not
		*/
		jgl::Bool operator < (const jgl::String other) const;

		/*
			Check if the base string is greater than [other], return true if yes, false if not
		*/
		jgl::Bool operator > (const jgl::String other) const;

		/*
			Print the string
		*/
		friend std::ostream& operator<<(std::ostream& os, const String& value)
		{
			for (Size_t i = 0; i < value.size(); i++)
			{
				os << value[i];
			}
			return os;
		}
	};
}