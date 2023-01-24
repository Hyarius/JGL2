#include "Structure/jgl2_json_parser.h"

namespace jgl
{
	std::string JSONFile::_readFileContent(std::string p_path)
	{
		std::fstream file;
		file.open(p_path, std::ios_base::in);

		std::stringstream buffer;
		buffer << file.rdbuf();

		file.close();

		return (buffer.str());
	}

	void JSONFile::_extractElementName(std::string& p_str, jgl::Size_t& p_index, JSONFile::ParsingData& p_element)
	{
		p_index++;
		jgl::Size_t start = p_index;
		for (; p_index < p_str.size() && (p_index == 0 || (p_str[p_index] != '\"' && p_str[p_index - 1] != '\\')); p_index++);

		p_element.name = (p_str.substr(start, p_index - start));
		p_index++;
	}

	void JSONFile::_skipDelimiter(std::string& p_str, jgl::Size_t& p_index)
	{
		for (; p_index < p_str.size() && (p_str[p_index] == '\n' || p_str[p_index] == '\t' || p_str[p_index] == ' '); p_index++);
		if (p_str[p_index] != ':')
			throw std::runtime_error("Error while parsing a JSON file : invalid delimiter");
		p_index++;
		for (; p_index < p_str.size() && (p_str[p_index] == '\n' || p_str[p_index] == '\t' || p_str[p_index] == ' '); p_index++);
	}

	void JSONFile::_extractElementDataArray(std::string& p_str, jgl::Size_t& p_index, JSONFile::ParsingData& p_element)
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

		for (; curlyBracketCount != 0 || bracketCount != 0; p_index++)
		{
			switch (p_str[p_index])
			{
			case '[':
				bracketCount++;
				break;
			case '{':
				curlyBracketCount++;
				break;
			case ']':
				bracketCount--;
				break;
			case '}':
				curlyBracketCount--;
				break;
			}
		}

		p_element.arrayContent = (p_str.substr(start, p_index - start));
	}

	void JSONFile::_extractElementDataString(std::string& p_str, jgl::Size_t& p_index, JSONFile::ParsingData& p_element)
	{
		p_index++;
		jgl::Size_t start = p_index;
		for (; p_index < p_str.size() && (p_index == 0 || (p_str[p_index] != '\"' && p_str[p_index - 1] != '\\')); p_index++);
		p_index++;

		p_element.data = (p_str.substr(start, p_index - start - 1));
	}

	void JSONFile::_extractElementDataValue(std::string& p_str, jgl::Size_t& p_index, JSONFile::ParsingData& p_element)
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

	void JSONFile::_extractElementData(std::string& p_str, jgl::Size_t& p_index, JSONFile::ParsingData& p_element)
	{
		if (p_str[p_index] == '[' || p_str[p_index] == '{')
			_extractElementDataArray(p_str, p_index, p_element);
		else if (p_str[p_index] == '"')
			_extractElementDataString(p_str, p_index, p_element);
		else
			_extractElementDataValue(p_str, p_index, p_element);
	}

	JSONFile::ParsingData JSONFile::_composeElement(std::string& p_str, jgl::Size_t& p_index)
	{
		JSONFile::ParsingData result;

		_extractElementName(p_str, p_index, result);
		_skipDelimiter(p_str, p_index);
		_extractElementData(p_str, p_index, result);

		return (result);
	}

	JSONFile::Block JSONFile::_parseBlock(std::string p_name, std::string& p_str, jgl::Size_t& p_index)
	{
		JSONFile::Block result;

		result.name = p_name;
		for (; p_str[p_index] != '}'; p_index++)
		{
			if (p_str[p_index] == '\"')
			{
				JSONFile::ParsingData newElement = _composeElement(p_str, p_index);

				if (newElement.arrayContent == "")
					result.elements.push_back(Element(newElement.name, newElement.data));
				else
				{
					std::string baseName = (result.name != "" ? result.name + "." : "") + newElement.name;
					jgl::Size_t subBlockIndex = 0;
					for (jgl::Size_t tmpIndex = 0; tmpIndex < newElement.arrayContent.size(); tmpIndex++)
					{
						if (newElement.arrayContent[tmpIndex] == '{')
						{
							std::string name = baseName + "[" + std::to_string(subBlockIndex) + "]";
							if (tmpIndex == 0)
								name = baseName;

							blocks.push_back(_parseBlock(name, newElement.arrayContent, tmpIndex));
							subBlockIndex++;
						}
					}

					result.elements.push_back(Element(baseName + ".size", static_cast<int>(subBlockIndex)));
				}
			}
		}

		return (result);
	}

	void JSONFile::_exportBlockComposition()
	{
		for (jgl::Size_t i = 0; i < blocks.size(); i++)
		{
			for (jgl::Size_t j = 0; j < blocks[i].elements.size(); j++)
			{
				datas[(blocks[i].name != "" ? blocks[i].name + "." : "") + blocks[i].elements[j].name] = blocks[i].elements[j].data;
			}
		}
	}

	void JSONFile::load(std::string p_path)
	{
		std::string fileContent = _readFileContent(p_path);

		jgl::Size_t index = 0;
		blocks.push_back(_parseBlock("", fileContent, index));

		_exportBlockComposition();

		blocks.clear();
	}

	JSONFile::JSONData JSONFile::get(std::string key)
	{
		if (datas.find(key) != datas.end())
			return datas[key];
		else
			return (JSONFile::JSONData());
	}

	jgl::Bool JSONFile::getBool(std::string key)
	{
		return (std::get<bool>(get(key)));
	}

	jgl::Int JSONFile::getInteger(std::string key)
	{
		return (std::get<int>(get(key)));
	}
	std::string JSONFile::getString(std::string key)
	{
		return (std::get<std::string>(get(key)));
	}
	jgl::Float JSONFile::getFloatingPoint(std::string key)
	{
		return (std::get<double>(get(key)));
	}
}