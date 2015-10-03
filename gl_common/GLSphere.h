//
//  GLSphere.h
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


// locals
#include "GLObject.h"
#include "Shaders.h"
#include "HCI557Datatypes.h"

using namespace std;

class GLSphere : public GLObject
{
public:
    /*!
     Constructor
     @param ceneter_x, center_y, center_z - the center of the sphere in x,y,z
     @param radius - the radius of the sphere
     @param rows - the number of triangle strip rows 
     @param segments - the number of triangles per row.
    */
    GLSphere(float center_x, float center_y, float center_z, float radius, int rows = 10, int segments = 10 );
    
    // default constructor
    GLSphere(){};
    
    // desctructor
    ~GLSphere();
    
    
    /*!
     Draw the objects
     */
    virtual  void draw(void);
    
    
    /*!
     Set the model matrix for this object
     @param modelmatrix: 4x4 model matrix
     */
    void setModelMatrix(glm::mat4& modelmatrix);
    
    /*!
     Retursn the model matrix.
     */
    inline glm::mat4& getModelMatrix(void){return _modelMatrix;};
    
    /*!
     Enables or disables the normal vector renderer
     @param value = true -> enables the renderer, false -> disables the renderer
     */
    void enableNormalVectorRenderer(bool value = true);
    
protected:
    
    
    /*!
     Create the vertex buffer object for this element
     */
    virtual void initVBO(void);
    
    /*
     Inits the shader program for this object
     */
    virtual void initShader(void);
    
    /*!
     Init a frame buffer object to draw normal vectors
     */
    void initVBONormals(void);
    
    /*!
     Shader for the normal vector renderer
     */
    void initShaderNormal(void);
    
    
    /*
     Creates a sphere
     */
    void make_Sphere(Vertex center, double r, std::vector<Vertex> &spherePoints, std::vector<Vertex> &normals);
    

    
    // the program
    GLuint                  _program;



    unsigned int            _vaoID[1]; // Our Vertex Array Object
    unsigned int            _vboID[3]; // Our Vertex Buffer Object
    
    
    // The light source
    GLLightSource           _light_source0;

    
    // The program and data to render normal vectors as lines
    GLuint                  _program_normals;
    unsigned int            _viewMatrixLocationN;
    unsigned int            _modelMatrixLocationN;
    unsigned int            _vaoIDNormals[1];
    unsigned int            _vboIDNormals[2]; // Our Vertex Buffer Object for normal vectors

    

    
    
    // the dimensions of the sphere
    int                     _segments;
    int                     _rows;
    
    int                     _num_vertices;
    int                     _num_vertices_normals;
    
    Vertex                  _center;
    float                   _radius;
    
    
    // variables to generate the sphere
    vector<Vertex>          _spherePoints;
    vector<Vertex>          _normalVectors;
    
    
    // value to switch the normal renderer on / off
    bool                    _render_normal_vectors;
};














