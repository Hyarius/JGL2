#pragma once

#include "jgl2_basic_types.h"

namespace jgl
{
	void _jgl_string_raise_exception(const char* str);

	template <typename T>
	class Array
	{

	private:
		bool _computed;
		T* _computed_result;

		T** _array_content;
		jgl::Size_t _size;
		jgl::Size_t _max_size;
		jgl::Size_t _push_size;

		void _add_new_line()
		{
			T** old_array_content = _array_content;

			jgl::Size_t nb_line = _max_size / _push_size;
			_max_size += _push_size;
			_array_content = new T * [nb_line + 1];

			for (jgl::Size_t i = 0; i < nb_line; i++)
				_array_content[i] = old_array_content[i];

			delete[] old_array_content;

			_array_content[nb_line] = new T[_push_size];
			for (jgl::Size_t i = 0; i < _push_size; i++)
				_array_content[nb_line][i] = T();

		}

		void _clear_computed()
		{
			if (_computed = true)
				delete _computed_result;
			_computed = false;
			_computed_result = nullptr;
		}
		void _delete_array_content()
		{
			if (_max_size == 0)
				return;

			if (_array_content != nullptr)
			{
				for (jgl::Size_t i = 0; i < _max_size / _push_size; i++)
				{
					delete[] _array_content[i];
				}
				delete[] _array_content;
				_array_content = nullptr;
			}
		}

	public:
		/*
			Iterator for the array
		*/
		class Iterator
		{
		private:
			const Array<T>* _parent;
			jgl::Size_t _index;
			const T* _element;

			/*
				Base constructor
				Set the iterator's parent to nullptr
				Set the element describe by the iterator to nullptr and his index to 0
			*/
			Iterator()
			{
				_parent = nullptr;
				_index = 0;
				_element = nullptr;
			}

			void _calc_element()
			{
				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				if (_index >= _parent->size())
					_element = nullptr;
				else
					_element = &(_parent->operator[](_index));
			}
			void _set_index(jgl::Size_t p_delta) {
				_index = p_delta;
			}
			void _set_element(const T* p_delta) {
				_element = p_delta;
			}
		public:

			/*
				Construct an iterator with desired index and parent
			*/
			Iterator(const Array<T>* p_parent, const jgl::Size_t p_index)
			{
				_parent = p_parent;
				_index = p_index;
				_calc_element();
			}

			/*
				Returns the parent of the iterator
			*/
			const Array<T>* parent() const { return (_parent); }

			/*
				Returns the index of the iterator
			*/
			const jgl::Size_t index() const { return (_index); }

			/*
				Returns the element of the iterator
			*/
			const T* element() const { return (_element); }

			/*
				Add both indexes and return the resulting iterator
			*/
			Iterator operator + (const Iterator other) const
			{
				Array<T>::Iterator result;
				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				if (other.parent() != _parent)
					_jgl_string_raise_exception("Bad interaction between two jgl::Array::Array<T>::Iterator from different array");

				if (result.index() + other.index() >= result.parent()->size())
					result._set_index(result.parent()->size());
				else
					result._set_index(result.index() + other.index());
				result._calc_element();
				return (result);
			}

			/*
				Remove from current interator index the value of the [other] iterator and return the resulting iterator
			*/
			Iterator operator - (const Iterator other) const
			{
				Array<T>::Iterator result;

				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				if (other.parent() != _parent)
					_jgl_string_raise_exception("Bad interaction between two jgl::Array::Array<T>::Iterator from different array");
				if (result.index() <= other.index())
					result._set_index(0);
				else
					result._set_index(result.index() - other.index());
				result._calc_element();
				return (result);
			}

			/*
				Add [delta] to the index in the current iterator and return the resulting iterator
			*/
			Iterator operator + (jgl::Int delta) const
			{
				Array<T>::Iterator result;

				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				if (result.index() + static_cast<jgl::Size_t>(delta) >= result.parent()->size())
					result._set_index(result.parent()->size());
				else
					result._set_index(result.index() + delta);
				result._calc_element();
				return (result);
			}

