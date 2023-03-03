#include "structure/jgl2_iostream.h"

namespace jgl
{
	IOStream::IOBuffer::IOBuffer(std::ostream& p_outputStream, const std::string& p_prefix) : 
		_outputStream(p_outputStream)
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
			_outputStream << "[" << std::string(_maximumPrefixSize - _prefix.size(), ' ') << _prefix << "] - ";
		_outputStream << str();
		str("");
		_outputStream.flush();
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

	IOStream::IOStream(std::ostream& p_outputStream, std::string p_prefix) :
		std::ostream(&buffer),
		buffer(p_outputStream, p_prefix)
	{

	}

	void IOStream::setPrefix(const std::string& p_prefix)
	{
		buffer.setPrefix(p_prefix);
	}

	thread_local jgl::IOStream cout = jgl::IOStream(std::cout);
	thread_local jgl::IOStream cerr = jgl::IOStream(std::cerr);
}