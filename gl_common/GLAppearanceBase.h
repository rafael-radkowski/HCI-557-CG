//
//  GLAppearance.hpp
//  HCI557_Spotlight
//
//  Created by Rafael Radkowski on 9/26/15.
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


using namespace std;


class GLVariable
{
public:
    GLVariable(){}
    
    virtual bool addVariablesToProgram(GLuint program, int variable_index = -1) = 0;
    
    
    
    virtual bool dirty(GLuint program) = 0;
    
protected:
    /*!
     Checks whether a uniform variable has been correctly added.
     @param idx - the unifrom locaiton
     @param name - the name of the uniform variable.
     */
    bool checkUniform(int idx, string name);
};




