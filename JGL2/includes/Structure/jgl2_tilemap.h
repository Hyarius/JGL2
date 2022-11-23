#pragma once

#include "Structure/jgl2_application.h"
#include "Structure/jgl2_vector.h"
#include "Structure/jgl2_shader.h"
#include "Structure/jgl2_sprite_sheet.h"

namespace jgl
{
	struct INode
	{
		static const jgl::Short NOT_AFFECTED = 0b0000000000000000;
		static const jgl::Short OBSTACLE = 0b0000000000000000;
		static const jgl::Short NORTH_WALKABLE = 0b0000000000000001;
		static const jgl::Short EAST_WALKABLE = 0b0000000000000010;
		static const jgl::Short SOUTH_WALKABLE = 0b0000000000000100;
		static const jgl::Short WEST_WALKABLE = 0b0000000000001000;
		static const jgl::Short WALKABLE = 0b0000000000001111;
		static jgl::Size_t SIZE;
		static jgl::Vector3 UNIT;
		jgl::ULong id;
		jgl::Vector2 sprite;
		jgl::Char animationSize;
		jgl::Bool autotiled;
		jgl::Short obstacle;

		INode(jgl::ULong p_id, jgl::Vector2Int p_sprite, jgl::Bool p_autotiled, jgl::Short p_obstacle, jgl::Int p_animationSize) :
			id(p_id),
			sprite(p_sprite),
			animationSize(p_animationSize),
			autotiled(p_autotiled),
			obstacle(p_obstacle)
		{

		}
	};

	template <typename TNodeType, const jgl::Size_t NChunkSize, const jgl::Size_t NChunkDepth = 1>
	class IChunk
	{
	public:
		typedef TNodeType NodeType;
		static const jgl::Size_t C_SIZE = NChunkSize;
		static const jgl::Size_t C_DEPTH = NChunkDepth;
		static inline jgl::Short C_OUTSIDE_NODE = -2;

	protected:
		static inline std::vector<TNodeType*> _nodes = {};

		jgl::Vector2Int _pos;
		jgl::Vector2Int _deltaPosToApply;
		jgl::Short _content[C_SIZE][C_SIZE][C_DEPTH];

	public:
		static void addNode(TNodeType* p_node)
		{
			_nodes.push_back(p_node);
		}
		static TNodeType* node(jgl::Size_t p_index)
		{
			if (p_index >= _nodes.size())
				return (nullptr);
			return (_nodes[p_index]);
		}
		static const std::vector<TNodeType*>& nodes()
		{
			return (_nodes);
		}

		IChunk(jgl::Vector2Int p_pos) :
			_pos(p_pos),
			_deltaPosToApply(p_pos* NChunkSize)
		{
			static_assert(NChunkDepth != 0, "Chunk can't have a depth of 0");
			static_assert(std::is_base_of<INode, TNodeType>::value, "Chunk can only allow INode inherenced template");
			for (jgl::Size_t i = 0; i < NChunkSize; i++)
			{
				for (jgl::Size_t j = 0; j < NChunkSize; j++)
				{
					for (jgl::Size_t h = 0; h < NChunkDepth; h++)
					{
						_content[i][j][h] = -1;
					}
				}
			}
		}

		jgl::Vector2Int pos() { return (_pos); }
		jgl::Vector2Int deltaPosToApply() { return (_deltaPosToApply); }

		jgl::Short* content()
		{
			return (&(_content[0][0][0]));
		}

		jgl::Short content(jgl::Vector2Int p_pos)
		{
			return (content(p_pos.x, p_pos.y, 0));
		}

		jgl::Short content(jgl::Vector2Int p_pos, jgl::Int p_z)
		{
			return (content(p_pos.x, p_pos.y, p_z));
		}

		jgl::Short content(jgl::Vector3Int p_pos)
		{
			return (content(p_pos.x, p_pos.y, p_pos.z));
		}

		jgl::Short content(jgl::Int p_x, jgl::Int p_y, jgl::Int p_z)
		{
			if (p_x < 0 || p_x >= C_SIZE || p_y < 0 || p_y >= C_SIZE || p_z < 0 || p_z >= C_DEPTH)
				return C_OUTSIDE_NODE;
			return (_content[p_x][p_y][p_z]);
		}

		virtual void setContent(jgl::Int p_x, jgl::Int p_y, jgl::Int p_z, jgl::Short p_value)
		{
			if (p_x < 0 || p_x >= C_SIZE || p_y < 0 || p_y >= C_SIZE || p_z < 0 || p_z >= C_DEPTH)
				return;
			_content[p_x][p_y][p_z] = p_value;
		}

