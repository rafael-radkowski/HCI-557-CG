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


#include "GLObject.h"
#include "Shaders.h"


using namespace std;

class GLSphere : public GLObject
{
private:
    
    class Vertex{
    public:
        float x;
        float y;
        float z;
        
        Vertex(float px, float py, float pz):
        x(px), y(py), z(pz){};
        
        Vertex():
        x(0.0), y(0.0), z(0.0){};
        
        
        Vertex operator-(Vertex& r)
        {
            Vertex res;
            res.x = x-r.x;
            res.y = y-r.y;
            res.z = z-r.z;
            return res;
        }
        
        Vertex operator+(Vertex& r)
        {
            Vertex res;
            res.x = x+r.x;
            res.y = y+r.y;
            res.z = z+r.z;
            return res;
        }
        
        Vertex& operator*(float r)
        {
            Vertex res;
            res.x = x*r;
            res.y = y*r;
            res.z = z*r;
            return res;
        }
        
        Vertex& operator/(float r)
        {
            Vertex res;
            res.x = x/r;
            res.y = y/r;
            res.z = z/r;
            return res;
        }
        
        void operator=(Vertex& r)
        {
            Vertex res;
            x = r.x;
            y = r.y;
            z = r.z;
        }
        
        inline void normalize(void)
        {
            float l = length();
            x = x / l;
            y = y / l;
            z = z / l;
            
        }
        
        inline float length(void)
        {
             return sqrt( float((x*x) + (y*y) + (z*z)) );
        }
        
        
    };
    

    
public:
    GLSphere(float center_x, float center_y, float center_z, float radius, int rows = 10, int segments = 10 );
    ~GLSphere();
    
    
    /*!
     Draw the objects
     */
    void draw(void);
    
    
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
    
private:
    
    
    /*!
     Create the vertex buffer object for this element
     */
    void initVBO(void);
    
    /*
     Inits the shader program for this object
     */
    void initShader(void);
    
    /*!
     Init a frame buffer object to draw normal vectors
     */
    void initVBONormals(void);
    
    /*!
     Shader for the normal vector renderer
     */
    void initShaderNormal(void);
    

    
    Vertex cross_product(Vertex& v1, Vertex& v2);
    
    /*
     Creates a sphere
     */
    void make_Sphere(Vertex center, double r, std::vector<Vertex> &spherePoints, std::vector<Vertex> &normals);
    
    
    // the program
    GLuint                  _program;
    int                     _viewMatrixLocation;
    int                     _modelMatrixLocation;
    int                     _lightPosition;


    
    // The material for thsi object
    GLMaterial              _sphereMaterial;
    
    // The light source
    GLLightSource           _light_source0;
   
    
    unsigned int            _vaoID[1]; // Our Vertex Array Object
    unsigned int            _vboID[3]; // Our Vertex Buffer Object
    
    
    GLuint                  _program_normals;
    int                     _viewMatrixLocationN;
    int                     _modelMatrixLocationN;
    unsigned int            _vaoIDNormals[1];
    unsigned int            _vboIDNormals[2]; // Our Vertex Buffer Object for normal vectors

    
    glm::mat4               _modelMatrix; // Store the model matrix
    
    
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














