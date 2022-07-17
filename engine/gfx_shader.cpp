#include "gfx_shader.h"
#include <GL/glew.h>

static u32 CompileShader(const char *source, u32 shaderType)
{
	// Creating shader
	u32 id = glCreateShader(shaderType);
	glShaderSource(id, 1, &source, NULL);
	glCompileShader(id);

	// Handler compiling errors
	int success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if(!success) {
		char infoLog[512];
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		Panic(infoLog);
	}

	return id;
}

gfx::Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
	// Load shaders source from file
	const char *vertexSource = LoadFileText(vertexPath);
	const char *fragmentSource = LoadFileText(fragmentPath);

	// Compile shaders
	u32 vertexId = CompileShader(vertexSource, GL_VERTEX_SHADER);
	u32 fragmentId = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

	// Create program
	id = glCreateProgram();
	glAttachShader(id, vertexId);
	glAttachShader(id, fragmentId);
	glLinkProgram(id);

	// Handler linking errors
	int success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if(!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		Panic(infoLog);
	}

	// These shaders are now unnecessary
	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
}

void gfx::Shader::Bind()
{
	glUseProgram(id);
}

void gfx::Shader::Delete()
{
	glDeleteProgram(id);
}

void gfx::Shader::SendInt(const char *name, int value)
{
	Bind();
	glUniform1i(glGetUniformLocation(id, name), value);
}

void gfx::Shader::SendFloat4(const char *name, f32 a, f32 b, f32 c, f32 d)
{
	Bind();
	glUniform4f(glGetUniformLocation(id, name), a, b, c, d);
}

void gfx::Shader::SendMat4(const char *name, float *elements)
{
	Bind();
	glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, elements);
}