#include <memory>
#include <array>
#include <iostream>
#include <algorithm>

#include "Renderer.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <FreeType/freetype-gl/freetype-gl.h>

#include "Game/Game Entities/Entity.h"

#define maxQuads 10000
#define maxVerticies maxQuads * 4
#define maxIndicies maxQuads * 6
#define maxTextureSlots 31

unsigned int Renderer::m_VertexBuffer;
unsigned int Renderer::m_IndexBuffer;
unsigned int Renderer::m_VertexArray;

std::shared_ptr<Shader> Renderer::m_Shader;

std::array<std::shared_ptr<Texture>, 32> Renderer::m_Textures;
unsigned int Renderer::m_NoOfTextures = 1;

VertexData* Renderer::m_VertexData;
VertexData* Renderer::m_BasePointer;

unsigned int Renderer::m_IndiciesBuffer;

static std::array<glm::vec2, 4> UV = { glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
									   glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f) };
static std::array<glm::vec4, 4> VertexPosition = { glm::vec4(-0.5f,  -0.5f, 0.0f, 1.0f), glm::vec4( 0.5f,  -0.5f, 0.0f, 1.0f) ,
												   glm::vec4( 0.5f,   0.5f, 0.0f, 1.0f),  glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f) };


void Renderer::Init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	unsigned int* indicies = new unsigned int[maxIndicies];

	m_Shader = std::make_shared<Shader>("Resources/Shaders/RendererShader.glsl");

	unsigned int offset = 0;
	for (unsigned int i = 0; i < maxIndicies; i += 6)
	{
		indicies[i + 0] = offset + 0;
		indicies[i + 1] = offset + 1;
		indicies[i + 2] = offset + 2;

		indicies[i + 3] = offset + 2;
		indicies[i + 4] = offset + 3;
		indicies[i + 5] = offset + 0;

		offset += 4;
	}

	glCreateBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndicies * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

	delete[] indicies;
	
	glCreateBuffers(1, &m_VertexBuffer);

	glCreateVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxVerticies * sizeof(VertexData), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)offsetof(VertexData, TexCoords));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)offsetof(VertexData, Color));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)offsetof(VertexData, TextureIndex));

	m_BasePointer = new VertexData[maxVerticies];

	int sample[31];
	for (int i = 0; i < maxTextureSlots; i++)
		sample[i] = i;

	m_Shader->setUniformIntArray("u_Tex", sample, maxTextureSlots);

	m_Shader->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::beginBatch()
{
	m_VertexData = m_BasePointer;
	m_NoOfTextures = 1;
	m_IndiciesBuffer = 0;
}

