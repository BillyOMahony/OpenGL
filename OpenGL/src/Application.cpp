#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 *	Compiles a shader of a specified type.
 *	@param source - Source code of the shader.
 *	@param type - Type of shader (Fragment or Vertex)
 */
static unsigned int CompileShader(GLenum type, const std::string& source)
{
	// Creates a shader object 
	unsigned int id = glCreateShader(type);
	// Gets a pointer to the beginning of shader source code.
	const char* src = source.c_str();
	// Assigns shader source code so shader
	glShaderSource(id, 1, &src, nullptr);
	// Compile the shader
	glCompileShader(id);

	// Check compilation status of the shader
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		// Get the error message...
		// but first get the length of the error message
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char)); // This because char message[length] doesn't work because c++....
		glGetShaderInfoLog(id, length, &length, message);
		
		std::cout << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
				  << "Shader Compilation Error: " 
				  << message 
				  << std::endl;

		// Delete the shader because it's broken
		glDeleteShader(id);
		return 0;
	}

	// return the compiled shader
	return id;
}

/**
 *	Creates a new shader from the supplied shader source code.
 *	@param vertexShader the source code for the vertex shader.
 *	@param fragmentShader the source code for the fragment shader.
 */
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// Get a program from OpenGL, this will be run later
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// Link the two shaders to the shader program
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	/*
	// This is not entirely necessary and optimization gains from it are minimal
	// once linked detach the shaders
	glDetachShader(program, vs);
	glDetachShader(program, fs);
	*/

	// Validate the program
	glValidateProgram(program);

	// Delete the shaders, these are no longer necessary
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		std::cout << "Could not initialise OpenGL" << std::endl;
		std::cin.get();
		return -1;
	}
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(500, 500, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Glew Code, this code allows access to graphics card's OpenGL functions */
	if (glewInit() != GLEW_OK) {
		// GLEW did not start correctly
		std::cout << "Glew Not OK" << std::endl;
		std::cin.get();
		return -1;
	}

	std::cout << "OpenGL has found: " << glGetString(GL_VERSION) << std::endl;

	// Triangle Vertex locations. 
	float triangleVertexPositions[6] = 
	{
		-.5f, -.5f,
		 0.f,  .5f, 
		 .5f, -.5f
	};

	// Create buffer
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	// Select the buffer, as in we're about to work on this buffer
	// Bind to what you want to draw on, think of layers in photoshop
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// Initialize buffer and give it data
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), triangleVertexPositions, GL_STATIC_DRAW);

	// Tell OpenGL The layout of out vertex buffer.
	// Enable drawing of Vertex
	glEnableVertexAttribArray(0);
	// Size = num points per vertex (two in this case).
	// Stride = length in bytes from the start of one vertex to start of the next.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	std::string vertexShader = 
		"#version 330 core							\n"
		"											\n"
		"layout(location = 0) in vec4 position;		\n"
		"											\n"
		"void main()								\n"
		"{											\n"
		"	gl_Position = position;					\n"
		"}											\n";

	std::string fragmentShader = 
		"#version 330 core							\n"
		"											\n"
		"layout(location = 0) out vec4 color;		\n"
		"											\n"
		"void main()								\n"
		"{											\n"
		"	color = vec4(1.0, 0.0, 0.0, 1.0);		\n"
		"}											\n";

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}