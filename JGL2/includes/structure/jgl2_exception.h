#pragma once

#include <exception>

#include "jgl2_basic_types.h"

namespace jgl
{
    class Exception : public std::exception {
    private:
        jgl::Int _errorId;
        const char* _message;

    public:
        Exception(jgl::Int p_errorId, const char* p_message);
        jgl::Int errorId();
        const char* what();
    };
}