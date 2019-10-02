#pragma once

#include "Transform.h"
#include "Object.h"

struct Character {
	GLuint id, advance;
	glm::ivec2 size, brearing;
};

class TextComponent : public Component
{
public:
	TextComponent(std::string fontPath, GLuint fontSize)
	{
		this->fontPath = fontPath;
		this->fontSize = fontSize;
	}

	bool Initialise() override
	{
		if (FT_Init_FreeType(&library)) return false;
		if (FT_New_Face(library, fontPath.c_str(), 0, &face)) return false;

		FT_Set_Pixel_Sizes(face, 0, fontSize);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		for (GLubyte c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);

			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				texture, face->glyph->advance.x,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			};
			Characters.insert(std::pair<GLchar, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		FT_Done_Face(face);
		FT_Done_FreeType(library);

		transform = &object->GetComponent<TransformComponent>();
		return true;
	}

	void Render() override {

	}

private:
	std::string fontPath;
	TransformComponent* transform;
	ShaderComponent* transform;

	//Freetype
	FT_Library library;
	FT_Face face;

	GLuint fontSize, VAO, VBO;
	std::map<GLchar, Character> Characters;
};