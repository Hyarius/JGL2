#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

template <typename TType>
class MonitoredValue
{
private:
	const TType& _defaultValue;
	TType _value;
	const TType* _actualValue;
	std::vector<std::function<void()>> _onEditValue;

	void _notify()
	{
		for (auto funct : _onEditValue)
		{
			funct();
		}
	}

public:
	MonitoredValue(const TType& p_defaultValue) :
		_defaultValue(p_defaultValue),
		_actualValue(&_defaultValue),
		_value()
	{
		_value = {};
	}

	template <typename Func, typename... Args>
	void onEditValue(Func&& p_funct, Args&&... p_args)
	{
		_onEditValue.push_back(std::bind(std::forward<Func>(p_funct), std::forward<Args>(p_args)...));
	}

	operator const TType& () const
	{
		return (*_actualValue);
	}

	const TType& value() const
	{
		return (*_actualValue);
	}

	void operator = (TType p_newValue)
	{
		_value = p_newValue;
		useValue();
	}
	void reset()
	{
		_actualValue = &_defaultValue;
		_notify();
	}
	void useValue()
	{
		_actualValue = &_value;
		_notify();
	}
};