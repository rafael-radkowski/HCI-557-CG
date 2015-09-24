//
//  GLObject.cpp
//  OpenGL_Transformations
//
//  Created by Rafael Radkowski on 9/12/15.
//
//

#include <glm/glm.hpp>


glm::mat4 g_projectionMatrix; // Store the projection matrix
glm::mat4 g_viewMatrix; // Store the view matrix


glm::mat4 g_trackball; // the trackball
glm::mat4 g_rotated_view;

#include "GLObject.h"



void SetTrackballLocation(glm::mat4 trackball)
{
    g_trackball = trackball;
    g_rotated_view = g_viewMatrix * g_trackball;
}

void SetViewAsLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
    g_viewMatrix = glm::lookAt(eye, center, up);
    
    g_rotated_view = g_viewMatrix * g_trackball;
}



GLObject::GLObject()
{

    g_projectionMatrix = glm::perspective(1.57f, (float)800 / (float)600, 0.1f, 100.f);
    g_viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    g_trackball =  glm::mat4();
    g_rotated_view = glm::mat4();
    
}


GLObject::~GLObject()
{

}



glm::mat4 GLObject::projectionMatrix(void)
{
    return g_projectionMatrix;
}


glm::mat4 GLObject::viewMatrix(void)
{
    return g_viewMatrix;
}



// returns the rotated view matrix, mulitplied with the trackball.
glm::mat4 GLObject::rotatedViewMatrix(void)
{
    return g_rotated_view;
}