		void setContent(jgl::Vector2Int p_pos, jgl::Short p_value)
		{
			setContent(p_pos.x, p_pos.y, 0, p_value);
		}


		void setContent(jgl::Vector2Int p_pos, jgl::Int p_depth, jgl::Short p_value)
		{
			setContent(p_pos.x, p_pos.y, p_depth, p_value);
		}

		void setContent(jgl::Vector3Int p_pos, jgl::Short p_value)
		{
			setContent(p_pos.x, p_pos.y, p_pos.z, p_value);
		}
	};

	template<typename TBakableChunkType>
	class ITilemap;

	template <typename TNodeType, const jgl::Size_t NChunkSize, const jgl::Size_t NChunkDepth = 1>
	class IBakableChunk : public IChunk<TNodeType, NChunkSize, NChunkDepth>
	{
	protected:
		struct ShaderData
		{
			static inline std::string C_VERTEX_SHADER_CODE = R"(#version 330 core
layout(location = 0) in vec3 model_space;
layout(location = 1) in vec2 model_uvs;
layout(location = 2) in float animation_sprite_delta;
uniform vec3 delta_model;
uniform int animation_state;
uniform vec2 uvs_unit;
out vec2 UV;
void main()
{
	gl_Position = vec4(model_space + delta_model, 1.0f);
	UV = model_uvs + (vec2(animation_state * animation_sprite_delta, 0) * uvs_unit);
})";
			static inline std::string C_FRAGMENT_SHADER_CODE = R"(#version 330 core
in vec2 UV;
layout(location = 0) out vec4 color;
uniform sampler2D textureID;
void main()
{
	if (UV.x < 0 || UV.x > 1 || UV.y < 0 || UV.y > 1)
	{
		color = vec4(1, 0, 0, 1);
	}
	else
	{
		color = texture(textureID, UV).rgba;
		if (color.a == 0)
			discard;
	}
})";
			static inline jgl::Shader* shader = nullptr;

			jgl::Bool generated = false;

			jgl::Buffer* modelSpaceBuffer = nullptr;
			jgl::Buffer* modelUvsBuffer = nullptr;
			jgl::Buffer* animationSpriteDeltaBuffer = nullptr;
			jgl::Buffer* indexesBuffer = nullptr;

			jgl::Uniform* deltaModelUniform = nullptr;
			jgl::Uniform* animationStateUniform = nullptr;
			jgl::Uniform* uvsUnitUniform = nullptr;
			jgl::Uniform* textureUniform = nullptr;

			void generate()
			{
				if (shader == nullptr)
				{
					shader = jgl::Application::instance()->shader("ChunkShader");
					if (shader == nullptr)
						shader = jgl::Application::instance()->addShader("ChunkShader", C_VERTEX_SHADER_CODE, C_FRAGMENT_SHADER_CODE);
				}

				if (modelSpaceBuffer == nullptr)
					modelSpaceBuffer = shader->buffer("model_space")->copy();
				if (modelUvsBuffer == nullptr)
					modelUvsBuffer = shader->buffer("model_uvs")->copy();
				if (animationSpriteDeltaBuffer == nullptr)
					animationSpriteDeltaBuffer = shader->buffer("animation_sprite_delta")->copy();
				if (indexesBuffer == nullptr)
					indexesBuffer = shader->indexesBuffer()->copy();

				if (deltaModelUniform == nullptr)
					deltaModelUniform = shader->uniform("delta_model")->copy();
				if (animationStateUniform == nullptr)
					animationStateUniform = shader->uniform("animation_state")->copy();
				if (uvsUnitUniform == nullptr)
					uvsUnitUniform = shader->uniform("uvs_unit")->copy();
				if (textureUniform == nullptr)
					textureUniform = shader->uniform("textureID")->copy();

				generated = true;
			}

