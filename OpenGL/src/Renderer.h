#pragma once

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/**
 *	Clears all errors
 */
void GLClearError();

/**
 *	Checks for and prints OpenGL errors
 */
bool GLLogCall(const char* function, const char* file, int line);