//
//  GLSphere.h
//  OpenGL_Transformations
//
//  Created by Rafael Radkowski on 9/12/15.
//
//

#pragma once

// stl include
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>




using namespace std;


/*!
 Check the shader code for errors
 @param shader: the shader program id
 @param shader_type, vertex or framgment shader
 */
void CheckShader(GLuint shader, GLenum shader_type);


/*
 Creates a shader program
 @param vertex source - the vertex shader source code
 @param fragment_source - the fragment shader source code
 @return - Gluint of the shader program
 */
GLuint CreateShaderProgram(string vertex_source, string fragment_source);





static const string vs_string_simple_shader_410 =
"#version 410 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrix;                                    \n"
"uniform mat4 viewMatrix;                                           \n"
"uniform mat4 modelMatrix;                                          \n"
"in vec3 in_Position;                                               \n"
"in vec3 in_Color;                                                  \n"
"out vec3 pass_Color;                                               \n"
"                                                                 \n"
"                                                                 \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"                                                                  \n"
"                                                                 \n"
"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
"    pass_Color = vec3(in_Color);                                         \n"
"}                                                                 \n";



static const string vs_string_simple_shader_300 =
"#version 310 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrix;                                    \n"
"uniform mat4 viewMatrix;                                           \n"
"uniform mat4 modelMatrix;                                          \n"
"in vec3 in_Position;                                               \n"
"in vec3 in_Color;                                                  \n"
"                                              \n"
"                                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
"                                                                   \n"
"                                                                   \n"
"    gl_FrontColor = vec4(in_Color, 1.0);                          \n"
"}                                                                   \n";




// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string_simple_shader_410  =
"#version 410 core                                                 \n"
"                                                                  \n"
"in vec3 pass_Color;                                                 \n"
"out vec4 color;                                                    \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    color = vec4(pass_Color, 1.0);                               \n"
"}                                                                 \n";


static const string fs_string_simple_shader_300  =
"#version 310 core                                                 \n"
"                                                                  \n"
"                                                \n"
"                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    gl_FragColor = vec4(pass_Color, 1.0);                               \n"
"}                                                                 \n";





