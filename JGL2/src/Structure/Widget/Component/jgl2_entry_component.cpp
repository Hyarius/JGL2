#include "Structure/Widget/Component/jgl2_entry_component.h"
#include "jgl2_drawing_functions.h"

namespace jgl::Widget::Component
{
	Entry::Entry() :
		_text(&(defaultValues.text)),
		_placeholder(&(defaultValues.placeholder)),
		_textSize(&(defaultValues.textSize)),
		_outlineSize(&(defaultValues.outlineSize)),
		_textColor(&(defaultValues.textColor)),
		_outlineColor(&(defaultValues.outlineColor)),
		_font(&(jgl::Application::Graphical::instance()->defaultFont())),
		_horizontalAlignment(&(defaultValues.horizontalAlignment)),
		_verticalAlignment(&(defaultValues.verticalAlignment)),
		_verticesBaked(false),
		_depthBaked(false)
	{
		_text.onEditValue([&]() {_verticesBaked = false; });
		_placeholder.onEditValue([&]() {_verticesBaked = false; });
		_font.onEditValue([&]() {_verticesBaked = false; });
		_outlineSize.onEditValue([&]() {_verticesBaked = false; });
		_textSize.onEditValue([&]() {_verticesBaked = false; });
		_outlineSize.onEditValue([&]() {_verticesBaked = false; });
		_horizontalAlignment.onEditValue([&]() {_verticesBaked = false; });
		_verticalAlignment.onEditValue([&]() {_verticesBaked = false; });
	}

	void Entry::setDefaultValues(DefaultValues& p_defaultValues)
	{
		_text.setDefaultValue(&(p_defaultValues.text));
		_placeholder.setDefaultValue(&(p_defaultValues.placeholder));
		_textSize.setDefaultValue(&(p_defaultValues.textSize));
		_outlineSize.setDefaultValue(&(p_defaultValues.outlineSize));
		_textColor.setDefaultValue(&(p_defaultValues.textColor));
		_outlineColor.setDefaultValue(&(p_defaultValues.outlineColor));
		_horizontalAlignment.setDefaultValue(&(p_defaultValues.horizontalAlignment));
		_verticalAlignment.setDefaultValue(&(p_defaultValues.verticalAlignment));
	}

	void Entry::reset()
	{
		_text.reset();
		_placeholder.reset();
		_textSize.reset();
		_outlineSize.reset();
		_textColor.reset();
		_outlineColor.reset();
		_font.reset();
		_horizontalAlignment.reset();
		_verticalAlignment.reset();
	}

	void Entry::useValue()
	{
		_text.useValue();
		_placeholder.useValue();
		_textSize.useValue();
		_outlineSize.useValue();
		_textColor.useValue();
		_outlineColor.useValue();
		_font.useValue();
		_horizontalAlignment.useValue();
		_verticalAlignment.useValue();
	}

	void Entry::computeTextSize()
	{
		if (_getInternalText() == "")
			setTextSize(0);
		else
			setTextSize(_font.value()->computeTextSize(_getInternalText(), _outlineSize.value(), _size));
	}

	jgl::Size_t Entry::calculateTextSize()
	{
		if (_getInternalText() == "")
			return (UINT32_MAX);
		else
			return (_font.value()->computeTextSize(_getInternalText(), _outlineSize.value(), _size));
	}

	void Entry::setText(std::string p_text)
	{
		if (_text == p_text)
			return;
		_text = p_text;
		_verticesBaked = false;
	}

	void Entry::setPlaceholder(std::string p_placeholder)
	{
		if (_placeholder == p_placeholder)
			return;
		_placeholder = p_placeholder;
		_verticesBaked = false;
	}

	void Entry::setTextSize(jgl::Size_t p_textSize)
	{
		_textSize = p_textSize;
	}

	void Entry::setOutlineSize(jgl::Size_t p_outlineSize)
	{
		_outlineSize = p_outlineSize;
	}

	void Entry::setTextColor(jgl::Color p_textColor)
	{
		_textColor = p_textColor;
	}

	void Entry::setOutlineColor(jgl::Color p_outlineColor)
	{
		_outlineColor = p_outlineColor;
	}

	void Entry::setHorizontalAlignment(jgl::HorizontalAlignment p_alignment)
	{
		_horizontalAlignment = p_alignment;
	}

	void Entry::setVerticalAlignment(jgl::VerticalAlignment p_alignment)
	{
		_verticalAlignment = p_alignment;
	}

	void Entry::setGeometry(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size)
	{
		_anchor = p_anchor;
		_size = p_size;
		_verticesBaked = false;
	}

	void Entry::setDepth(jgl::Float p_depth)
	{
		_depth = p_depth;
		_depthBaked = false;
	}

	void Entry::_computeTextAnchor()
	{
		_textRenderedSize = _font.value()->calcStringSize(_getInternalText(), _textSize, _outlineSize);

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

	void Entry::_initializeShaderDatas()
	{
		if (_shader == nullptr)
			_shader = jgl::Application::Graphical::instance()->shaders().get("TextTexture2D");

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

	void Entry::_computeVerticesData()
	{
		_computeTextAnchor();

		_font.value()->prepareDraw(_getInternalText(), _anchor + _textAnchorOffset, _textSize, _outlineSize, _depth);
		_font.value()->exportShaderData(_verticesBuffer, _depthBuffer, _uvsBuffer, _elementBuffer);
		_fontTextureID = _font.value()->textureID(_textSize, _outlineSize);

		_verticesBaked = true;
	}

	void Entry::_computeDepthData()
	{
		_depthBaked = true;
	}

	void Entry::_castRender()
	{
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

	void Entry::render()
	{
		if (_getInternalText() == "")
			return;

		if (_shaderInitialized == false)
			_initializeShaderDatas();

		if (_verticesBaked == false)
			_computeVerticesData();

		if (_depthBaked == false)
			_computeDepthData();

		_castRender();
	}

	jgl::Bool Entry::update()
	{

		return (false);
	}
}