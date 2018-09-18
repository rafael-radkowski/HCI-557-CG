#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <strstream>
#include <string>



using namespace std;

namespace cs557
{



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
		"    pass_Color = in_Color;                                         \n"
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
	static const string fs_string_simple_shader_410 =
		"#version 410 core                                                 \n"
		"                                                                  \n"
		"in vec3 pass_Color;                                                 \n"
		"out vec4 color;                                                    \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    color = vec4(pass_Color, 1.0);                               \n"
		"}                                                                 \n";


	static const string fs_string_simple_shader_300 =
		"#version 310 core                                                 \n"
		"                                                                  \n"
		"                                                \n"
		"                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    gl_FragColor = vec4(pass_Color, 1.0);                               \n"
		"}                                                                 \n";




}