			void cast(jgl::Vector3 p_offset, jgl::Int p_animationState)
			{
				shader->activate();

				deltaModelUniform->send(p_offset);
				animationStateUniform->send(p_animationState);
				uvsUnitUniform->send(_nodeTexture->unit());
				textureUniform->send(0);

				if (indexesBuffer->size() != 0 && _nodeTexture != nullptr)
				{
					IBakableChunk::nodeTexture()->activate();

					modelSpaceBuffer->activate();
					modelUvsBuffer->activate();
					animationSpriteDeltaBuffer->activate();
					indexesBuffer->activate();

					shader->cast(jgl::Shader::Mode::Triangle, indexesBuffer->size() / sizeof(jgl::Uint));
				}
			}
		};
		std::recursive_mutex _mutex;
		ShaderData _shaderData;
		jgl::Bool _baked;

		IBakableChunk<TNodeType, NChunkSize, NChunkDepth>* _neightbourChunks[3][3];

		static inline jgl::Vector2Int _bakingNeightbourNode[4][3] = {
		{
			jgl::Vector2Int(-1, 0),
			jgl::Vector2Int(-1, -1),
			jgl::Vector2Int(0, -1),
		},
		{
			jgl::Vector2Int(0, 1),
			jgl::Vector2Int(-1, 1),
			jgl::Vector2Int(-1, 0),
		},
		{
			jgl::Vector2Int(0, -1),
			jgl::Vector2Int(1, -1),
			jgl::Vector2Int(1, 0),
		},
		{
			jgl::Vector2Int(1, 0),
			jgl::Vector2Int(1, 1),
			jgl::Vector2Int(0, 1),
		},
		};
		static inline jgl::Vector2Int _bakingDeltaNodeSprite[4][2][2][2] = {
			/*
				if next node == actual -> 0
				else -> 1
			*/
			{// Passe 1
				{// A = 1
					{// B = 1
						jgl::Vector2Int(1, 3), // ABC
						jgl::Vector2Int(1, 2)  // AB
					},
					{// B = 0
						jgl::Vector2Int(2, 0), // A C
						jgl::Vector2Int(1, 2)  // A
					}
				},
				{// A = 0
					{// B = 1
						jgl::Vector2Int(0, 3), //  BC
						jgl::Vector2Int(0, 0)  //  B
					},
					{// B = 0
						jgl::Vector2Int(0, 3), //   C
						jgl::Vector2Int(0, 0)  // 
					}
				}
			},
			{// Passe 2
				{// A = 1
					{// B = 1
						jgl::Vector2Int(1, 4), // ABC
						jgl::Vector2Int(0, 4)  // AB
					},
					{// B = 0
						jgl::Vector2Int(2, 1), // A C
						jgl::Vector2Int(0, 4)  // A
					}
				},
				{// A = 0
					{// B = 1
						jgl::Vector2Int(1, 5), //  BC
						jgl::Vector2Int(0, 1)  //  B
					},
					{// B = 0
						jgl::Vector2Int(1, 5), //   C
						jgl::Vector2Int(0, 1)  // 
					}
				}
			},
			{// Passe 3
				{// A = 1
					{// B = 1
						jgl::Vector2Int(2, 3), // ABC
						jgl::Vector2Int(3, 3)  // AB
					},
					{// B = 0
						jgl::Vector2Int(3, 0), // A C
						jgl::Vector2Int(3, 3)  // A
					}
				},
				{// A = 0
					{// B = 1
						jgl::Vector2Int(2, 2), //  BC
						jgl::Vector2Int(1, 0)  //  B
					},
					{// B = 0
						jgl::Vector2Int(2, 2), //   C
						jgl::Vector2Int(1, 0)  // 
					}
				}
			},
			{// Passe 4
				{// A = 1
					{// B = 1
						jgl::Vector2Int(2, 4), // ABC
						jgl::Vector2Int(2, 5)  // AB
					},
					{// B = 0
						jgl::Vector2Int(3, 1), // A C
						jgl::Vector2Int(2, 5)  // A
					}
				},
				{// A = 0
					{// B = 1
						jgl::Vector2Int(3, 4), //  BC
						jgl::Vector2Int(1, 1)  //  B
					},
					{// B = 0
						jgl::Vector2Int(3, 4), //   C
						jgl::Vector2Int(1, 1)  // 
					}
				}
			},
		};
		static inline jgl::Vector3 _deltaAutotilePosition[4] = {
			jgl::Vector3(0.0f, 0.0f, 0.0f),
			jgl::Vector3(0.0f, 0.5f, 0.0f),
			jgl::Vector3(0.5f, 0.0f, 0.0f),
			jgl::Vector3(0.5f, 0.5f, 0.0f)
		};
		static inline jgl::Vector3 _deltaTilePosition[4] = {
			jgl::Vector3(0.0f, 0.0f, 0.0f),
			jgl::Vector3(0.0f, 1.0f, 0.0f),
			jgl::Vector3(1.0f, 0.0f, 0.0f),
			jgl::Vector3(1.0f, 1.0f, 0.0f)
		};
		static inline jgl::Vector2 _deltaUvs[4] = {
			jgl::Vector2(0, 0),
			jgl::Vector2(0, 1),
			jgl::Vector2(1, 0),
			jgl::Vector2(1, 1)
		};
		static inline jgl::Uint _elementIndex[6] = {
			0, 3, 1, 2, 3, 0
		};
		static inline jgl::SpriteSheet* _nodeTexture = nullptr;
		static inline std::vector<jgl::Vector3> vertexArray;
		static inline std::vector<jgl::Vector2> uvsArray;
		static inline std::vector<jgl::Float> animationSpriteDeltaArray;
		static inline std::vector<jgl::Uint> elementArray;

	protected:
		void _initializeOpenglData()
		{
			if (TNodeType::UNIT == 0.0f)
			{
				TNodeType::UNIT = jgl::Application::instance()->convertScreenToOpenGL(jgl::Vector2Int(TNodeType::SIZE, TNodeType::SIZE), 1) - jgl::Application::instance()->convertScreenToOpenGL(0, 0);
			}
			if (_shaderData.generated == false)
				_shaderData.generate();
		}

		jgl::Vector2Int _calcSubPartSprite(jgl::Int p_x, jgl::Int p_y, jgl::Int p_z, jgl::Size_t p_sub_part)
		{
			jgl::Int firstValue;

			firstValue = this->content(jgl::Vector3Int(p_x, p_y, p_z));

			jgl::Bool values[3] = { false, false, false };
			for (jgl::Size_t j = 0; j < 3; j++)
			{
				jgl::Vector2Int next_pos = jgl::Vector2Int(p_x + _bakingNeightbourNode[p_sub_part][j].x, p_y + _bakingNeightbourNode[p_sub_part][j].y);
				jgl::Int chunk_x = (next_pos.x == -1 ? 0 : (next_pos.x != NChunkSize ? 1 : 2));
				jgl::Int chunk_y = (next_pos.y == -1 ? 0 : (next_pos.y != NChunkSize ? 1 : 2));

				if (next_pos.x == -1)
					next_pos.x = NChunkSize - 1;
				if (next_pos.y == -1)
					next_pos.y = NChunkSize - 1;

				if (next_pos.x == NChunkSize)
					next_pos.x = 0;
				if (next_pos.y == NChunkSize)
					next_pos.y = 0;

				jgl::Int nextValue;

				if (_neightbourChunks[chunk_x][chunk_y] == nullptr)
				{
					nextValue = -1;
				}
				else
				{
					nextValue = _neightbourChunks[chunk_x][chunk_y]->content(next_pos, p_z);
				}
				if (nextValue == -1)
				{
					values[j] = true;
				}
				else
				{
					if (nextValue == firstValue)
					{
						values[j] = false;
					}
					else
						values[j] = true;
				}
			}

			return (_bakingDeltaNodeSprite[p_sub_part][values[0]][values[1]][values[2]]);
		}


		void _bakeAutotile(
			TNodeType* p_node,
			jgl::Int p_x, jgl::Int p_y, jgl::Int p_z)
		{
			for (jgl::Size_t face = 0; face < 4; face++)
			{
				jgl::Vector2Int sprite_value = p_node->sprite + _calcSubPartSprite(p_x, p_y, p_z, face);
				jgl::Uint sprite_id = (_nodeTexture->size().x * sprite_value.y) + sprite_value.x;
				jgl::Vector3 node_pos = jgl::Vector3(p_x, p_y, p_z) + _deltaAutotilePosition[face];
				jgl::Vector2 sprite = _nodeTexture->sprite(sprite_id);
				jgl::Vector2 unit = _nodeTexture->unit();

				jgl::Size_t vertexArrayEntrySize = _vertexArray.size();

				for (size_t i = 0; i < 4; i++)
				{
					_vertexArray.push_back(TNodeType::UNIT * (node_pos + _deltaAutotilePosition[i]));
					_uvsArray.push_back(sprite + unit * _deltaUvs[i]);
				}
				for (jgl::Size_t i = 0; i < 6; i++)
				{
					_elementArray.push_back(_elementIndex[i] + vertexArrayEntrySize);
				}

				for (jgl::Size_t i = 0; i < 4; i++)
					_animationSpriteDeltaArray.push_back(static_cast<jgl::Float>(p_node->animationSize));
			}

		}

		void _bakeTile(
			TNodeType* p_node,
			jgl::Int p_x, jgl::Int p_y, jgl::Int p_z)
		{
			jgl::Uint sprite_id = (_nodeTexture->size().x * p_node->sprite.y) + p_node->sprite.x;
			jgl::Vector3 node_pos = jgl::Vector3(p_x, p_y, p_z);
			jgl::Vector2 sprite = _nodeTexture->sprite(sprite_id);
			jgl::Vector2 unit = _nodeTexture->unit();

			jgl::Size_t vertexArrayEntrySize = _vertexArray.size();

			for (size_t i = 0; i < 4; i++)
			{
				_vertexArray.push_back(TNodeType::UNIT * (node_pos + _deltaTilePosition[i]));
				_uvsArray.push_back(sprite + unit * _deltaUvs[i]);
			}
			for (jgl::Size_t i = 0; i < 6; i++)
			{
				_elementArray.push_back(_elementIndex[i] + vertexArrayEntrySize);
			}
			for (jgl::Size_t i = 0; i < 4; i++)
				_animationSpriteDeltaArray.push_back(static_cast<jgl::Float>(p_node->animationSize));
		}

		void _bakeContent(jgl::Int p_x, jgl::Int p_y, jgl::Int p_z)
		{
			jgl::Short value = this->content(jgl::Vector3Int(p_x, p_y, p_z));

			if (value != -1 && static_cast<jgl::Size_t>(value) < this->_nodes.size())
			{
				TNodeType* tmpNode = this->_nodes[value];

				if (tmpNode->sprite.x >= 0 && tmpNode->sprite.y >= 0)
				{
					if (tmpNode->autotiled == true)
					{
						_bakeAutotile(tmpNode, p_x, p_y, p_z);
					}
					else
					{
						_bakeTile(tmpNode, p_x, p_y, p_z);
					}
				}
			}
		}

	public:
		IBakableChunk(jgl::Vector2Int p_pos) : IChunk<TNodeType, NChunkSize, NChunkDepth>(p_pos),
			_baked(false)
		{
			unbake();
		}

		static void setNodeTexture(jgl::SpriteSheet* p_nodeTexture)
		{
			_nodeTexture = p_nodeTexture;
		}
		static jgl::SpriteSheet* nodeTexture()
		{
			return (_nodeTexture);
		}

		jgl::Bool baked() { return (_baked); }

		void unbake()
		{
			_mutex.lock();
			for (jgl::Int i = -1; i <= 1; i++)
			{
				for (jgl::Int j = -1; j <= 1; j++)
				{
					_neightbourChunks[i + 1][j + 1] = nullptr;
				}
			}
			_baked = false;
			_mutex.unlock();
		}

		template <typename TMapChunkType>
		void bake(ITilemap<TMapChunkType>* p_map, jgl::Bool rebake)
		{
			_mutex.lock();
			_initializeOpenglData();

			if (_nodeTexture == nullptr)
				return;

			for (jgl::Int i = -1; i <= 1; i++)
			{
				for (jgl::Int j = -1; j <= 1; j++)
				{
					if (_neightbourChunks[i + 1][j + 1] == nullptr)
						_neightbourChunks[i + 1][j + 1] = p_map->chunk(jgl::Vector2Int(i, j) + this->_pos);
				}
			}


			vertexArray.clear();
			uvsArray.clear();
			animationSpriteDeltaArray.clear();
			elementArray.clear();

			for (jgl::Size_t i = 0; i < NChunkSize; i++)
				for (jgl::Size_t j = 0; j < NChunkSize; j++)
					for (jgl::Size_t h = 0; h < NChunkDepth; h++)
					{
						_bakeContent(i, j, h);
					}

			_shaderData.modelSpaceBuffer->send(vertexArray.all(), vertexArray.size());
			_shaderData.modelUvsBuffer->send(uvsArray.all(), uvsArray.size());
			_shaderData.animationSpriteDeltaBuffer->send(animationSpriteDeltaArray.all(), animationSpriteDeltaArray.size());
			_shaderData.indexesBuffer->send(elementArray.all(), elementArray.size());

			_baked = true;
			_mutex.unlock();

			if (rebake == true)
			{
				for (jgl::Size_t i = 0; i < 3; i++)
				{
					for (jgl::Size_t j = 0; j < 3; j++)
					{
						if ((i != 1 || j != 1) && _neightbourChunks[i][j] != nullptr)
							_neightbourChunks[i][j]->bake(p_map, false);
					}
				}
			}
		}

		void render(jgl::Vector3 p_offset, jgl::Int p_animationState)
		{
			if (_nodeTexture != nullptr)
			{
				if (_shaderData.generated == true)
					_shaderData.cast(p_offset, p_animationState);
			}
		}
	};

	template<typename TBakableChunkType>
	class ITilemap
	{
	public:
		using ChunkType = TBakableChunkType;
		static const jgl::Short outside_world = -2;

		jgl::Short _emptyObstacle = TBakableChunkType::NodeType::OBSTACLE;
	public:
		virtual jgl::Vector2Int convertWorldToChunk(const jgl::Vector2Int& p_pos)
		{
			jgl::Vector2 result;

			result.x = static_cast<jgl::Float>(p_pos.x) / static_cast<jgl::Float>(TBakableChunkType::C_SIZE);
			result.y = static_cast<jgl::Float>(p_pos.y) / static_cast<jgl::Float>(TBakableChunkType::C_SIZE);

			return (result.floor());
		}

		virtual jgl::Vector2Int convertWorldToChunk(const jgl::Vector3Int& p_pos)
		{
			jgl::Vector2Int converted = jgl::Vector2Int(p_pos.x, p_pos.y);
			return (convertWorldToChunk(converted));
		}

		virtual TBakableChunkType* chunk(jgl::Vector2Int p_pos) = 0;

		void setContent(jgl::Vector2Int p_pos, jgl::Int p_depth, jgl::Short p_value)
		{
			TBakableChunkType* tmp_chunk = this->chunk(this->convertWorldToChunk(p_pos));
			if (tmp_chunk != nullptr)
				tmp_chunk->setContent(p_pos - tmp_chunk->deltaPosToApply(), p_depth, p_value);
		}

		void setContent(jgl::Vector2Int p_pos, jgl::Short p_value)
		{
			setContent(p_pos, 0, p_value);
		}

		void setContent(jgl::Vector3Int p_pos, jgl::Short p_value)
		{
			setContent(jgl::Vector2Int(p_pos.x, p_pos.y), p_pos.z, p_value);
		}

		jgl::Short content(jgl::Vector2Int p_pos, jgl::Int p_depth)
		{
			TBakableChunkType* tmp_chunk = this->chunk(this->convertWorldToChunk(p_pos));
			if (tmp_chunk != nullptr)
				return (tmp_chunk->content(p_pos - tmp_chunk->deltaPosToApply(), p_depth));
			return (outside_world);
		}

		jgl::Short content(jgl::Vector2Int p_pos)
		{
			return (content(p_pos, 0));
		}

		jgl::Short content(jgl::Vector3Int p_pos)
		{
			return (content(jgl::Vector2Int(p_pos.x, p_pos.y), p_pos.z));
		}

		void set_emptyObstacle(jgl::Short p_emptyObstacle)
		{
			_emptyObstacle = p_emptyObstacle;
		}

		virtual jgl::Short _calcAccesPos(TBakableChunkType* p_chunk, const jgl::Vector2Int& p_pos)
		{
			if (p_chunk == nullptr)
				return (TBakableChunkType::NodeType::OBSTACLE);

			jgl::Vector2Int converted_pos = p_pos - p_chunk->deltaPosToApply();

			jgl::Short result = TBakableChunkType::NodeType::OBSTACLE;

			for (jgl::Size_t i = 0; i < TBakableChunkType::C_DEPTH; i++)
			{
				jgl::Short tmp_content = p_chunk->content(converted_pos.x, converted_pos.y, i);

				result += (tmp_content == -1 ? _emptyObstacle : TBakableChunkType::node(tmp_content)->obstacle);
			}

			return (result);
		}

		static inline jgl::Int direction_mask[3][3] = {
			{// x < 0
				TBakableChunkType::NodeType::EAST_WALKABLE | TBakableChunkType::NodeType::SOUTH_WALKABLE,	// x < 0 && y < 0
				TBakableChunkType::NodeType::EAST_WALKABLE | TBakableChunkType::NodeType::NOT_AFFECTED,	// x < 0 && y == 0
				TBakableChunkType::NodeType::EAST_WALKABLE | TBakableChunkType::NodeType::NORTH_WALKABLE	// x < 0 && y > 0
			},
			{// x == 0
				TBakableChunkType::NodeType::NOT_AFFECTED | TBakableChunkType::NodeType::SOUTH_WALKABLE,	// x == 0 && y < 0
				TBakableChunkType::NodeType::WALKABLE | TBakableChunkType::NodeType::WALKABLE,	// x == 0 && y == 0
				TBakableChunkType::NodeType::NOT_AFFECTED | TBakableChunkType::NodeType::NORTH_WALKABLE	// x == 0 && y > 0
			},
			{ // x > 0
				TBakableChunkType::NodeType::WEST_WALKABLE | TBakableChunkType::NodeType::SOUTH_WALKABLE,	// x > 0 && y < 0
				TBakableChunkType::NodeType::WEST_WALKABLE | TBakableChunkType::NodeType::NOT_AFFECTED,	// x > 0 && y == 0
				TBakableChunkType::NodeType::WEST_WALKABLE | TBakableChunkType::NodeType::NORTH_WALKABLE	// x > 0 && y > 0
			},
		};

		static inline jgl::Int rev_direction_mask[3][3] = {
			{// x < 0
				TBakableChunkType::NodeType::WEST_WALKABLE | TBakableChunkType::NodeType::NORTH_WALKABLE,	// x < 0 && y < 0
				TBakableChunkType::NodeType::WEST_WALKABLE | TBakableChunkType::NodeType::NOT_AFFECTED,	// x < 0 && y == 0
				TBakableChunkType::NodeType::WEST_WALKABLE | TBakableChunkType::NodeType::SOUTH_WALKABLE	// x < 0 && y > 0
			},
			{// x == 0
				TBakableChunkType::NodeType::NOT_AFFECTED | TBakableChunkType::NodeType::NORTH_WALKABLE,	// x == 0 && y < 0
				TBakableChunkType::NodeType::WALKABLE | TBakableChunkType::NodeType::WALKABLE,	// x == 0 && y == 0
				TBakableChunkType::NodeType::NOT_AFFECTED | TBakableChunkType::NodeType::SOUTH_WALKABLE	// x == 0 && y > 0
			},
			{ // x > 0
				TBakableChunkType::NodeType::EAST_WALKABLE | TBakableChunkType::NodeType::NORTH_WALKABLE,	// x > 0 && y < 0
				TBakableChunkType::NodeType::EAST_WALKABLE | TBakableChunkType::NodeType::NOT_AFFECTED,	// x > 0 && y == 0
				TBakableChunkType::NodeType::EAST_WALKABLE | TBakableChunkType::NodeType::SOUTH_WALKABLE	// x > 0 && y > 0
			},
		};

		virtual jgl::Bool canAcces(const jgl::Vector2& p_pos, const jgl::Vector2& p_direction, TBakableChunkType* p_actual_chunk = nullptr, TBakableChunkType* p_destination_chunk = nullptr)
		{
			if (p_destination_chunk == nullptr)
				p_destination_chunk = this->chunk(this->convertWorldToChunk(p_pos + p_direction));

			jgl::Short destinationValue = _calcAccesPos(p_destination_chunk, p_pos + p_direction);

			if (destinationValue == TBakableChunkType::NodeType::OBSTACLE)
				return (false);

			jgl::Size_t index[2] = { (p_direction.x < 0 ? 0u : (p_direction.x == 0 ? 1u : 2u)), (p_direction.y < 0 ? 0u : (p_direction.y == 0 ? 1u : 2u)) };

			if (index[0] == 1 && index[1] == 1)
				return (!(destinationValue == TBakableChunkType::NodeType::OBSTACLE));

			if (p_actual_chunk == nullptr)
				p_actual_chunk = this->chunk(convertWorldToChunk(p_pos));

			jgl::Short actualValue = _calcAccesPos(p_actual_chunk, p_pos);

			if ((actualValue & direction_mask[index[0]][index[1]]) != direction_mask[index[0]][index[1]] ||
				(destinationValue & rev_direction_mask[index[0]][index[1]]) != rev_direction_mask[index[0]][index[1]])
				return (false);
			return (true);
		}

	public:
	};

	template<typename TBakableChunkType, const jgl::Size_t NSize_x, const jgl::Size_t NSize_y>
	class FiniteTilemap : public ITilemap <TBakableChunkType>
	{
	public:
		static const jgl::Size_t C_SIZE_X = NSize_x;
		static const jgl::Size_t C_SIZE_Y = NSize_y;

	protected:
		TBakableChunkType* _chunks[C_SIZE_X][C_SIZE_Y];

		static inline jgl::Vector2Int _converterWorldToChunk[C_SIZE_X * TBakableChunkType::C_SIZE][C_SIZE_Y * TBakableChunkType::C_SIZE];

	public:
		jgl::Vector2Int convertWorldToChunk(const jgl::Vector2Int& p_pos)
		{
			if (p_pos.x < 0 || p_pos.y < 0 || p_pos.x >= C_SIZE_X * TBakableChunkType::C_SIZE || p_pos.y >= C_SIZE_Y * TBakableChunkType::C_SIZE)
				return (-1);

			if (_converterWorldToChunk[p_pos.x][p_pos.y].x == -1 && _converterWorldToChunk[p_pos.x][p_pos.y].y == -1)
			{
				_converterWorldToChunk[p_pos.x][p_pos.y].x = static_cast<jgl::Float>(p_pos.x) / static_cast<jgl::Float>(TBakableChunkType::C_SIZE);
				_converterWorldToChunk[p_pos.x][p_pos.y].y = static_cast<jgl::Float>(p_pos.y) / static_cast<jgl::Float>(TBakableChunkType::C_SIZE);
			}

			return (_converterWorldToChunk[p_pos.x][p_pos.y]);
		}

		FiniteTilemap()
		{
			for (jgl::Size_t i = 0; i < C_SIZE_X; i++)
				for (jgl::Size_t j = 0; j < C_SIZE_Y; j++)
				{
					_chunks[i][j] = nullptr;
				}


			for (jgl::Size_t j = 0; j < C_SIZE_Y; j++)
			{
				for (jgl::Size_t i = 0; i < C_SIZE_X; i++)
				{
					jgl::Vector2Int chunk_pos = jgl::Vector2Int(i, j);

					for (jgl::Size_t x = 0; x < TBakableChunkType::C_SIZE; x++)
					{
						for (jgl::Size_t y = 0; y < TBakableChunkType::C_SIZE; y++)
						{
							_converterWorldToChunk[chunk_pos.x * TBakableChunkType::C_SIZE + x][chunk_pos.y * TBakableChunkType::C_SIZE + y] = -1;
						}
					}
				}
			}
		}

		void unbake()
		{
			TBakableChunkType::NodeType::UNIT = 0;
			for (jgl::Size_t i = 0; i < C_SIZE_X; i++)
				for (jgl::Size_t j = 0; j < C_SIZE_Y; j++)
				{
					if (_chunks[i][j] != nullptr)
						_chunks[i][j]->unbake();
				}
		}

		TBakableChunkType* requestChunk(jgl::Vector2Int p_pos)
		{
			if (p_pos.x >= C_SIZE_X || p_pos.y >= C_SIZE_Y)
				return (nullptr);
			else if (_chunks[p_pos.x][p_pos.y] == nullptr)
				_chunks[p_pos.x][p_pos.y] = new TBakableChunkType(p_pos);
			return (_chunks[p_pos.x][p_pos.y]);
		}

		TBakableChunkType* chunk(jgl::Vector2Int p_pos)
		{
			if (p_pos.x >= C_SIZE_X || p_pos.y >= C_SIZE_Y)
				return (nullptr);
			return (_chunks[p_pos.x][p_pos.y]);
		}

		void addChunk(jgl::Vector2Int p_pos, TBakableChunkType* p_chunk)
		{
			if (p_pos.x >= C_SIZE_X || p_pos.y >= C_SIZE_Y)
				return;
			else if (_chunks[p_pos.x][p_pos.y] != nullptr)
				delete _chunks[p_pos.x][p_pos.y];
			_chunks[p_pos.x][p_pos.y] = p_chunk;
		}
	};


	template<typename TBakableChunkType>
	class Tilemap : public ITilemap <TBakableChunkType>
	{
	protected:
		std::map<jgl::Vector2Int, TBakableChunkType*> _chunks;

	public:
		Tilemap()
		{

		}

		void unbake()
		{
			TBakableChunkType::NodeType::UNIT = 0;
			for (auto tmp : _chunks)
			{
				tmp.second->unbake();
			}
		}

		TBakableChunkType* requestChunk(jgl::Vector2Int p_pos)
		{
			if (_chunks.count(p_pos) == 0)
				_chunks[p_pos] = new TBakableChunkType(p_pos);
			return (_chunks[p_pos]);
		}

		TBakableChunkType* chunk(jgl::Vector2Int p_pos)
		{
			if (_chunks.count(p_pos) == 0)
				return (nullptr);
			return (_chunks[p_pos]);
		}

		void addChunk(jgl::Vector2Int p_pos, TBakableChunkType* p_chunk)
		{
			if (_chunks.count(p_pos) != 0)
				delete _chunks[p_pos];
			_chunks[p_pos] = p_chunk;
		}
	};

}