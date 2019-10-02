#pragma once

#include "Object.h"

//Components
#include "TransformComponent.h"
#include "ShaderComponent.h"

struct Character {
	GLuint id, advance;
	glm::ivec2 size, brearing;
};

class TextComponent : public Component
{
public:
	TextComponent(std::string text, std::string fontPath, GLuint fontSize)
	{
		this->text = text;
		this->fontPath = fontPath;
		this->fontSize = fontSize;
	}

	bool Initialise() override
	{
		if (FT_Init_FreeType(&library)) return false;
		if (FT_New_Face(library, fontPath.c_str(), 0, &face)) return false;

		FT_Set_Pixel_Sizes(face, 0, fontSize);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (GLubyte c = 0; c < 128; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
			
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
		shader = &object->GetComponent<ShaderComponent>();

		return shader->CanCompile();
	}

	void Render() override
	{
		shader->Activate();

		glUniform3f(glGetUniformLocation(shader->GetProgram(), "textColor"), color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];

			GLfloat xpos = transform->GetPosition().x + ch.brearing.x;
			GLfloat ypos = transform->GetPosition().y - (ch.size.y - ch.brearing.y);

			GLfloat w = ch.size.x;
			GLfloat h = ch.size.y;

			GLfloat vertices[6][4] = {
				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos,     ypos,       0.0, 1.0 },
				{ xpos + w, ypos,       1.0, 1.0 },

				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos + w, ypos,       1.0, 1.0 },
				{ xpos + w, ypos + h,   1.0, 0.0 }
			};
			glBindTexture(GL_TEXTURE_2D, ch.id);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			transform->Translate(glm::vec2(ch.advance >> 6, 0.0f));
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void SetText(std::string text) 
	{
		this->text = text;
	}

	void SetColor(std::string text)
	{
		this->text = text;
	}

private:
	std::string fontPath, text;
	TransformComponent* transform;
	ShaderComponent* shader;

	//Freetype
	FT_Library library;
	FT_Face face;

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
	GLuint fontSize, VAO, VBO;
	std::map<GLchar, Character> Characters;
};