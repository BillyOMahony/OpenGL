#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/**
 *	Contains source code for the Vertex and Fragment shaders.
 */
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

/**
 * Parses the shader file found at filepath into a Vertex and Fragment shader
 * @param filepath path to the shader
 * @return a struct containing the vertex and fragment shaders
 */
static ShaderProgramSource ParseShader(const std::string& filepath)
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
		if(line.find("#shader") != std::string::npos)
		{
			// If the line contains #shader, check what type of shader it is and set the mode
			if(line.find("vertex") != std::string::npos)
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
		
		std::cout << (type == GL_VERTEX_SHADER ?  "Vertex"  :  "Fragment" )
				  <<  " Shader Compilation Error: "  
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
		std::cout <<  "Could not initialise OpenGL"  << std::endl;
		std::cin.get();
		return -1;
	}
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(500, 500,  "Hello World" , NULL, NULL);
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
		std::cout <<  "Glew Not OK"  << std::endl;
		std::cin.get();
		return -1;
	}

	std::cout <<  "OpenGL has found: "  << glGetString(GL_VERSION) << std::endl;

	// Triangle Vertex locations. 
	float shapeVertices[] =
	{
		-0.5f, -0.5f, // 0
		-0.5f,  0.5f, // 1
		 0.5f, -0.5f, // 2
		 0.5f,  0.5f  // 3
	};

	// The index buffer tells us which points in the shape are joined to form edges
	unsigned int shapeIndexBuffer[] = {
		0, 1, 2, // TriangleOne
		1, 2, 3  // TriangleTwo
	};

	// Create vertex buffer
	// glBindBuffer selects the buffer, as in we're about to work on this buffer
	// Bind to what you want to draw on, think of layers in photoshop
	// glBufferData initializes the buffer and give it data
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), shapeVertices, GL_STATIC_DRAW);

	// Tell OpenGL The layout of out vertex buffer.
	// Enable drawing of Vertex
	glEnableVertexAttribArray(0);
	// Size = num points per vertex (two in this case).
	// Stride = length in bytes from the start of one vertex to start of the next.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	// Create a buffer for the shape index
	unsigned int ibo; // Index Buffer Object
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), shapeIndexBuffer, GL_STATIC_DRAW);

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}