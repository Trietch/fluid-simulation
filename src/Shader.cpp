#include "Shader.h"
#include "System.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vert_path, const char* frag_path)
	: _vert_path{vert_path}
	, _frag_path{frag_path}
{
	// TODO REFACTOR EVERYTHING
	
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vert_path);
		fShaderFile.open(frag_path);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode   = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (const std::ifstream::failure & e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	check_compilation(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	check_compilation(fragment, "FRAGMENT");
	// shader Program
	_id = glCreateProgram();
	glAttachShader(_id, vertex);
	glAttachShader(_id, fragment);
	glLinkProgram(_id);
	check_compilation(_id, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader() {

}

void Shader::use() const {
	glUseProgram(_id);
}

int Shader::get_location(const std::string &name) const {
	int location = glGetUniformLocation(_id, name.c_str());
	if (location == -1) {
		//std::cout << "SHADER: CAN'T FIND UNIFORM LOCATION : " << name << std::endl;
	}	
	return location;
}

std::string Shader::vert_path() const {
	return _vert_path;
}

std::string Shader::frag_path() const {
	return _frag_path;
}

void Shader::set_1i(const std::string &name, int value) const {
	glUniform1i(get_location(name), value);
}

void Shader::set_1f(const std::string &name, float value) const {
	glUniform1f(get_location(name), value);
}

void Shader::set_4f(const std::string &name, glm::vec4 values) const {
	glUniform4f(get_location(name), values.x, values.y, values.z, values.w);
}

void Shader::set_3f(const std::string &name, glm::vec3 values) const {
	glUniform3f(get_location(name), values.x, values.y, values.z);
}

void Shader::set_mat4(const std::string &name, glm::mat4 values) const {
	glUniformMatrix4fv(get_location(name), 1, GL_FALSE, glm::value_ptr(values));
}

void Shader::check_compilation(unsigned int shader, std::string type) {
	int success;
	char infoLog[1024]; // TODO TROUVER LA BONNE TAILLE PAS 1024 degueux
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