			/*
				Remove [delta] to the index in the current iterator and return the resulting iterator
			*/
			Iterator operator - (jgl::Int delta) const
			{
				Array<T>::Iterator result;

				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				if (result.index() <= static_cast<jgl::Size_t>(delta))
					result._set_index(0);
				else
					result._set_index(result.index() - delta);
				result._calc_element();
				return (result);
			}

			/*
				Increment the index by one
			*/
			void operator ++ ()
			{
				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				if (_index + 1 >= _parent->size())
					_index = _parent->size();
				else
					_index++;
				_calc_element();
			}

			/*
				Decrement the index by one
			*/
			void operator -- ()
			{
				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				if (_index == 0)
					return;
				_index--;
				_calc_element();
			}

			/*
				Assignement operator with jgl::Size_t

				Assign by index
			*/
			Iterator& operator = (const jgl::Size_t p_value)
			{
				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				_index = p_value;
				_calc_element();
				return (*this);
			}

			/*
				return the content of the iterator
			*/
			const T& operator*() const
			{
				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				if (_element == nullptr)
					_jgl_string_raise_exception("Dereferencing an empty jgl::Array::Array<T>::Iterator");
				return (*_element);
			}

			/*
				Check if two iterator are equal

				Return true if both parent and index are equal, false if not
			*/
			bool operator == (const Iterator other) const
			{
				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				if (_parent != other.parent())
					return (false);
				return (_index == other.index());
			}
			/*
				Check if two iterator are different

				Return true if iterators parents or indexes are differents, false if not
			*/
			bool operator != (const Iterator other) const
			{
				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				if (_parent != other.parent())
					return (false);
				return (_index != other.index());
			}

			/*
				Check if iterators are lower than [other], by comparing them indexes

				Return false if they do not have the same parent
			*/
			bool operator < (const Iterator other) const
			{
				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				if (_parent != other.parent())
					return (false);
				return (_index < other.index());
			}

			/*
				Check if iterators are greater than [other], by comparing them indexes

				Return false if they do not have the same parent
			*/
			bool operator > (const Iterator other) const
			{
				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				if (_parent != other.parent())
					return (false);
				return (_index > other.index());
			}

			/*
				Check if iterators are lower or equal than [other], by comparing them indexes

				Return false if they do not have the same parent
			*/
			bool operator <= (const Iterator other) const
			{
				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				if (_parent != other.parent())
					return (false);
				return (_index <= other.index());
			}

			/*
				Check if iterators are greater or equal than [other], by comparing them indexes

				Return false if they do not have the same parent
			*/
			bool operator >= (const Iterator other) const
			{
				if (_parent == nullptr)
					_jgl_string_raise_exception("No parent in jgl::Array : segfault");
				if (_parent != other.parent())
					return (false);
				return (_index >= other.index());
			}
		};

		/*
			Initializer_list constructor
		*/
		Array(const std::initializer_list<T> init) : Array<T>()
		{
			for (jgl::Size_t i = 0; i < init.size(); i++)
				push_back(init.begin()[i]);
		}

		/*
			Default constructor

			Optional :
			jgl::Size_t p_push_size - number of element per line
		*/
		Array(const jgl::Size_t p_push_size = 100)
		{
			_array_content = nullptr;
			_push_size = p_push_size;
			_size = 0;
			_max_size = 0;
			_computed = false;
			_computed_result = nullptr;
		}

		/*
			Copy constructor

			Full copy and not just pointer duplication
		*/
		Array(const Array<T>& other) : Array<T>(other._push_size)
		{
			for (jgl::Size_t i = 0; i < other.size(); i++)
				push_back(other[i]);
		}

		/*
			Destructor
		*/
		~Array()
		{
			_clear_computed();
			_delete_array_content();
		}

