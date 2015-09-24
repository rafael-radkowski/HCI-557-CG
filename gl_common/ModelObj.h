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



using namespace std;

class ModelObj
{
public:
    ModelObj(string filename);
    ModelObj();
    ~ModelObj();
    
    
    /*!
     Draw the objects
     */
    void draw(void);
    
    
private:
    
    /*!
     Load an obj model from a file
     */
    bool load_obj(const char* filename, vector<glm::vec3> &vertices, vector<glm::vec3> &normals, vector<GLushort> &elements);

    
    /*!
     Create the vertex buffer object for this element
     */
    void initVBO(void);
    
    
    
    // The data
    vector<glm::vec3>       _vertices;
    vector<glm::vec3>       _vertices2;
    vector<glm::vec4>       _vertex_colors;
    vector<glm::vec3>       _normals;
    vector<GLushort>        _elements;
    
    string                  _material_file;
    string                  _model_name;
    
    
    unsigned int            _vaoID[1]; // Our Vertex Array Object
    
    unsigned int            _vboID[3]; // Our Vertex Buffer Object
};