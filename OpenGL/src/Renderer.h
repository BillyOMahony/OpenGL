#pragma once

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

class Shader;
class VertexArray;
class IndexBuffer;
/**
 *	Clears all errors
 */
void GLClearError();

/**
 *	Checks for and prints OpenGL errors
 */
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{

public:
	/**
	 * Drawn the specified Vertex Array, Index Buffer and Shader
	 * @param va The VertexArray to draw
	 * @param ib The IndexBuffer containing shape and material information
	 * @param shader The shader object
	 */
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);


};