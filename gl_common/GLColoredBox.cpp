//
//  GLColoredBox.cpp
//  OpenGL_Transformations
//
//  Created by Rafael Radkowski on 9/12/15.
//
//

#include "GLColoredBox.h"





static const string vs_string_ColoredBox_410 =
"#version 410 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrixBox;                                    \n"
"uniform mat4 viewMatrixBox;                                           \n"
"uniform mat4 modelMatrixBox;                                          \n"
"in vec3 in_Position;                                               \n"
"                                                                   \n"
"in vec3 in_Color;                                                  \n"
"out vec3 pass_Color;                                               \n"
"                                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    gl_Position = projectionMatrixBox * viewMatrixBox * modelMatrixBox * vec4(in_Position, 1.0);  \n"
"    pass_Color = in_Color;                                         \n"
"}                                                                 \n";




static const string vs_string_ColoredBox_300 =
"#version 310 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrixBox;                                    \n"
"uniform mat4 viewMatrixBox;                                           \n"
"uniform mat4 modelMatrixBox;                                          \n"
"in vec3 in_Position;                                               \n"
"                                                                   \n"
"in vec3 in_Color;                                                  \n"
"                                              \n"
"                                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    gl_Position = projectionMatrixBox * viewMatrixBox * modelMatrixBox * vec4(in_Position, 1.0);  \n"
"    gl_FrontColor = vec4(in_Color, 1.0);                                         \n"
"}                                                                       \n";




// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string_ColoredBox_410  =
"#version 410 core                                                 \n"
"                                                                  \n"
"in vec3 pass_Color;                                                 \n"
"out vec4 color;                                                    \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    color = vec4(pass_Color, 1.0);                               \n"
"}                                                                 \n";


static const string fs_string_ColoredBox_300  =
"#version 310 core                                                 \n"
"                                                                  \n"
"                                                \n"
"                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    gl_FragColor = vec4(pass_Color, 1.0);                               \n"
"}                                                                 \n";




GLColoredBox::GLColoredBox(float size_x, float size_y, float size_z):
    _size_x(size_x),_size_y(size_y),_size_z(size_z)
{
    
    initShader();
    initVBO();
}




GLColoredBox::~GLColoredBox()
{
    // Program clean up when the window gets closed.
    glDeleteVertexArrays(1, _vaoID);
    glDeleteProgram(_program);
}


/*!
 Draw the objects
 */
void GLColoredBox::draw(void)
{
    
    // Enable the shader program
    glUseProgram(_program);
    
    
    
    // this changes the camera location
    glm::mat4 rotated_view =  rotatedViewMatrix();
    glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
    
    glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); //
    
    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(_vaoID[0]);
    

    // Draw the triangles
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

    
    // Unbind our Vertex Array Object
    glBindVertexArray(0);
    
    // Unbind the shader program
    glUseProgram(0);
    
    
}

/*!
 Set the model matrix for this object
 @param modelmatrix: 4x4 model matrix
 */
void GLColoredBox::setModelMatrix(glm::mat4& modelmatrix)
{
    _modelMatrix=modelmatrix;

}



/*!
 Create the vertex buffer object for this element
 */
void GLColoredBox::initVBO(void)
{
    
    
    
    float vertices[]  = {   -_size_x/2.0f, -_size_y/2.0f, _size_z/2.0, -_size_x/2.0f, _size_y/2.0f, _size_z/2.0,
                            _size_x/2.0f, -_size_y/2.0f, _size_z/2.0, _size_x/2.0f, _size_y/2.0f, _size_z/2.0,
                            _size_x/2.0f, -_size_y/2.0f, _size_z/2.0,  _size_x/2.0f, _size_y/2.0f, _size_z/2.0,
                             _size_x/2.0f, -_size_y/2.0f, -_size_z/2.0, _size_x/2.0f, _size_y/2.0f, -_size_z/2.0,
                            _size_x/2.0f, -_size_y/2.0f, -_size_z/2.0, _size_x/2.0f, _size_y/2.0f, -_size_z/2.0,
                            -_size_x/2.0f, -_size_y/2.0f, -_size_z/2.0, -_size_x/2.0f, _size_y/2.0f, -_size_z/2.0,
                            -_size_x/2.0f, -_size_y/2.0f, -_size_z/2.0,  -_size_x/2.0f, _size_y/2.0f, -_size_z/2.0,
                            -_size_x/2.0f, -_size_y/2.0f, _size_z/2.0, -_size_x/2.0f, _size_y/2.0f, _size_z/2.0,
                            -_size_x/2.0f, _size_y/2.0f, _size_z/2.0, -_size_x/2.0f, _size_y/2.0f, -_size_z/2.0,
                            _size_x/2.0f, _size_y/2.0f, _size_z/2.0, _size_x/2.0f, _size_y/2.0f, -_size_z/2.0,
                            -_size_x/2.0f, -_size_y/2.0f, -_size_z/2.0, -_size_x/2.0f, -_size_y/2.0f, _size_z/2.0,
                            _size_x/2.0f, -_size_y/2.0f, -_size_z/2.0, _size_x/2.0f, -_size_y/2.0f, _size_z/2.0
                            };
    
    float colors[] =  { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                        1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
                        0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
                        1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
                        1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
                        1.0, 1.0, 0.0, 1.0, 1.0, 0.0,
                        1.0, 1.0, 0.0, 1.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
                        0.0, 1.0, 1.0, 0.0, 1.0, 1.0,
                        0.0, 1.0, 1.0, 0.0, 1.0, 1.0
                        };
    
    
    glGenVertexArrays(1, _vaoID); // Create our Vertex Array Object
    glBindVertexArray(_vaoID[0]); // Bind our Vertex Array Object so we can use it
    
    
    glGenBuffers(2, _vboID); // Generate our Vertex Buffer Object
    
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(0); //
    
    
    //Color
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(1); //
    
    
    glBindVertexArray(0); // Disable our Vertex Buffer Object
    
    
}


/*
 Inits the shader program for this object
 */
void GLColoredBox::initShader(void)
{
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    // static const string vertex_code = vs_string_CoordSystem;
    static const char * vs_source = vs_string_ColoredBox_410.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    // static const string fragment_code = fs_string_CoordSystem;
    static const char * fs_source = fs_string_ColoredBox_410.c_str();
    
    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    _program = glCreateProgram();
    
    // We create a shader with our fragment shader source code and compile it.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    
    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    
    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(_program, vs);
    glAttachShader(_program, fs);
    
    glLinkProgram(_program);
    
    glUseProgram(_program);
    

    _modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model


    int projectionMatrixLocation = glGetUniformLocation(_program, "projectionMatrixBox"); // Get the location of our projection matrix in the shader
    _viewMatrixLocation = glGetUniformLocation(_program, "viewMatrixBox"); // Get the location of our view matrix in the shader
    _modelMatrixLocation = glGetUniformLocation(_program, "modelMatrixBox"); // Get the location of our model matrix in the shader
    
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix()[0][0] ); // Send our projection matrix to the shader
    glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &viewMatrix()[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); // Send our model matrix to the shader
    
    
    glBindAttribLocation(_program, 0, "in_Position");
    glBindAttribLocation(_program, 1, "in_Color");
    
    
    
    
    
}


