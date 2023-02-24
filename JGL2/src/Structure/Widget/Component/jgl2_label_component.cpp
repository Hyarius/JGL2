#include "Structure/Widget/Component/jgl2_label_component.h"
#include "jgl2_drawing_functions.h"

namespace jgl
{
	namespace WidgetComponent
	{
		Label::Label() :
			_textSize(defaultValues.textSize),
			_outlineSize(defaultValues.outlineSize),
			_textColor(defaultValues.textColor),
			_outlineColor(defaultValues.outlineColor),
			_font(jgl::Application::instance()->defaultFont()),
			_horizontalAlignment(defaultValues.horizontalAlignment),
			_verticalAlignment(defaultValues.verticalAlignment),
			_text(""),
			_verticesBaked(false),
			_depthBaked(false)
		{
			_font.onEditValue([&]() {_verticesBaked = false; });
			_textSize.onEditValue([&]() {_verticesBaked = false; });
			_outlineSize.onEditValue([&]() {_verticesBaked = false; });
			_horizontalAlignment.onEditValue([&]() {_verticesBaked = false; });
			_verticalAlignment.onEditValue([&]() {_verticesBaked = false; });
		}

		void Label::setDefaultValues(DefaultValues& p_defaultValues)
		{
			_textSize.setDefaultValue(p_defaultValues.textSize);
			_outlineSize.setDefaultValue(p_defaultValues.outlineSize);
			_textColor.setDefaultValue(p_defaultValues.textColor);
			_outlineColor.setDefaultValue(p_defaultValues.outlineColor);
			_horizontalAlignment.setDefaultValue(p_defaultValues.horizontalAlignment);
			_verticalAlignment.setDefaultValue(p_defaultValues.verticalAlignment);
		}

		void Label::reset()
		{
			_textSize.reset();
			_outlineSize.reset();
			_textColor.reset();
			_outlineColor.reset();
			_font.reset();
			_horizontalAlignment.reset();
			_verticalAlignment.reset();
		}

		void Label::useValue()
		{
			_textSize.useValue();
			_outlineSize.useValue();
			_textColor.useValue();
			_outlineColor.useValue();
			_font.useValue();
			_horizontalAlignment.useValue();
			_verticalAlignment.useValue();
		}

		void Label::computeTextSize()
		{
			if (_text == "")
				setTextSize(0);
			else
				setTextSize(_font.value()->computeTextSize(_text, _outlineSize.value(), _size));
		}

		void Label::setText(std::string p_text)
		{
			_text = p_text;
			_verticesBaked = false;
		}

		void Label::setTextSize(jgl::Size_t p_textSize)
		{
			_textSize = p_textSize;
		}

		void Label::setOutlineSize(jgl::Size_t p_outlineSize)
		{
			_outlineSize = p_outlineSize;
		}

		void Label::setHorizontalAlignment(jgl::HorizontalAlignment p_alignment)
		{
			_horizontalAlignment = p_alignment;
		}

		void Label::setVerticalAlignment(jgl::VerticalAlignment p_alignment)
		{
			_verticalAlignment = p_alignment;
		}

		void Label::setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size)
		{
			_verticesBaked = false;
			_anchor = p_anchor;
			_size = p_size;
		}

		void Label::setDepth(jgl::Float p_depth)
		{
			_depthBaked = false;
			_depth = p_depth;
		}

		void Label::_computeTextAnchor()
		{
			_textRenderedSize = _font.value()->calcStringSize(_text, _textSize, _outlineSize);

			switch (_horizontalAlignment)
			{
			case jgl::HorizontalAlignment::Left:
				_textAnchorOffset.x = 0;
				break;
			case jgl::HorizontalAlignment::Centred:
				_textAnchorOffset.x = (size().x - _textRenderedSize.x) / 2;
				break;
			case jgl::HorizontalAlignment::Right:
				_textAnchorOffset.x = size().x - _textRenderedSize.x;
				break;
			}

			switch (_verticalAlignment)
			{
			case jgl::VerticalAlignment::Top:
				_textAnchorOffset.y = 0;
				break;
			case jgl::VerticalAlignment::Centred:
				_textAnchorOffset.y = (size().y - _textRenderedSize.y) / 2;
				break;
			case jgl::VerticalAlignment::Down:
				_textAnchorOffset.y = size().y - _textRenderedSize.y;
				break;
			}
		}

		void Label::_initializeShaderDatas()
		{
			if (_shader == nullptr)
				_shader = jgl::Application::instance()->shaders().get("TextTexture2D");

			_verticesBuffer = _shader->buffer("model_space")->copy();
			_uvsBuffer = _shader->buffer("model_uvs")->copy();
			_depthBuffer = _shader->buffer("model_depth")->copy();
			_elementBuffer = _shader->elementBuffer()->copy();

			_textureUniform = _shader->uniform("textureID");
			_textColorUniform = _shader->uniform("textColor");
			_outlineColorUniform = _shader->uniform("outlineColor");
			_outlineSizeUniform = _shader->uniform("outlineSize");

			_shaderInitialized = true;
		}

		void Label::_computeVerticesData()
		{
			_computeTextAnchor();

			_font.value()->prepareDraw(_text, _anchor + _textAnchorOffset, _textSize, _outlineSize, _depth);
			_font.value()->exportShaderData(_verticesBuffer, _depthBuffer, _uvsBuffer, _elementBuffer);
			_fontTextureID = _font.value()->textureID(_textSize, _outlineSize);

			_verticesBaked = true;
		}

		void Label::_computeDepthData()
		{
			_depthBaked = true;
		}

		void Label::render()
		{
			if (_shaderInitialized == false)
				_initializeShaderDatas();

			if (_verticesBaked == false)
				_computeVerticesData();

			if (_depthBaked == false)
				_computeDepthData();

			_shader->activate();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _fontTextureID);

			_verticesBuffer->activate();
			_depthBuffer->activate();
			_uvsBuffer->activate();
			_elementBuffer->activate();

			_textureUniform->send(0);
			_textColorUniform->send(_textColor.value());
			_outlineColorUniform->send(_outlineColor.value());
			_outlineSizeUniform->send(_outlineSize.value());

			_shader->cast(Shader::Mode::Triangle, _elementBuffer->size() / sizeof(jgl::UInt));
		}
	}
}