//
//  GLColoredBox.h
//  OpenGL_Transformations
//
//  Created by Rafael Radkowski on 9/12/15.
//
//

#pragma once

// stl include
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// local
#include "VertexBuffers.h"			// create vertex buffer object
#include "ShaderProgram.h"			// create a shader program


using namespace std;

class ModelSurface 
{
   
    
public:
   
    
    
    /*!
     Draw the objects
     */
    void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 modelMatrix);
    
    
    /*!
     Init the geometry object
     */
    void create(vector<glm::vec3> &vertices, vector<glm::vec3> &normals,  int shader_program = -1);


    /*
    Return the shader program
    @return - int containing the shader program
    */
    int getProgram(void){return _program;}
    
    


    
private:
    
    
    
    /*!
     Create the vertex buffer object for this element
     */
    void initVBO(void);
    

    
    
    // the program
    GLuint                  _program;
    

protected:
    // The data
    vector<glm::vec3>       _vertices;
    vector<glm::vec4>       _vertex_colors;
    vector<glm::vec3>       _normals;
    
private:
    
    int                     _num_vertices;
    
    unsigned int            _vaoID[1]; // Our Vertex Array Object
    
    unsigned int            _vboID[2]; // Our Vertex Buffer Object


    int viewMatrixLocation;
	int modelMatrixLocation;
	int projMatrixLocation;

};