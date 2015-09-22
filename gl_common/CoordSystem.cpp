

#include "CoordSystem.h"





static const string vs_string_CoordSystem_410 =
"#version 410 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrix;                                    \n"
"uniform mat4 viewMatrix;                                           \n"
"uniform mat4 modelMatrix;                                          \n"
"in vec3 in_Position;                                               \n"
"                                                                   \n"
"in vec3 in_Color;                                                  \n"
"out vec3 pass_Color;                                               \n"
"                                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
"    pass_Color = in_Color;                                         \n"
"}                                                                 \n";

// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string_CoordSystem_410  =
"#version 410 core                                                 \n"
"                                                                  \n"
"in vec3 pass_Color;                                                 \n"
"out vec4 color;                                                    \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    color = vec4(pass_Color, 1.0);                               \n"
"}                                                                 \n";






CoordSystem::CoordSystem(float length):
    _length(length)
{
    
    initShader();
    initVBO();
}




CoordSystem::~CoordSystem()
{
    // Program clean up when the window gets closed.
    glDeleteVertexArrays(1, _vaoID);
    glDeleteProgram(_program);
}


/*!
 Draw the objects
 */
void CoordSystem::draw(void)
{
    
    // Enable the shader program
    glUseProgram(_program);
    
    
    
    // this changes the camera location
    glm::mat4 rotated_view =  rotatedViewMatrix();
    glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
    

    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(_vaoID[0]);
    
    glLineWidth((GLfloat)3.0);
    // Draw the triangles
    glDrawArrays(GL_LINES, 0, 6);
    
    // Unbind our Vertex Array Object
    glBindVertexArray(0);
    
    // Unbind the shader program
    glUseProgram(0);
    

}







/*!
 Create the vertex buffer object for this element
 */
void CoordSystem::initVBO(void)
{



    float vertices[]  = {0.0, 0.0, 0.0, _length, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, _length, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0, _length};
    
    float colors[] =  { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0, 1.0};
    
    
    glGenVertexArrays(1, _vaoID); // Create our Vertex Array Object
    glBindVertexArray(_vaoID[0]); // Bind our Vertex Array Object so we can use it
    
    
    glGenBuffers(2, _vboID); // Generate our Vertex Buffer Object
    
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(0); //
    
    
    //Color
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(1); //
    
    
    glBindVertexArray(0); // Disable our Vertex Buffer Object


    
}


/*
 Inits the shader program for this object
 */
void CoordSystem::initShader(void)
{
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
   // static const string vertex_code = vs_string_CoordSystem;
    static const char * vs_source = vs_string_CoordSystem_410.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
   // static const string fragment_code = fs_string_CoordSystem;
    static const char * fs_source = fs_string_CoordSystem_410.c_str();
    
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
    

    _projectionMatrixLocation = glGetUniformLocation(_program, "projectionMatrix"); // Get the location of our projection matrix in the shader
    _viewMatrixLocation = glGetUniformLocation(_program, "viewMatrix"); // Get the location of our view matrix in the shader
    int modelMatrixLocation = glGetUniformLocation(_program, "modelMatrix"); // Get the location of our model matrix in the shader
    
    
    glUniformMatrix4fv(_projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix()[0][0] ); // Send our projection matrix to the shader
    glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &viewMatrix()[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); // Send our model matrix to the shader

    
    glBindAttribLocation(_program, 0, "in_Position");
    glBindAttribLocation(_program, 1, "in_Color");
    
    
    
    
    
}




