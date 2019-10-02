#pragma once

#include "Component.h"

struct ShaderData
{
	std::string vextexPath, fragmentPath;
};

class ShaderComponent : public Component
{
public:
	ShaderComponent(ShaderData shaderData)
	{
		this->shaderData = shaderData;
	}

	~ShaderComponent()
	{
		delete vertex;
		delete fragment;
	}


	bool Initialise() override
	{
		std::ifstream vertexFile, fragmentFile;

		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vertexFile.open(shaderData.vextexPath);
			fragmentFile.open(shaderData.fragmentPath);

			std::stringstream vertexStream, fragmentStream;

			vertexStream << vertexFile.rdbuf();
			fragmentStream << fragmentFile.rdbuf();

			vertexFile.close();
			fragmentFile.close();

			vertex = vertexStream.str().c_str();
			fragment = fragmentStream.str().c_str();
		}
		catch (std::ifstream::failure e) {}
	}

	bool CanCompile()
	{
		unsigned int vertexShader, fragmentShader;

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertex, NULL);
		glCompileShader(vertexShader);

		unsigned int vertexShader, fragmentShader;

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragment, NULL);
		glCompileShader(fragmentShader);
	
		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return false;
	}

	void Activate()
	{
		glUseProgram(program);
	}

	unsigned int GetProgram()
	{
		return program;
	}

private:
	GLuint program;
	const char * vertex;
	const char * fragment;
	ShaderData shaderData;
};