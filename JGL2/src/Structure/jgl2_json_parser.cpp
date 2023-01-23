#include "Structure/jgl2_json_parser.h"

std::string JSONData::_readFileContent(std::string p_path)
{
	std::fstream file;
	file.open(p_path, std::ios_base::in);

	std::stringstream buffer;
	buffer << file.rdbuf();

	file.close();

	return (buffer.str());
}

void JSONData::_extractElementName(std::string& p_str, jgl::Size_t& p_index, JSONData::Element& p_element)
{
	p_index++;
	jgl::Size_t start = p_index;
	for (; p_index < p_str.size() && (p_index == 0 || (p_str[p_index] != '\"' && p_str[p_index - 1] != '\\')); p_index++);

	p_element.name = (p_str.substr(start, p_index - start));
	p_index++;
}

void JSONData::_skipDelimiter(std::string& p_str, jgl::Size_t& p_index)
{
	for (; p_index < p_str.size() && (p_str[p_index] == '\n' || p_str[p_index] == '\t' || p_str[p_index] == ' '); p_index++);
	if (p_str[p_index] != ':')
		throw std::runtime_error("Error while parsing a JSON file : invalid delimiter");
	p_index++;
	for (; p_index < p_str.size() && (p_str[p_index] == '\n' || p_str[p_index] == '\t' || p_str[p_index] == ' '); p_index++);
}

void JSONData::_extractElementDataArray(std::string& p_str, jgl::Size_t& p_index, JSONData::Element& p_element)
{
	jgl::Size_t curlyBracketCount = 0;
	jgl::Size_t bracketCount = 0;
	jgl::Size_t start = p_index;
	if (p_str[p_index] == '[')
	{
		p_index++;
		bracketCount++;
	}
	if (p_str[p_index] == '{')
	{
		p_index++;
		curlyBracketCount++;
	}
	
	for (;curlyBracketCount != 0 || bracketCount != 0; p_index++)
	{
		if (p_str[p_index] == '[')
			bracketCount++;
		else if (p_str[p_index] == '{')
			curlyBracketCount++;
		else if (p_str[p_index] == ']')
			bracketCount--;
		else if (p_str[p_index] == '}')
			curlyBracketCount--;
	}

	p_element.arrayContent = (p_str.substr(start, p_index - start));
}

void JSONData::_extractElementDataString(std::string& p_str, jgl::Size_t& p_index, JSONData::Element& p_element)
{
	p_index++;
	jgl::Size_t start = p_index;
	for (; p_index < p_str.size() && (p_index == 0 || (p_str[p_index] != '\"' && p_str[p_index - 1] != '\\')); p_index++);
	p_index++;

	p_element.data = (p_str.substr(start, p_index - start - 1));
}

void JSONData::_extractElementDataValue(std::string& p_str, jgl::Size_t& p_index, JSONData::Element& p_element)
{
	jgl::Size_t start = p_index;
	for (; p_index < p_str.size() && p_str[p_index] != ','; p_index++);

	std::string valueInput = (p_str.substr(start, p_index - start));

	if (valueInput == "true")
	{
		p_element.data = true;
	}
	else if (valueInput == "false")
	{
		p_element.data = false;
	}
	else if (valueInput.find(".") != valueInput.npos)
	{
		p_element.data = std::stof(valueInput);
	}
	else
	{
		p_element.data = std::stoi(valueInput);
	}
}

void JSONData::_extractElementData(std::string& p_str, jgl::Size_t& p_index, JSONData::Element& p_element)
{
	if (p_str[p_index] == '[' || p_str[p_index] == '{')
	{
		_extractElementDataArray(p_str, p_index, p_element);
	}
	else if (p_str[p_index] == '"')
	{
		_extractElementDataString(p_str, p_index, p_element);
	}
	else
	{
		_extractElementDataValue(p_str, p_index, p_element);
	}
}

JSONData::Element JSONData::_composeElement(std::string& p_str, jgl::Size_t& p_index)
{
	JSONData::Element result;

	_extractElementName(p_str, p_index, result);
	_skipDelimiter(p_str, p_index);
	_extractElementData(p_str, p_index, result);

	return (result);
}

JSONData::Block JSONData::_parseBlock(std::string p_name, std::string& p_str, jgl::Size_t& p_index, JSONData::Block* p_owner)
{
	JSONData::Block result;

	result.owner = p_owner;
	result.name = p_name;
	for (; p_str[p_index] != '}';p_index++)
	{
		if (p_str[p_index] == '\"')
		{
			JSONData::Element newElement = _composeElement(p_str, p_index);

			if (newElement.arrayContent == "")
			{
				result.elements.push_back(newElement);
			}
			else
			{
				std::string baseName = (result.name != "" ? result.name + "." : "") + newElement.name;
				jgl::Size_t subBlockIndex = 0;
				for (jgl::Size_t tmpIndex = 0; tmpIndex < newElement.arrayContent.size(); tmpIndex++)
				{
					if (newElement.arrayContent[tmpIndex] == '{')
					{
						blocks.push_back(_parseBlock(baseName + "[" + std::to_string(subBlockIndex) + "]", newElement.arrayContent, tmpIndex, &result));
						subBlockIndex++;
					}
				}
			}
		}
	}

	return (result);
}

void JSONData::_exportBlockComposition()
{
	for (jgl::Size_t i = 0; i < blocks.size(); i++)
	{
		for (jgl::Size_t j = 0; j < blocks[i].elements.size(); j++)
		{
			if (blocks[i].name != "")
				jgl::cout << blocks[i].name << ".";
			jgl::cout << blocks[i].elements[j].name << jgl::endl;
		}
	}
}

void JSONData::load(std::string p_path)
{
	std::string fileContent = _readFileContent(p_path);

	jgl::Size_t index = 0;
	blocks.push_back(_parseBlock("", fileContent, index, nullptr));

	_exportBlockComposition();
}

std::string JSONData::get(std::string key)
{
	if (data.find(key) != data.end())
	{
		return data[key];
	}
	else
	{
		return defaultValue;
	}
}