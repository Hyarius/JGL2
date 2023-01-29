#pragma once

#include "jgl2_includes.h"
#include "jgl2_basic_functions.h"

#include "jgl2_iostream.h"

namespace jgl
{
	class JSONFile {
	private:
		using JSONData = std::variant<bool, int, std::string, double>;
		struct ParsingData
		{
			std::string name;
			JSONData data;
			std::string arrayContent;
		};

		struct Element
		{
			std::string name;
			JSONData data;

			Element(std::string p_name = "Undefined", JSONData p_data = JSONData()) :
				name(p_name),
				data(p_data)
			{

			}
		};

		struct Block
		{
			std::string name;
			std::vector<Element> elements;
		};

		std::vector<Block> blocks;

		std::map<std::string, JSONData> datas;

		std::string _readFileContent(std::string p_path);

		void _extractElementName(std::string& p_str, jgl::Size_t& p_index, ParsingData& p_element);
		void _skipDelimiter(std::string& p_str, jgl::Size_t& p_index);

		void _extractElementDataArray(std::string& p_str, jgl::Size_t& p_index, ParsingData& p_element);
		void _extractElementDataString(std::string& p_str, jgl::Size_t& p_index, ParsingData& p_element);
		void _extractElementDataValue(std::string& p_str, jgl::Size_t& p_index, ParsingData& p_element);

		void _extractElementData(std::string& p_str, jgl::Size_t& p_index, ParsingData& p_element);

		ParsingData _composeElement(std::string& p_str, jgl::Size_t& p_index);

		Block _parseBlock(std::string p_name, std::string& p_str, jgl::Size_t& p_index);

		void _exportBlockComposition();

	public:
		JSONFile()
		{

		}

		JSONFile(std::string p_filePath)
		{
			load(p_filePath);
		}
		void load(std::string p_path);
		JSONFile::JSONData get(std::string key);

		template<typename TType>
		TType smartGet(std::string key)
		{
			JSONFile::JSONData baseValue = get(key);

			return (std::get<TType>(baseValue));
		}

		jgl::Bool getBool(std::string key);
		jgl::Int getInteger(std::string key);
		std::string getString(std::string key);
		jgl::Float getFloatingPoint(std::string key);
	};
}