void Renderer::endBatch()
{
	unsigned int size = (uint32_t)((uint8_t*)m_VertexData - (uint8_t*)m_BasePointer); 
	glBindVertexArray(m_VertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_BasePointer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

	for (int index = 1; index < m_NoOfTextures; index++)
	{
		m_Textures[index]->Bind(index);
	}

	m_Shader->Bind();

	glDrawElements(GL_TRIANGLES, m_IndiciesBuffer, GL_UNSIGNED_INT, nullptr);
}

void Renderer::nextBatch()
{
	endBatch();
	beginBatch();
}

void Renderer::beginScene(std::shared_ptr<Camera>& cam)
{
	m_Shader->Bind();
	m_Shader->setUniformMat4("u_Orthographic", cam->getMatrix());
	beginBatch();
}

void Renderer::beginScene(const glm::mat4& mat)
{
	m_Shader->Bind();
	m_Shader->setUniformMat4("u_Orthographic", mat);
	beginBatch();
}

void Renderer::endScene()
{
	endBatch();
}

void Renderer::clear(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawQuad(const std::shared_ptr<Texture>& tex, const glm::vec4& color, const glm::vec3& pos, const glm::vec3& scale, float rotation)
{
	float texIndex = 0.0f;

	glm::mat4 model = glm::translate(glm::mat4(1.0f), pos) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(glm::mat4(1.0f), scale);

	checkBatch(tex, texIndex);

	for (int index = 0; index < 4; index++)
	{
		m_VertexData->Position = model * VertexPosition[index];
		m_VertexData->TexCoords = UV[index];
		m_VertexData->Color = color;
		m_VertexData->TextureIndex = texIndex;
		m_VertexData++;
	}
	m_IndiciesBuffer += 6;
}

void Renderer::drawEntity(const std::shared_ptr<Entity>& ent)
{
	float texIndex = 0.0f;

	checkBatch(ent->m_Animations[(int)ent->m_WorldState][(int)ent->m_CurrentAnimation].getSprite(), texIndex);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), ent->m_Position) * glm::rotate(glm::mat4(1.0f), ent->m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(glm::mat4(1.0f), ent->m_Scale);

	for (int index = 0; index < 4; index++)
	{
		m_VertexData->Position = model * VertexPosition[index];
		m_VertexData->TexCoords = ent->m_Animations[(int)ent->m_WorldState][(int)ent->m_CurrentAnimation].getCurrentFrame()[index];
		m_VertexData->Color = ent->m_Color;
		m_VertexData->TextureIndex = texIndex;
		m_VertexData++;
	}

	m_IndiciesBuffer += 6;
}

void Renderer::drawQuad(const glm::vec4& color, const glm::vec3& pos, const glm::vec3& scale, float rotation)
{
	float texIndex = 0.0f;

	glm::mat4 model = glm::translate(glm::mat4(1.0f), pos) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(glm::mat4(1.0f), scale);

	for (int index = 0; index < 4; index++)
	{
		m_VertexData->Position = model * VertexPosition[index];
		m_VertexData->TexCoords = UV[index];
		m_VertexData->Color = color;
		m_VertexData->TextureIndex = texIndex;
		m_VertexData++;
	}
	m_IndiciesBuffer += 6;
}

void Renderer::drawQuad(const std::shared_ptr<Texture>& tex, std::array<glm::vec2, 4> uvs, const glm::vec4& color, const glm::vec3& pos, const glm::vec3& scale, float rotation)
{
	float texIndex = 0.0f;

	glm::mat4 model = glm::translate(glm::mat4(1.0f), pos) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(glm::mat4(1.0f), scale);

	checkBatch(tex, texIndex);

	for (int index = 0; index < 4; index++)
	{
		m_VertexData->Position = model * VertexPosition[index];
		m_VertexData->TexCoords = uvs[index];
		m_VertexData->Color = color;
		m_VertexData->TextureIndex = texIndex;
		m_VertexData++;
	}
	m_IndiciesBuffer += 6;
}

void Renderer::drawString(const std::string& string, const std::shared_ptr<Font>& font, const glm::vec3& pos, const glm::vec3& sca, const glm::vec4& color)
{
	auto scale = font->getSize();

	float width = sca.x / scale;
	float height = sca.y / scale;

	float x = pos.x;
	float texIndex = 0.0f;

	checkBatch(font->getTexture(), texIndex);

	auto* ttf = font->getFont();

	for (int index = 0; index != string.size(); index++)
	{
		char c = string[index];
		ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(ttf, c);

		if (glyph != nullptr)
		{
			float kerning = 0.0f;
			if (index > 0)
				kerning = ftgl::texture_glyph_get_kerning(glyph, string[index - 1]);

			float x0 = x + glyph->offset_x * width;
			float y0 = pos.y + glyph->offset_y * height;
			float x1 = x0 + glyph->width * width;
			float y1 = y0 - glyph->height * height;
			float s0 = glyph->s0;
			float t0 = glyph->t0;
			float s1 = glyph->s1;
			float t1 = glyph->t1;

			m_VertexData->Position = glm::vec4(x0, y0, 0, 1.0f);
			m_VertexData->TexCoords = glm::vec2(s0, t0);
			m_VertexData->Color = color;
			m_VertexData->TextureIndex = texIndex;
			m_VertexData++;

			m_VertexData->Position = glm::vec4(x0, y1, 0, 1.0f);
			m_VertexData->TexCoords = glm::vec2(s0, t1);
			m_VertexData->Color = color;
			m_VertexData->TextureIndex = texIndex;
			m_VertexData++;

			m_VertexData->Position = glm::vec4(x1, y1, 0, 1.0f);
			m_VertexData->TexCoords = glm::vec2(s1, t1);
			m_VertexData->Color = color;
			m_VertexData->TextureIndex = texIndex;
			m_VertexData++;

			m_VertexData->Position = glm::vec4(x1, y0, 0, 1.0f);
			m_VertexData->TexCoords = glm::vec2(s1, t0);
			m_VertexData->Color = color;
			m_VertexData->TextureIndex = texIndex;
			m_VertexData++;

			x += glyph->advance_x * width;
			m_IndiciesBuffer += 6;
		}
	}
}

void Renderer::checkBatch(const std::shared_ptr<Texture>& tex, float& index)
{
	if (m_IndiciesBuffer >= maxIndicies)
		nextBatch();

	if (m_NoOfTextures == 1)
	{
		m_Textures[m_NoOfTextures] = tex;
		index = m_NoOfTextures;
		m_NoOfTextures++;
		return;
	}
	else
	{
		if (m_NoOfTextures > 31)
		{
			nextBatch();
			return;
		}

		for (int i = 1; i < m_NoOfTextures; i++)
			if (*m_Textures[i] == *tex)
			{
				index = i;
				goto END;
			}

		m_Textures[m_NoOfTextures] = tex;
		index = m_NoOfTextures;
		m_NoOfTextures++;
	}

END:
	return;
}

