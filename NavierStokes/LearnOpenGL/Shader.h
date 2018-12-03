#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{

public:

	unsigned int ID;

	Shader( const char* vertexPath, const char* fragmentPath )
	{
	
		// Retrieve the vertex/fragment source from filePath.
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// Ensure ifstream objects can throw exceptions.
		vShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
		
			// Open files.
			vShaderFile.open( vertexPath );
			fShaderFile.open( fragmentPath );
			std::stringstream vShaderStream, fShaderStream;

			// Read file's buffer contents into streams.
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// Close file handlers
			vShaderFile.close();
			fShaderFile.close();

			// Convert stream into string.
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

		}

		catch( std::ifstream::failure e )
		{
		
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// Compile shaders.
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		// Vertex shader.
		vertex = glCreateShader( GL_VERTEX_SHADER );
		glShaderSource( vertex, 1, &vShaderCode, NULL );
		glCompileShader( vertex );
		checkCompileErrors( vertex, "VERTEX" );

		// Fragment shader.
		fragment = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource( fragment, 1, &fShaderCode, NULL );
		glCompileShader( fragment );
		checkCompileErrors(vertex, "FRAGMENT");

		// Shader program.
		ID = glCreateProgram();
		glAttachShader( ID, vertex );
		glAttachShader( ID, fragment );
		glLinkProgram( ID );
		checkCompileErrors( ID, "PROGRAM" );

		// Delete the shaders we don't need them anymore they are linked into our
		// program.
		glDeleteShader( vertex );
		glDeleteShader( fragment );

	}

	void use() 
	{
	
		glUseProgram( ID );
	
	}

	void del()
	{
	
		glUseProgram( 0 );
	
	}

	// utility uniform functions
// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string &name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	// We define our compile error function
	void checkCompileErrors( unsigned int shader, std::string type )
	{

		int success;
		char infoLog[1024];

		if( type != "PROGRAM" )
		{
		
			glGetShaderiv( shader, GL_COMPILE_STATUS, &success );

			if ( !success )
			{

				glGetShaderInfoLog( shader, 1024, NULL, infoLog );
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;

			}

		}

		else
		{
		
			glGetProgramiv( shader, GL_LINK_STATUS, &success );

			if( !success )
			{ 
			
				glGetProgramInfoLog( shader, 1024, NULL, infoLog );
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;

			}

		}

	}

};

#endif