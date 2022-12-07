#include "Structure/Widget/Component/jgl2_box_component.h"
#include "jgl2_drawing_functions.h"
#include "Structure/jgl2_application.h"

namespace jgl
{
	namespace WidgetComponent
	{
		Box::Box()
		{
			_computed = false;
		}

		const Color& Box::backgroundColor() const
		{
			return (_backgroundColor);
		}

		const Color& Box::frontgroundColor() const
		{
			return (_frontgroundColor);
		}

		const Vector2Int& Box::anchor() const
		{
			return (_anchor);
		}

		const Vector2Int& Box::size() const
		{
			return (_size);
		}

		const Vector2Int& Box::borderSize() const
		{
			return (_borderSize);
		}

		const Vector2Int& Box::usableAnchor() const
		{
			return (_usableAnchor);
		}
		
		const Vector2Int& Box::usableSize() const
		{
			return (_usableSize);
		}
		
		void Box::_initiatizeOpenGL()
		{
			const std::string shader_name = "Color2D";

			if (_shader == nullptr)
				_shader = jgl::Application::instance()->shader(shader_name);

			if (_shader == nullptr)
				throw std::runtime_error("Error : no shader Color2D in jgl::Application");

			if (_modelBuffer == nullptr)
				_modelBuffer = _shader->buffer("model_space")->copy();
			if (_colorBuffer == nullptr)
				_colorBuffer = _shader->buffer("model_color")->copy();
			if (_elementBuffer == nullptr)
				_elementBuffer = _shader->elementBuffer()->copy();

			_initialized = true;
		}

		void Box::_computeShaderBuffer(jgl::Float p_depth)
		{
			_shader->activate();

			Vector3 vertexContent[8];
			Color colorContent[8];
			UInt elementIndex[12] = { 0, 3, 1, 2, 3, 0, 4, 7, 5, 6, 7, 4 };
			static jgl::Vector2Int delta_pos[4] = {
				jgl::Vector2Int(0, 0),
				jgl::Vector2Int(1, 0),
				jgl::Vector2Int(0, 1),
				jgl::Vector2Int(1, 1)
			};

			for (size_t i = 0; i < 4; i++)
			{
				vertexContent[i] = Vector3(
					jgl::Application::instance()->convertScreenToOpenGL(_anchor + _borderSize + (_size - _borderSize * jgl::Vector2(2, 2)) * delta_pos[i]),
					static_cast<jgl::Float>(p_depth) / jgl::Application::instance()->maxDepth()
				);
				colorContent[i] = _frontgroundColor;
			}
			for (size_t i = 0; i < 4; i++)
			{
				vertexContent[i + 4] = Vector3(
					jgl::Application::instance()->convertScreenToOpenGL(_anchor + _size * delta_pos[i]),
					static_cast<jgl::Float>(p_depth) / jgl::Application::instance()->maxDepth()
				);
				colorContent[i + 4] = _backgroundColor;
			}

			_modelBuffer->send(vertexContent, 8);
			_colorBuffer->send(colorContent, 8);
			_elementBuffer->send(elementIndex, 12);

			_computed = true;
		}

		void Box::_castRender()
		{
			_shader->activate();

			_modelBuffer->activate();
			_colorBuffer->activate();
			_elementBuffer->activate();

			_shader->cast(jgl::Shader::Mode::Triangle, _elementBuffer->size() / sizeof(jgl::UInt));
		}

		void Box::render(jgl::Float p_depth)
		{
			if (_initialized == false)
				_initiatizeOpenGL();

			if (_computed == false)
				_computeShaderBuffer(p_depth);

			_castRender();
		}

		Bool Box::isPointed() const
		{
			Vector2Int pos = jgl::Application::instance()->mouse().pos();

			if (pos.x() < anchor().x() || pos.x() >= anchor().x() + size().x() ||
				pos.y() < anchor().y() || pos.y() >= anchor().y() + size().y())
				return (false);

			return (true);
		}

		void Box::setColor(Color p_backgroundColor, Color p_frontgroundColor)
		{
			_frontgroundColor = p_frontgroundColor;
			_backgroundColor = p_backgroundColor;
			_computed = false;
		}

		void Box::setGeometry(Vector2Int p_anchor, Vector2Int p_size)
		{
			_anchor = p_anchor;
			_size = p_size;
			_usableAnchor = _anchor + _borderSize;
			_usableSize = _size - _borderSize * Vector2Int(2, 2);
			_computed = false;
		}

		void Box::setBorderSize(jgl::Vector2Int p_borderSize)
		{
			_borderSize = p_borderSize;
			_usableAnchor = _anchor + _borderSize;
			_usableSize = _size - _borderSize * Vector2Int(2, 2);
			_computed = false;
		}
	}
}