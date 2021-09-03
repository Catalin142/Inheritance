#pragma once
#include "Shader.h"
#include "glm/glm.hpp"
#include "Texture.h"
#include "Engine/Utils/Camera.h"
#include "Engine/Utils/Font.h"

#include <array>

struct VertexData
{
	glm::vec4 Position;
	glm::vec2 TexCoords;
	glm::vec4 Color;
	float TextureIndex;
};

class Renderer
{
public:
	static void Init();

	static void beginBatch();
	static void endBatch();
	static void nextBatch();

	static void beginScene(std::shared_ptr<Camera>& cam);
	static void beginScene(const glm::mat4& mat);
	static void endScene();

	static void clear(float r, float g, float b);

	static void drawQuad(const std::shared_ptr<Texture>& tex, const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f), float rotation = 0.0f);

	static void drawEntity(const std::shared_ptr<Entity>& ent);

	static void drawQuad(const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f), float rotation = 0.0f);

	static void drawQuad(const std::shared_ptr<Texture>& tex, std::array<glm::vec2, 4> uvs, const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f), float rotation = 0.0f);

	static void drawString(const std::string& string, const std::shared_ptr<Font>& font, const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 0.0f),
		const  glm::vec3& sca = glm::vec3(1.0f, 1.0f, 1.0f), const  glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

private:
	static unsigned int m_VertexBuffer;
	static unsigned int m_IndexBuffer;
	static unsigned int m_VertexArray;
	
	static std::shared_ptr<Shader> m_Shader;

	static std::array<std::shared_ptr<Texture>, 32> m_Textures;
	static unsigned int m_NoOfTextures;

	static VertexData* m_VertexData;
	static VertexData* m_BasePointer;

	static unsigned int m_IndiciesBuffer;

private:
	Renderer() = default;
	~Renderer() = default;

	static void checkBatch(const std::shared_ptr<Texture>& tex, float& index);
};