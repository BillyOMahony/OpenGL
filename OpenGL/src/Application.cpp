#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();

// Macro which calls function x between GLClearError and GLLogCall
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))


/**
 *	Contains source code for the Vertex and Fragment shaders.
 */
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


/**
 *	Clears all errors
 */
static void GLClearError()
{
	// While there are errors (glGetError will return error flags until there are no errors)
	while (glGetError() != GL_NO_ERROR);
}

/**
 *	Checks for and prints OpenGL errors
 */
static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL_Error] (" << error << ")" << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

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
		char* message = (char*) alloca(length * sizeof(char)); // This because char message[length] doesn't work because c++....
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		
		std::cout << (type == GL_VERTEX_SHADER ?  "Vertex"  :  "Fragment" )
				  <<  " Shader Compilation Error: "  
				  << message 
				  << std::endl;

		// Delete the shader because it's broken
		GLCall(glDeleteShader(id));
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
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// Link the two shaders to the shader program
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));

	/*
	// This is not entirely necessary and optimization gains from it are minimal
	// once linked detach the shaders
	glDetachShader(program, vs);
	glDetachShader(program, fs);
	*/

	// Validate the program
	GLCall(glValidateProgram(program));

	// Delete the shaders, these are no longer necessary
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	unsigned int vao; // holds vertex array id
	GLCall(glGenVertexArrays(1, &vao)); // Generate one vertex array and store id in vao variable
	GLCall(glBindVertexArray(vao));

	// Create vertex buffer
	// glBindBuffer selects the buffer, as in we're about to work on this buffer
	// Bind to what you want to draw on, think of layers in photoshop
	// glBufferData initializes the buffer and give it data
	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), shapeVertices, GL_STATIC_DRAW));

	// Tell OpenGL The layout of our vertex buffer.
	// Enable drawing of Vertex
	GLCall(glEnableVertexAttribArray(0));
	// Size = num points per vertex (two in this case).
	// Stride = length in bytes from the start of one vertex to start of the next.
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

	// Create an index buffer, this tells us which vertices are connected to form edges
	unsigned int ibo; // Index Buffer Object
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), shapeIndexBuffer, GL_STATIC_DRAW));

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);
	GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.f));

	// Unbind everything
	GLCall(glBindVertexArray(0));
	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


	float r = 0.2f;
	float g = 0.2f;
	float b = 0.8f;
	float a = 1.f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		// Bind Shader and set colour
		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, g, b, a));

		// Bind Vertex Array and Index Buffer
		GLCall(glBindVertexArray(vao));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));	
				
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}