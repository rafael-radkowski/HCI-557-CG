//
//  GLSphereExt.cpp
//  HCI557_Spotlight
//
//  Created by Rafael Radkowski on 9/26/15.
//
//

#include "Sphere3D.h"




GLSphere3D::GLSphere3D(float center_x, float center_y, float center_z, float radius, int rows, int segments )
{

    _center.x() = center_x;
    _center.y() = center_y;
    _center.z() = center_z;
    
    
    _radius = radius;
    _rows = rows;
    _segments = segments;
    
    _program_normals = 0;
    _program = 0;

    
    _render_normal_vectors = false;
    
}


GLSphere3D::~GLSphere3D()
{
    
    // Program clean up when the window gets closed.
    glDeleteVertexArrays(1, _vaoID);
    glDeleteVertexArrays(1, _vaoIDNormals);
    glDeleteProgram(_program);
    glDeleteProgram(_program_normals);
}




/*!
 Init the geometry object
 */
void GLSphere3D::init(void)
{
    
    initShader();
    initVBO();
    
    initShaderNormal();
    initVBONormals();

}



/*
 Inits the shader program for this object
 */
void GLSphere3D::initShader(void)
{
    
    if(!_apperance.exists())return;
    
    // This loads the shader program from a file
    _program = _apperance.getProgram();
    

    glUseProgram(_program);
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Vertex information / names
    
    glBindAttribLocation(_program, 0, "in_Position");
    glBindAttribLocation(_program, 1, "in_Normal");
    glBindAttribLocation(_program, 2, "in_Color");
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Define the model view matrix.
    _modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    addModelViewMatrixToProgram(_program);
    
 

    glUseProgram(0);

}
