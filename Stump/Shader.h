#ifndef ST_SHADER_H
#define ST_SHADER_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);
class Vector3;
class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* p_vertexFile, const char* p_fragmentFile);

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

	void CompileErrors(unsigned int p_shader, const char* p_type);

	void SetInt(const char* p_name, int p_value, bool p_useShader = false);

	void SetFloat(const char* p_name, float p_value, bool p_useShader = false);

	void SetFloat3(const char* p_name, float p_x, float p_y, float p_z, bool p_useShader = false);

	void SetFloat4(const char* p_name, float p_x, float p_y, float p_z, float p_w, bool p_useShader = false);

	void SetVec3(const char* p_name, Vector3 p_vec, bool p_useShader = false);

	void SetMat4(const char* p_name, const float* p_matrix, bool p_transposed = false, bool p_useShader = false);
};
#endif
