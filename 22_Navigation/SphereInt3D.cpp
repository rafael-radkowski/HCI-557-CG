//
//  GLSphereExt.cpp
//  HCI557_Spotlight
//
//  Created by Rafael Radkowski on 9/26/15.
//
//

#include "SphereInt3D.h"




GLSphereInt3D::GLSphereInt3D(float center_x, float center_y, float center_z, float radius, int rows, int segments )
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
 
    _position_initialized = false;
    _target_position  = glm::vec3(0.0, 0.0, 0.0);
    
    _with_interpolation = true;
}


GLSphereInt3D::~GLSphereInt3D()
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
void GLSphereInt3D::init(void)
{
    
    initShader();
    initVBO();
    
    initShaderNormal();
    initVBONormals();

}



/*
 Inits the shader program for this object
 */
void GLSphereInt3D::initShader(void)
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



/*!
 Draw the objects
 */
void GLSphereInt3D::draw(void)
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
    
    ////////////////////////////
    // Call the interpolation function
    if(_with_interpolation) interpolateMat();
    
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // allows to see the primitives
    // Draw the triangles
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _num_vertices);
    
    //////////////////////////////////////////////////
    // Renders the normal vectors
    
    if(_render_normal_vectors)
    {
        
        // Enable the shader program
        glUseProgram(_program_normals);
        
        glUniformMatrix4fv(_viewMatrixLocationN, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
        glUniformMatrix4fv(_modelMatrixLocationN, 1, GL_FALSE, &_modelMatrix[0][0]); //
        
        
        // Bind the buffer and switch it to an active buffer
        glBindVertexArray(_vaoIDNormals[0]);
        glDrawArrays(GL_LINES, 0, _num_vertices_normals);
    }
    
    // Unbind our Vertex Array Object
    glBindVertexArray(0);
    
    // Unbind the shader program
    glUseProgram(0);
    
    
}



/*!
 Set a model matrix to move the object around
 @param matrix - the model matrix for this object.
 */
void GLSphereInt3D::setMatrix(glm::mat4& matrix)
{
    /////////////////////////////////////////////
    // With interpolation
    if(_with_interpolation)
    {
        // with interpolation, remember the target location that you like to find
        _target_matrix = matrix;
        _target_quat = glm::quat_cast(matrix);
        _target_position[0] = _target_matrix[3][0];
        _target_position[1] = _target_matrix[3][1];
        _target_position[2] = _target_matrix[3][2];
    
    
        // Just for initialization. Move your object immediately to the location
        if(!_position_initialized)
        {
            _current_quat = glm::quat_cast(matrix);
            _current_position = _target_position;
        
            // update the matrix.
            _modelMatrix =  matrix;
            glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]);
            _position_initialized = true;
        }
    }
    else
    /////////////////////////////////////////////
    // Without interpolation
    {
        _modelMatrix =  matrix;
        glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]);
    }

}



/*!
 Interpolation function for the matrix /
 the position of the object
 */
void GLSphereInt3D::interpolateMat(void)
{
    // Calculate the distance between the target position and the current position.
    glm::vec3 temp = _target_position - _current_position;
    float distance = sqrt(dot(temp, temp));
    
    
    glm::quat temp_quad;
    temp_quad.x = _target_quat.x - _current_quat.x;
    temp_quad.y = _target_quat.y - _current_quat.y;
    temp_quad.z = _target_quat.z - _current_quat.z;
    temp_quad.w = _target_quat.w - _current_quat.w;
    
    // Calculate the distance between the target angle and the current angle.
    float delta_angle = sqrt( (_target_quat.x - _current_quat.x)*(_target_quat.x - _current_quat.x)  +
                         (_target_quat.y - _current_quat.y)*(_target_quat.y - _current_quat.y)  +
                         (_target_quat.z - _current_quat.z)*(_target_quat.z - _current_quat.z)  +
                        (_target_quat.w - _current_quat.w)*(_target_quat.w - _current_quat.w));
    
    // If the distance is too large, find the next step
    if (distance > 0.01 || delta_angle > 0.01) {
        
        // Linear interpolation of the position
        _current_position = _current_position + temp *  glm::vec3(0.08);
        
        // Linear interpolation of the rotation using slerp
        _current_quat = glm::slerp(_current_quat, _target_quat, 0.25f);
        
        // convert the quaternion to a matrix
        _target_matrix = glm::mat4_cast(_current_quat);
        
        // write the position back.
        _target_matrix[3][0] = _current_position[0];
        _target_matrix[3][1] = _current_position[1];
        _target_matrix[3][2] = _current_position[2];
        
        // update the model matrix.
        _modelMatrix = _target_matrix;
        glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]);
        
    }
}


/*!
 Enable interpolation.
 */
void GLSphereInt3D::enableInterpolation(bool enable)
{
    _with_interpolation = enable;
}