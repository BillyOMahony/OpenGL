#include "Shader.h"
#include "Renderer.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string & filepath)
	: m_Filepath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}
	GLCall(unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1) {
		std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
	}

	m_UniformLocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) const
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		// search for #shader, this is a custom token
		if (line.find("#shader") != std::string::npos)
		{
			// If the line contains #shader, check what type of shader it is and set the mode
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			// above we are declaring two stringstreams
			// we also know ShaderType::VERTEX = 0 and ShaderType::FRAGMENT = 1
			// so we use the enum's int value to differentiate between the two string streams
			ss[(int)type] << line << '\n';
		}
	}

	ShaderProgramSource shaderSource;
	shaderSource.VertexSource = ss[(int)ShaderType::VERTEX].str();
	shaderSource.FragmentSource = ss[(int)ShaderType::FRAGMENT].str();

	return shaderSource;
}

unsigned int Shader::CompileShader(GLenum type, const std::string& source)
{
	// Creates a shader object 
	GLCall(unsigned int id = glCreateShader(type));
	// Gets a pointer to the beginning of shader source code.
	const char* src = source.c_str();
	// Assigns shader source code so shader
	GLCall(glShaderSource(id, 1, &src, nullptr));
	// Compile the shader
	GLCall(glCompileShader(id));

	// Check compilation status of the shader
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		// Get the error message...
		// but first get the length of the error message
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char)); // This because char message[length] doesn't work because c++....
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
			<< " Shader Compilation Error: "
			<< message
			<< std::endl;

		// Delete the shader because it's broken
		GLCall(glDeleteShader(id));
		return 0;
	}

	// return the compiled shader
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// Get a program from OpenGL, this will be run later
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// Link the two shaders to the shader program
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));

	// Validate the program
	GLCall(glValidateProgram(program));

	// Delete the shaders, these are no longer necessary
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}