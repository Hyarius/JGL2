#include "Structure/jgl2_exception.h"

namespace jgl
{
    Exception::Exception(jgl::Int p_errorId, const char* p_message)
    {
        _errorId = p_errorId;
        _message = p_message;
    }

    jgl::Int Exception::errorId()
    {
        return (_errorId);
    }

    const char* Exception::what() {
        return _message;
    }
}