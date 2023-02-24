#include "Structure/Widget/Component/jgl2_box_component.h"

namespace jgl::Widget::Component
{
	void Box::_initializeShaderDatas()
	{
		_shader = jgl::Application::Graphical::instance()->shaders().get("Color2D");
		_modelSpace = _shader->buffer("model_space")->copy();
		_modelDepth = _shader->buffer("model_depth")->copy();
		_modelColor = _shader->buffer("model_color")->copy();
		_elementBuffer = _shader->elementBuffer()->copy();

		_shaderInitialized = true;
	}

	void Box::_bakeColors()
	{
		for (jgl::Size_t i = 0; i < 8; i++)
		{
			_modelColorBufferData[i] = (i < 4 ? _color : _borderColor);
		}

		_modelColor->send(_modelColorBufferData, 8);

		_colorBaked = true;
	}

	void Box::_bakeVertices()
	{
		for (jgl::Size_t nbSquare = 0; nbSquare < 2; nbSquare++)
		{
			jgl::Vector2Int anchor = _anchor;
			jgl::Vector2 squareSize = _size;
			if (nbSquare == 0)
			{
				anchor += _borderSize.value();
				squareSize = (_size - _borderSize.value() * 2);
			}

			for (jgl::Int i = 0; i < 4; i++)
			{
				_modelSpaceBufferData[nbSquare * 4 + i] = jgl::Application::Graphical::instance()->convertScreenToOpenGL(anchor + squareSize * jgl::Vector2Int(i % 2, i / 2));
			}
		}

		_modelSpace->send(_modelSpaceBufferData, 8);
		_elementBuffer->send(_elementBufferData, 12);

		_verticesBaked = true;
	}

	void Box::_bakeDepth()
	{
		for (size_t i = 0; i < 8; i++)
		{
			_modelDepthBufferData[i] = jgl::Application::Graphical::instance()->convertDepthToOpenGL(_depth);
		}

		_modelDepth->send(_modelDepthBufferData, 8);

		_depthBaked = true;
	}

	Box::Box() :
		_color(defaultValues.color),
		_borderColor(defaultValues.borderColor),
		_borderSize(defaultValues.borderSize)
	{
		_borderSize.onEditValue([&]() {_verticesBaked = false; });
		_color.onEditValue([&]() {_colorBaked = false; });
		_borderColor.onEditValue([&]() {_colorBaked = false; });
	}

	void Box::setDefaultValues(DefaultValues& p_defaultValue)
	{
		_color.setDefaultValue(p_defaultValue.color);
		_borderSize.setDefaultValue(p_defaultValue.borderSize);
		_borderColor.setDefaultValue(p_defaultValue.borderColor);
	}

	void Box::reset()
	{
		_color.reset();
		_borderColor.reset();
		_borderSize.reset();
	}
	void Box::useValue()
	{
		_color.useValue();
		_borderColor.useValue();
		_borderSize.useValue();
	}
	void Box::setBorderSize(jgl::Vector2Int p_borderSize)
	{
		_borderSize = p_borderSize;
	}
	void Box::setColors(jgl::Color p_color, jgl::Color p_borderColor)
	{
		_color = p_color;
		_borderColor = p_borderColor;
	}
	void Box::setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size)
	{
		_anchor = p_anchor;
		_size = p_size;
		_verticesBaked = false;
	}
	void Box::setDepth(jgl::Float p_depth)
	{
		_depth = p_depth;
		_depthBaked = false;
	}
	void Box::render()
	{
		if (_shaderInitialized == false)
			_initializeShaderDatas();

		if (_colorBaked == false)
			_bakeColors();

		if (_verticesBaked == false)
			_bakeVertices();

		if (_depthBaked == false)
			_bakeDepth();

		_shader->activate();

		_modelSpace->activate();
		_modelDepth->activate();
		_modelColor->activate();
		_elementBuffer->activate();

		_shader->cast(jgl::Shader::Mode::Triangle, _elementBuffer->size() / sizeof(jgl::UInt));
	}
}