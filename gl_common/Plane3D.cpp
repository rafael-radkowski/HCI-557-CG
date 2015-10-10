//
//  GLSphereExt.cpp
//  HCI557_Spotlight
//
//  Created by Rafael Radkowski on 9/26/15.
//
//

#include "Plane3D.h"




GLPlane3D::GLPlane3D(float center_x, float center_y, float center_z, float width, float height,  int rows, int cols  ):
    _center_x(center_x), _center_y(center_y), _center_z(center_z), _width(width), _height(height), _rows(rows), _cols(cols)
{

    _program = 0;

}


GLPlane3D::~GLPlane3D()
{
    // Program clean up when the window gets closed.
    glDeleteVertexArrays(1, _vaoID);
    glDeleteProgram(_program);

}




/*!
 Init the geometry object
 */
void GLPlane3D::init(void)
{
    
    _num_vertices = 0;
    
    initShader();
    initVBO();
    


}


/*!
 Draw the objects
 */
void GLPlane3D::draw(void)
{
    
    //////////////////////////////////////////////////
    // Renders the sphere
    
    // Enable the shader program
    glUseProgram(_program);
    
    // this changes the camera location
    glm::mat4 rotated_view =  rotatedViewMatrix();
    glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
    glUniformMatrix4fv(_inverseViewMatrixLocation, 1, GL_FALSE, &invRotatedViewMatrix()[0][0]);
    glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); //
    
    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(_vaoID[0]);
    
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // allows to see the primitives
    // Draw the triangles
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _num_vertices);
    
    // Unbind our Vertex Array Object
    glBindVertexArray(0);
    
    // Unbind the shader program
    glUseProgram(0);
    
    
}



/*
 Inits the shader program for this object
 */
void GLPlane3D::initShader(void)
{
    
    if(!_apperance.exists())return;
    
    // This loads the shader program from a file
    _program = _apperance.getProgram();
    

    glUseProgram(_program);
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Vertex information / names
    
    glBindAttribLocation(_program, 0, "in_Position");
    glBindAttribLocation(_program, 1, "in_Normal");
    glBindAttribLocation(_program, 2, "in_TexCoord");
    
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Define the model view matrix.
    _modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    addModelViewMatrixToProgram(_program);
    
 

    glUseProgram(0);

}


/*!
 Create the vertex buffer object for this element
 */
void GLPlane3D::initVBO(void)
{
    
    
    _vertex_points.clear();
    _normal_vectors.clear();
    
    //---------------------------------------------------------
    // This creates the box geometry
    make_box(_center_x, _center_y, _center_z, _width,
              _height,  _rows, _cols,
             _vertex_points, _normal_vectors);
    
    _num_vertices = _vertex_points.size();
    
    
    // create memory for the vertices, etc.
    float* vertices = new float[_num_vertices * 5];
    float* normals = new float[_num_vertices * 3];
    
    
    // copy the data to the vectors
    for(int i=0; i<_vertex_points.size() ; i++)
    {
        Vertex v = _vertex_points[i];
        vertices[(i*5)] = v.x(); vertices[(i*5)+1] = v.y(); vertices[(i*5)+2] = v.z();
        vertices[(i*5)+3] = v.u(); vertices[(i*5)+4] = v.v();
        
        Vertex n = _normal_vectors[i];
        normals[(i*3)] = n.x(); normals[(i*3)+1] = n.y(); normals[(i*3)+2] = n.z();

    }
    
    glUseProgram(_program);
    
    
    glGenVertexArrays(1, _vaoID); // Create our Vertex Array Object
    glBindVertexArray(_vaoID[0]); // Bind our Vertex Array Object so we can use it
    
    
    glGenBuffers(3, _vboID); // Generate our Vertex Buffer Object
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, _num_vertices * 5 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    int locPos = glGetAttribLocation(_program, "in_Position");
    glVertexAttribPointer((GLuint)locPos, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(locPos); //
    
    
    int tex_idx = glGetAttribLocation(_program, "in_TexCoord");
    glVertexAttribPointer((GLuint)tex_idx, 2, GL_FLOAT, GL_TRUE, 5*sizeof(GLfloat),
                          (const GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(tex_idx);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Normals
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, _num_vertices * 3 *  sizeof(GLfloat), normals, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    int locNorm = glGetAttribLocation(_program, "in_Normal");
    glVertexAttribPointer((GLuint)locNorm, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(locNorm); //
    
    
   
    
    glBindVertexArray(0); // Disable our Vertex Buffer Object
    
    // delete the memory
    delete vertices;
    delete normals;
}




/*!
 Creates the points and the normal vectors for this box
 */
void GLPlane3D::make_box(float center_x, float center_y, float center_z, float width,
                        float height,  int rows, int cols,
                        vector<Vertex>& vp, vector<Vertex>& nv )
{

    float delta_x = width/cols;
    float delta_y = height/rows;
    float start_x = width/2;
    float start_y = height/2;
    
    // TODO: add a better vertex generation code, one which incorporates the number of rows and colums.
    
    // allocate the memory.
    vp = vector<Vertex>(4);
    nv = vector<Vertex>(4);
    
    vp[0].x() = -_width/2.0 + center_x;
    vp[0].y() = -_height/2.0 + center_y;
    vp[0].z() = center_z;
    vp[0].u() = 0.0;
    vp[0].v() = 0.0;
    
    vp[1].x() = -_width/2.0 + center_x;
    vp[1].y() = _height/2.0 + center_y;
    vp[1].z() = center_z;
    vp[1].u() = 0.0;
    vp[1].v() = 1.0;
    
    vp[2].x() = _width/2.0 + center_x;
    vp[2].y() = -_height/2.0 + center_y;
    vp[2].z() = center_z;
    vp[2].u() = 1.0;
    vp[2].v() = 0.0;
    
    vp[3].x() = _width/2.0 + center_x;
    vp[3].y() = _height/2.0 + center_y;
    vp[3].z() = center_z;
    vp[3].u() = 1.0;
    vp[3].v() = 1.0;

    for (int i=0; i<4; i++) {
        nv[i].x() = 0.0;
        nv[i].y() = 0.0;
        nv[i].z() = 1.0;
    }

}





























