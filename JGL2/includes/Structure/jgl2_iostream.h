#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_types.h"

namespace jgl
{
    class IOStream : public std::ostream
    {
    private:
        class IOBuffer : public std::stringbuf
        {
        private:
            static inline std::recursive_mutex _mutex;
            static inline jgl::Size_t _maximumPrefixSize = 0;
            std::string _prefix;

        public:
            IOBuffer(const std::string& p_prefix);
            ~IOBuffer();

            virtual int sync();
            void flush();
            void setPrefix(std::string p_prefix);
        };
        
    public:
        IOBuffer buffer;
    public:
        IOStream(std::string p_prefix = "");
        void setPrefix(const std::string& p_prefix);
    };

    extern thread_local jgl::IOStream cout;
}