		/*
			Add an object at the end of the array
		*/
		T& push_back(const T elem)
		{
			if (_size + 1 >= _max_size)
			{
				_add_new_line();
			}
			jgl::Size_t nb_line = _size / _push_size;
			jgl::Size_t nb_index = _size % _push_size;
			_array_content[nb_line][nb_index] = elem;
			_size++;
			_clear_computed();
			return (_array_content[nb_line][nb_index]);
		}

		/*
			Add an object at the beggining of the array
		*/
		T& push_front(const T& elem)
		{
			return (insert(0, elem));
		}

		/*
			Return the first element of the array
		*/
		T& first()
		{
			if (_size == 0)
				_jgl_string_raise_exception("Segfault in array");
			return (this->operator[](0));
		}

		/*
			Return the last element of the array
		*/
		T& last()
		{
			if (_size == 0)
				_jgl_string_raise_exception("Segfault in array");
			return (this->operator[](_size - 1));
		}

		/*
			Return the first element of the array, as a const reference
		*/
		const T& first() const
		{
			if (_size == 0)
				_jgl_string_raise_exception("Segfault in array");
			return (this->operator[](0));
		}

		/*
			Return the last element of the array, as a const reference
		*/
		const T& last() const
		{
			if (_size == 0)
				_jgl_string_raise_exception("Segfault in array");
			return (this->operator[](_size - 1));
		}

		/*
			Return a const iterator pointing to the first element of the array
		*/
		const Iterator front() const
		{
			return (Iterator(this, 0));
		}

		/*
			Return a const iterator pointing to the last element of the array
		*/
		const Iterator back() const
		{
			if (size() == 0)
				return (end());
			return (Iterator(this, _size - 1));
		}


		/*
			Assignement operator with other jgl::Array

			Full copy and not just pointer duplication
		*/
		jgl::Array<T>& operator = (const jgl::Array<T>& other)
		{
			this->clear();
			for (jgl::Size_t i = 0; i < other.size(); i++)
				push_back(other[i]);

			return (*this);
		}

		/*
			Accessor operator
			Return a reference to the element [index] of the array

			Can raise exception if index out of range to prevent segfault
		*/
		T& operator [](const jgl::Size_t index)
		{
			if (index >= _size)
				_jgl_string_raise_exception("Segfault in jgl::Array");

			jgl::Size_t nb_line = index / _push_size;
			jgl::Size_t nb_index = index % _push_size;
			return (_array_content[nb_line][nb_index]);
		}

		/*
			Const accessor operator
			Return a const reference to the element [index] of the array

			Can raise exception if index out of range to prevent segfault
		*/
		const T& operator [](const jgl::Size_t index) const
		{
			if (index >= _size)
				_jgl_string_raise_exception("Segfault in jgl::Array");

			jgl::Size_t nb_line = index / _push_size;
			jgl::Size_t nb_index = index % _push_size;
			return (_array_content[nb_line][nb_index]);
		}

		/*
			Add an element [elem] at the desired [index]

			Can raise exception if array's size is too small, to prevent out of range to prevent segfault
		*/
		T& insert(const jgl::Size_t index, const T elem)
		{
			if (index >= _size + 1)
				_jgl_string_raise_exception("Segfault in jgl::Array");


			jgl::Size_t i = size();
			push_back({});
			while (i >= 1 && i > index)
			{
				T tmp = this->operator[](i - 1);
				this->operator[](i) = tmp;
				i--;
			}
			this->operator[](index) = elem;
			_clear_computed();
			return (this->operator[](index));
		}

		/*
			Add an element [elem] at the desired index describ by the iterator [iter]

			Can raise exception if array's size is too small, to prevent out of range to prevent segfault
		*/
		T& insert(const Iterator iter, const T elem)
		{
			if (iter.index() >= _size + 1)
				_jgl_string_raise_exception("Segfault in jgl::Array");

			jgl::Size_t i = size();
			push_back('\0');
			while (i >= 1 && i > iter.index())
			{
				T tmp = this->operator[](i - 1);
				this->operator[](i) = tmp;
				i--;
			}
			this->operator[](iter.index()) = elem;
			_clear_computed();
			return (this->operator[](iter.index()));
		}

