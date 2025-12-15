#include "Shader.h"
#include <iostream>
#include <vector>
#include "Vector3.h"
// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* p_filename)
{
	std::ifstream in(p_filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char* p_vertexFile, const char* p_fragmentFile)
{

	std::cout << "Compiling Shader: " << p_vertexFile << " and " << p_fragmentFile << std::endl;
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = get_file_contents(p_vertexFile);
	std::string fragmentCode = get_file_contents(p_fragmentFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);


	CompileErrors(vertexShader, "VERTEX");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	
	glCompileShader(fragmentShader);

	CompileErrors(fragmentShader, "FRAGMENT");

	ID = glCreateProgram();
	
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	
	glLinkProgram(ID);

	CompileErrors(ID, "PROGRAM");

	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

// Activates the Shader Program
void Shader::Activate()
{
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}

// Checks if the different Shaders have compiled properly
void Shader::CompileErrors(unsigned int p_shader, const char* p_type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (p_type != "PROGRAM")
	{
		glGetShaderiv(p_shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(p_shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << p_type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(p_shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(p_shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << p_type << "\n" << infoLog << std::endl;
		}
	}
}

void Shader::SetFloat(const char* p_name, float p_value, bool p_useShader)
{
	if (p_useShader)
		Activate();
	glUniform1f(glGetUniformLocation(ID, p_name), p_value);
}
void Shader::SetFloat3(const char* p_name, float p_x, float p_y, float p_z, bool p_useShader)
{
	if (p_useShader)
		Activate();
	glUniform3f(glGetUniformLocation(ID, p_name), p_x, p_y, p_z);
}
void Shader::SetFloat4(const char* p_name, float p_x, float p_y, float p_z, float p_w, bool p_useShader)
{
	if (p_useShader)
		Activate();
	glUniform4f(glGetUniformLocation(ID, p_name), p_x, p_y, p_z, p_w);
}
void Shader::SetVec3(const char* p_name, Vector3 p_vec, bool p_useShader)
{
	if (p_useShader)
		Activate();
	glUniform3f(glGetUniformLocation(ID, p_name), p_vec.x, p_vec.y, p_vec.z);
}
void Shader::SetMat4(const char* p_name, const float* p_matrix, bool p_transposed, bool p_useShader)
{
	if (p_useShader)
		Activate();
	if(p_transposed)
		glUniformMatrix4fv(glGetUniformLocation(ID, p_name), 1, GL_TRUE, p_matrix);
	else
		glUniformMatrix4fv(glGetUniformLocation(ID, p_name), 1, GL_FALSE, p_matrix);
}
