//
//  GLSphereExt.cpp
//  HCI557_Spotlight
//
//  Created by Rafael Radkowski on 9/26/15.
//
//

#include "Box3D.h"




GLBox3D::GLBox3D(float width, float height, float length,  int rows, int cols  ):
   _width(width), _height(height), _length(length), _rows(rows), _cols(cols)
{
    _center_x = 0.0;
    _center_y = 0.0;
    _center_z = 0.0;
    
    _program = 0;

}


GLBox3D::~GLBox3D()
{
    // Program clean up when the window gets closed.
    glDeleteVertexArrays(1, _vaoID);
    glDeleteProgram(_program);

}




/*!
 Init the geometry object
 */
void GLBox3D::init(void)
{
    
    _num_vertices = 0;
    
    initShader();
    initVBO();
    


}


/*!
 Draw the objects
 */
void GLBox3D::draw(void)
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
    
   // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // allows to see the primitives
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



/*
 Inits the shader program for this object
 */
void GLBox3D::initShader(void)
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
void GLBox3D::initVBO(void)
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
    glBufferData(GL_ARRAY_BUFFER, _num_vertices * 5 * sizeof(GLfloat), &vertices[0], GL_DYNAMIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    
    int locPos = glGetAttribLocation(_program, "in_Position");
    glVertexAttribPointer((GLuint)locPos, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(locPos); //
    
    
    int tex_idx = glGetAttribLocation(_program, "in_TexCoord");
    glEnableVertexAttribArray(tex_idx);
    glVertexAttribPointer((GLuint)tex_idx, 2, GL_FLOAT, GL_TRUE, 5*sizeof(GLfloat),
                          (const GLvoid*)(3 * sizeof(GLfloat)));
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Normals
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, _num_vertices * 3 *  sizeof(GLfloat), normals, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    int locNorm = glGetAttribLocation(_program, "in_Normal");
    glEnableVertexAttribArray(locNorm); //
    glVertexAttribPointer((GLuint)locNorm, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    
    
    glBindVertexArray(0); // Disable our Vertex Buffer Object
    
    // delete the memory
    delete vertices;
    delete normals;
}




/*!
 Creates the points and the normal vectors for this box
 */
void GLBox3D::make_box(float center_x, float center_y, float center_z, float width,
                        float height,  int rows, int cols,
                        vector<Vertex>& vp, vector<Vertex>& nv )
{

    float delta_x = width/cols;
    float delta_y = height/rows;
    float start_x = width/2;
    float start_y = height/2;
    
    // TODO: add a better vertex generation code, one which incorporates the number of rows and colums.
    
    // allocate the memory.
    vp = vector<Vertex>(24);
    nv = vector<Vertex>(24);
    
    //--------------------------------------------------------
    // xy-plane, positive z direction
    vp[0].x() = -_width/2.0 + center_x;
    vp[0].y() = -_height/2.0 + center_y;
    vp[0].z() = _length/2.0 + center_z;
    vp[0].u() = 0.0;
    vp[0].v() = 0.0;
    
    vp[1].x() = -_width/2.0 + center_x;
    vp[1].y() = _height/2.0 + center_y;
    vp[1].z() = _length/2.0 + center_z;
    vp[1].u() = 0.0;
    vp[1].v() = 1.0;
    
    vp[2].x() = _width/2.0 + center_x;
    vp[2].y() = -_height/2.0 + center_y;
    vp[2].z() = _length/2.0 + center_z;
    vp[2].u() = 1.0;
    vp[2].v() = 0.0;
    
    vp[3].x() = _width/2.0 + center_x;
    vp[3].y() = _height/2.0 + center_y;
    vp[3].z() = _length/2.0 + center_z;
    vp[3].u() = 1.0;
    vp[3].v() = 1.0;

    for (int i=0; i<4; i++) {
        nv[i].x() = 0.0;
        nv[i].y() = 0.0;
        nv[i].z() = 1.0;
    }

    
    //--------------------------------------------------------
    // xy-plane, negative z direction
    vp[4].x() = _width/2.0 + center_x;
    vp[4].y() = -_height/2.0 + center_y;
    vp[4].z() = -_length/2.0 + center_z;
    vp[4].u() = 0.0;
    vp[4].v() = 0.0;
    
    vp[5].x() = _width/2.0 + center_x;
    vp[5].y() = _height/2.0 + center_y;
    vp[5].z() = -_length/2.0 + center_z;
    vp[5].u() = 0.0;
    vp[5].v() = 1.0;
    
    vp[6].x() = -_width/2.0 + center_x;
    vp[6].y() = -_height/2.0 + center_y;
    vp[6].z() = -_length/2.0 + center_z;
    vp[6].u() = 1.0;
    vp[6].v() = 0.0;
    
    vp[7].x() = -_width/2.0 + center_x;
    vp[7].y() = _height/2.0 + center_y;
    vp[7].z() = -_length/2.0 + center_z;
    vp[7].u() = 1.0;
    vp[7].v() = 1.0;
    
    for (int i=4; i<8; i++) {
        nv[i].x() = 0.0;
        nv[i].y() = 0.0;
        nv[i].z() = -1.0;
    }
    
    //--------------------------------------------------------
    // yz-plane, positive x direction
    vp[8].x() = _width/2.0 + center_x;
    vp[8].y() = -_height/2.0 + center_y;
    vp[8].z() = _length/2.0 + center_z;
    vp[8].u() = 0.0;
    vp[8].v() = 0.0;
    
    vp[9].x() = _width/2.0 + center_x;
    vp[9].y() = _height/2.0 + center_y;
    vp[9].z() = _length/2.0 + center_z;
    vp[9].u() = 0.0;
    vp[9].v() = 1.0;
    
    vp[10].x() = _width/2.0 + center_x;
    vp[10].y() = -_height/2.0 + center_y;
    vp[10].z() = -_length/2.0 + center_z;
    vp[10].u() = 1.0;
    vp[10].v() = 0.0;
    
    vp[11].x() = _width/2.0 + center_x;
    vp[11].y() = _height/2.0 + center_y;
    vp[11].z() = -_length/2.0 + center_z;
    vp[11].u() = 1.0;
    vp[11].v() = 1.0;
    
    for (int i=8; i<8+4; i++) {
        nv[i].x() = 1.0;
        nv[i].y() = 0.0;
        nv[i].z() = 0.0;
    }
    
    
    //--------------------------------------------------------
    // yz-plane, negative x direction
    vp[12].x() = -_width/2.0 + center_x;
    vp[12].y() = -_height/2.0 + center_y;
    vp[12].z() = _length/2.0 + center_z;
    vp[12].u() = 0.0;
    vp[12].v() = 0.0;
    
    vp[13].x() = -_width/2.0 + center_x;
    vp[13].y() = _height/2.0 + center_y;
    vp[13].z() = _length/2.0 + center_z;
    vp[13].u() = 0.0;
    vp[13].v() = 1.0;
    
    vp[14].x() = -_width/2.0 + center_x;
    vp[14].y() = -_height/2.0 + center_y;
    vp[14].z() = -_length/2.0 + center_z;
    vp[14].u() = 1.0;
    vp[14].v() = 0.0;
    
    vp[15].x() = -_width/2.0 + center_x;
    vp[15].y() = _height/2.0 + center_y;
    vp[15].z() = -_length/2.0 + center_z;
    vp[15].u() = 1.0;
    vp[15].v() = 1.0;
    
    for (int i=12; i<12+4; i++) {
        nv[i].x() = -1.0;
        nv[i].y() = 0.0;
        nv[i].z() = 0.0;
    }
    
    
    
    
    //--------------------------------------------------------
    // xz-plane, positive y direction
    vp[16].x() = -_width/2.0 + center_x;
    vp[16].y() = _height/2.0 + center_y;
    vp[16].z() = _length/2.0 + center_z;
    vp[16].u() = 0.0;
    vp[16].v() = 0.0;
    
    vp[17].x() = -_width/2.0 + center_x;
    vp[17].y() = _height/2.0 + center_y;
    vp[17].z() = -_length/2.0 + center_z;
    vp[17].u() = 0.0;
    vp[17].v() = 1.0;
    
    vp[18].x() = _width/2.0 + center_x;
    vp[18].y() = _height/2.0 + center_y;
    vp[18].z() = _length/2.0 + center_z;
    vp[18].u() = 1.0;
    vp[18].v() = 0.0;
    
    vp[19].x() = _width/2.0 + center_x;
    vp[19].y() = _height/2.0 + center_y;
    vp[19].z() = -_length/2.0 + center_z;
    vp[19].u() = 1.0;
    vp[19].v() = 1.0;
    
    for (int i=16; i<16+4; i++) {
        nv[i].x() = 0.0;
        nv[i].y() = 1.0;
        nv[i].z() = 0.0;
    }
    
    
    //--------------------------------------------------------
    // xz-plane, negative y direction
    vp[20].x() = -_width/2.0 + center_x;
    vp[20].y() = -_height/2.0 + center_y;
    vp[20].z() = _length/2.0 + center_z;
    vp[20].u() = 0.0;
    vp[20].v() = 0.0;
    
    vp[21].x() = -_width/2.0 + center_x;
    vp[21].y() = -_height/2.0 + center_y;
    vp[21].z() = -_length/2.0 + center_z;
    vp[21].u() = 0.0;
    vp[21].v() = 1.0;
    
    vp[22].x() = _width/2.0 + center_x;
    vp[22].y() = -_height/2.0 + center_y;
    vp[22].z() = _length/2.0 + center_z;
    vp[22].u() = 1.0;
    vp[22].v() = 0.0;
    
    vp[23].x() = _width/2.0 + center_x;
    vp[23].y() = -_height/2.0 + center_y;
    vp[23].z() = -_length/2.0 + center_z;
    vp[23].u() = 1.0;
    vp[23].v() = 1.0;
    
    for (int i=20; i<20+4; i++) {
        nv[i].x() = 0.0;
        nv[i].y() = -1.0;
        nv[i].z() = 0.0;
    }
    

}





























