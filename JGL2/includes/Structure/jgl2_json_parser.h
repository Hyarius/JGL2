#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_functions.h"

#include "jgl2_iostream.h"

class JSONData {
private:
    struct Element
    {
        std::string name;
        std::variant<bool, int, std::string, double> data;
        std::string arrayContent;
    };

    struct Block
    {
        Block* owner;
        std::string name;
        std::vector<Element> elements;
    };

    std::vector<Block> blocks;

    std::map<std::string, std::string> data;
    std::string defaultValue = "NoKeyFound";

    std::string _readFileContent(std::string p_path);

    void _extractElementName(std::string& p_str, jgl::Size_t& p_index, Element& p_element);
    void _skipDelimiter(std::string& p_str, jgl::Size_t& p_index);

    void _extractElementDataArray(std::string& p_str, jgl::Size_t& p_index, Element& p_element);
    void _extractElementDataString(std::string& p_str, jgl::Size_t& p_index, Element& p_element);
    void _extractElementDataValue(std::string& p_str, jgl::Size_t& p_index, Element& p_element);

    void _extractElementData(std::string& p_str, jgl::Size_t& p_index, Element& p_element);

    Element _composeElement(std::string& p_str, jgl::Size_t& p_index);

    Block _parseBlock(std::string p_name, std::string& p_str, jgl::Size_t& p_index, Block* p_owner);

    void _exportBlockComposition();

public:
    void load(std::string p_path);
    std::string get(std::string key);
};