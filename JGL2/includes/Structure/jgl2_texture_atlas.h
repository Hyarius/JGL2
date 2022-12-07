#pragma once

#include "jgl2_includes.h"
#include "Structure/jgl2_singleton.h"
#include "Structure/jgl2_image.h"

namespace jgl
{
	class TextureAtlas
	{
	private:
		std::map<std::string, jgl::Image*> _textureMap;

	public:
		TextureAtlas()
		{

		}
		void addTexture(std::string p_name, jgl::Image* p_image)
		{
			_textureMap[p_name] = p_image;
		}
		jgl::Image* operator [] (std::string p_name) const
		{
			if (_textureMap.count(p_name) == 0)
				return (nullptr);
			return (_textureMap.at(p_name));
		}
	};
}