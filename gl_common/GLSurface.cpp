

#include "GLSurface.h"

#include <algorithm>


GLSurface::GLSurface( vector<glm::vec3> &vertices, vector<glm::vec3> &normals):
    _vertices(vertices), _normals(normals)
{
    
    
}


GLSurface::~GLSurface()
{

}



/*!
 Init the geometry object
 */
void GLSurface::init(void)
{
  
    
    initShader();
    initVBO();
    
}








/*!
 Create the vertex buffer object for this element
 */
void GLSurface::initVBO(void)
{
    _num_vertices = _vertices.size();
    
    // create memory for the vertices, etc.
    float* vertices = new float[_num_vertices * 3];
    float* normals = new float[_normals.size() * 3];

    
    // Copy all vertices
    for(int i=0; i<_vertices.size() ; i++)
    {
        glm::vec3 t = _vertices[i];
        for (int j=0; j<3; j++) {
            vertices[(i*3)+j] = t[j];
        }
    }
    
    // copy all normals
    for(int i=0; i<_normals.size() ; i++)
    {
        glm::vec3 n = _normals[i];
        for (int j=0; j<3; j++) {
            normals[(i*3)+j] = n[j];
        }
    }

    
    
    
    glGenVertexArrays(1, _vaoID); // Create our Vertex Array Object
    glBindVertexArray(_vaoID[0]); // Bind our Vertex Array Object so we can use it
    
    
    glGenBuffers(2, _vboID); // Generate our Vertex Buffer Object
    
    // vertices
    int locPos = glGetAttribLocation(_program, "in_Position");
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, _num_vertices * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)locPos, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(locPos); //
    
    
     // normals
    int locNorm = glGetAttribLocation(_program, "in_Normal");
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, _normals.size() * 3 * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)locNorm, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(locNorm); //
    

    
    glBindVertexArray(0); // Disable our Vertex Buffer Object

}


/*
 Inits the shader program for this object
 */
void GLSurface::initShader(void)
{
    
    if(!_apperance.exists())return;
    
    // This loads the shader program from a file
    _program = _apperance.getProgram();
    
    
    glUseProgram(_program);
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Vertex information / names
    
    glBindAttribLocation(_program, 0, "in_Position");
    glBindAttribLocation(_program, 1, "in_Normal");

    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Define the model view matrix.
    _modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    addModelViewMatrixToProgram(_program);
    
    
    
    glUseProgram(0);
    
}




/*!
 Draw the objects
 */
void GLSurface::draw(void)
{
    
    glUseProgram(_program);

    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(_vaoID[0]);
    
    
    // this changes the camera location
    glm::mat4 rotated_view =  rotatedViewMatrix();
    glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
    glUniformMatrix4fv(_inverseViewMatrixLocation, 1, GL_FALSE, &invRotatedViewMatrix()[0][0]);
    glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); //

    
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, _num_vertices);
    
    
    
    // Unbind our Vertex Array Object
    glBindVertexArray(0);
    
}



/*!
Returns the number of vertices
*/
int GLSurface::size(void)
{
    return _num_vertices;
}


