#include "structure/jgl2_iostream.h"

namespace jgl
{
	IOStream::IOBuffer::IOBuffer(const std::string& p_prefix)
	{
		setPrefix(p_prefix);
	}

	IOStream::IOBuffer::~IOBuffer() {
		if (pbase() != pptr()) {
			flush();
		}
	}

	int IOStream::IOBuffer::sync() {
		flush();
		return 0;
	}

	void IOStream::IOBuffer::flush() {
		_mutex.lock();
		if (_prefix.size() != 0)
			std::cout << "[" << std::string(_maximumPrefixSize - _prefix.size(), ' ') << _prefix << "] - ";
		std::cout << str();
		str("");
		std::cout.flush();
		_mutex.unlock();
	}

	void IOStream::IOBuffer::setPrefix(std::string p_prefix)
	{
		_prefix = p_prefix;
		_mutex.lock();
		if (_prefix.size() > _maximumPrefixSize)
			_maximumPrefixSize = static_cast<jgl::Size_t>(_prefix.size());
		_mutex.unlock();
	}

	IOStream::IOStream(std::string p_prefix) :
		std::ostream(&buffer),
		buffer(p_prefix)
	{

	}

	void IOStream::setPrefix(const std::string& p_prefix)
	{
		buffer.setPrefix(p_prefix);
	}

	thread_local jgl::IOStream cout;
}