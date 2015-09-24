//
//  Shaders.cpp
//  OpenGL_Transformations
//
//  Created by Rafael Radkowski on 9/12/15.
//
//

#include "Shaders.h"



void CheckShader(GLuint shader, GLenum shader_type)
{
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    
    if (!compiled)
    {
        GLint info_len = 0;
        
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
        
        if (info_len)
        {
            char* buf = (char*) malloc(info_len);
            if (buf) {
                glGetShaderInfoLog(shader, info_len, NULL, buf);
                cout << "Could not compile shader shader_type:\n" <<  buf << endl;
                free(buf);
            }
            glDeleteShader(shader);
            shader = 0;
        }
    }
    

}



/*
 Creates a shader program
 */
GLuint CreateShaderProgram(string vertex_source, string fragment_source)
{
    GLuint  program;
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    // static const string vertex_code = vs_string_CoordSystem;
    static const char * vs_source = vertex_source.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    // static const string fragment_code = fs_string_CoordSystem;
    static const char * fs_source = fragment_source.c_str();
    
    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    program = glCreateProgram();
    
    // We create a shader with our fragment shader source code and compile it.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    CheckShader(fs, GL_FRAGMENT_SHADER);

    
    
    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    CheckShader(vs, GL_FRAGMENT_SHADER);
    
    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    
    glUseProgram(program);
    
    return program;
}

