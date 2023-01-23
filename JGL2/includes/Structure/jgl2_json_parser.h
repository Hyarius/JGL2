#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_functions.h"

#include "jgl2_iostream.h"

class JSONData {
private:
    struct Element
    {
        enum class ValueType
        {
            Boolean,
            Integer,
            FloatingPoint,
            String,
            Array
        };
        std::string name;
        ValueType valueType;
        std::variant<bool, int, std::string, double> data;
        std::string arrayContent;

        friend jgl::IOStream& operator << (jgl::IOStream& p_os, const Element& p_element)
        {
            p_os << p_element.name << " / ";

            switch (p_element.valueType)
            {
            case Element::ValueType::Boolean:
            {
                p_os << std::get<bool>(p_element.data);
                break;
            }
            case Element::ValueType::Integer:
            {
                p_os << std::get<int>(p_element.data);
                break;
            }
            case Element::ValueType::FloatingPoint:
            {
                p_os << std::get<double>(p_element.data);
                break;
            }
            case Element::ValueType::String:
            {
                p_os << std::get<std::string>(p_element.data);
                break;
            }
            case Element::ValueType::Array:
            {
                p_os << p_element.arrayContent;
                break;
            }
            }
            return (p_os);
        }
    };

    struct Block
    {
        Block* owner;
        std::string name;
        std::vector<Element> elements;

        friend jgl::IOStream& operator << (jgl::IOStream& p_os, const Block& p_block)
        {
            p_os << "Name : " << p_block.name << jgl::endl;
            p_os << "Element : " << p_block.elements;
            return (p_os);
        }
    };

    std::vector<Block> blocks;

    std::map<std::string, std::string> data;
    std::string defaultValue = "NoKeyFound";

    std::string _readFileContent(std::string p_path);

    std::string _composeFullName(Block* p_element);

    void _extractElementName(std::string& p_str, jgl::Size_t& p_index, Element& p_element);
    void _skipDelimiter(std::string& p_str, jgl::Size_t& p_index);

    void _extractElementDataArray(std::string& p_str, jgl::Size_t& p_index, Element& p_element);
    void _extractElementDataString(std::string& p_str, jgl::Size_t& p_index, Element& p_element);
    void _extractElementDataValue(std::string& p_str, jgl::Size_t& p_index, Element& p_element);

    void _extractElementData(std::string& p_str, jgl::Size_t& p_index, Element& p_element);

    Element _composeElement(std::string& p_str, jgl::Size_t& p_index);

    Block _parseBlock(std::string p_name, std::string& p_str, jgl::Size_t& p_index, Block* p_owner);


public:
    void load(std::string p_path);
    std::string get(std::string key);
};