		/*
			Allocate desired [new_space] element inside the array
		*/
		void resize(const jgl::Size_t new_size)
		{
			while (_max_size < new_size)
				_add_new_line();
			_size = new_size;
		}

		/*
			Reset array size

			/!\ This method do not release memory allocated by this array /!\
		*/
		void clear()
		{
			_size = 0;
			_clear_computed();
		}

		/*
			Check if the array is empty
		*/
		bool empty() const
		{
			return (_size == 0 ? true : false);
		}

		/*
			Search for the element [to_find] inside the array

			Return true if found, false if not
		*/
		bool contain(const T to_find) const
		{
			for (jgl::Size_t i = 0; i < size(); i++)
				if (this->operator[](i) == to_find)
					return(true);
			return (false);
		}

		/*
			Remove the last element of the array
		*/
		void pop_back()
		{
			if (_size > 0)
				_size--;
			_clear_computed();
		}

		/*
			Remove the first element of the array
		*/
		void pop_front()
		{
			erase(0);
		}

		/*
			Remove the element at position [index] 
		*/
		void erase(const jgl::Size_t index)
		{
			if (_size == 0)
				return;
			for (jgl::Size_t i = index; i < _size - 1; i++)
				this->operator[](i) = this->operator[](i + 1);
			_size--;
			_clear_computed();
		}

		/*
			Remove the element at position describe by the iterator [iter]
		*/
		void erase(const Iterator iter)
		{
			if (_size == 0)
				return;
			for (jgl::Size_t i = iter.index(); i < _size - 1; i++)
				this->operator[](i) = this->operator[](i + 1);
			_size--;
			_clear_computed();
		}

		/*
			Reverse every element inside the array
		*/
		void reverse()
		{
			for (jgl::Size_t i = 0; i < _size / 2; i++)
			{
				T tmp = this->operator[](i);
				this->operator[](i) = this->operator[](_size - i - 1);
				this->operator[](_size - i - 1) = tmp;
			}
		}

		/*
			Return true if the array has been computed
		*/
		const bool computed() const { return (_computed); }

		/*
			Return the computed content of the array

			/!\ The content is uninitialized at the creation of the array /!\
			/!\ Need to be initialized via the all() method /!\
		*/
		const T* computed_content() const { return (_computed_result); }

		/*
			Return the size of the array
		*/
		const jgl::Size_t size() const { return (_size); }

		/*
			Return the computed content of the array, in a single buffer
			If not computed, compute it

			/!\ Memory created by this method should not been deleted /!\
		*/
		const T* all()
		{
			if (_computed == false)
			{
				if (_computed_result != nullptr)
					delete _computed_result;
				_computed_result = new T[_size];

				for (jgl::Size_t i = 0; i < _size; i++)
					_computed_result[i] = this->operator[](i);
				_computed = true;
			}

			return (_computed_result);
		}

		/*
			Search for the element to_find and return an iterator pointing to the element
		*/
		Iterator find(const T to_find) const {
			return (find(to_find, 0));
		}

		/*
			Search from [start] for the element to_find and return an iterator pointing to the element
		*/
		Iterator find(const T to_find, const jgl::Size_t start) const {
			for (jgl::Size_t i = start; i < size(); i++)
			{
				if (this->operator[](i) == to_find)
					return (Iterator(this, i));
			}
			return (end());
		}

		/*
			Return an iterator for the first element of the array

			Same as the front() method
		*/
		Iterator begin() const
		{
			return (Iterator(this, 0));
		}
		/*
			Return an iterator at the end of the array

			/!\ This is not the same as the last() method /!\
		*/
		Iterator end() const
		{
			return (Iterator(this, size()));
		}

		/*
			Print the content of the array, separated by " - "
		*/
		friend std::ostream& operator<<(std::ostream& os, const jgl::Array<T> value)
		{
			for (jgl::Size_t i = 0; i < value.size(); i++)
			{
				if (i != 0)
					os << " - ";
				os << "[" << value[i] << "]";
			}
			return os;
		}
	};
}