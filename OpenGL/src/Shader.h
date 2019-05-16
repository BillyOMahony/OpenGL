#pragma once
#include <string>
#include <GL/glew.h>
#include <unordered_map> // hash map

/**
 *	Contains source code for the Vertex and Fragment shaders.
 */
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:

	Shader(const std::string& filepath);

	~Shader();

	/**
	 * Binds this shader for use with the renderer
	 */
	void Bind() const;

	/**
	 * Unbinds this shader from the renderer
	 */
	void UnBind() const;

	/**
	 *	Sets a colour (RGBA) for this shader.
	 */
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	unsigned int m_RendererID;

	std::string m_Filepath;
	
	// caching for uniforms
	std::unordered_map<std::string, unsigned int> m_UniformLocationCache;


	/**
	 *	@return the location of the colour value (RGBA) for this shader
	 */
	unsigned int GetUniformLocation(const std::string& name);

	/**
	 * Parses the shader file found at filepath into a Vertex and Fragment shader
	 * @param filepath path to the shader
	 * @return a struct containing the vertex and fragment shaders
	 */
	ShaderProgramSource ParseShader(const std::string& filepath) const;

	/**
	 *	Compiles a shader of a specified type.
	 *	@param source - Source code of the shader.
	 *	@param type - Type of shader (Fragment or Vertex)
	 *	@return true if shader compiled successfully
	 */
	unsigned int CompileShader(GLenum type, const std::string& source);

	/**
	 *	Creates a new shader from the supplied shader source code.
	 *	@param vertexShader the source code for the vertex shader.
	 *	@param fragmentShader the source code for the fragment shader.
	 */
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};
