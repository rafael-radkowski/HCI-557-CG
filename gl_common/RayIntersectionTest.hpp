//
//  RayIntersectionTest.hpp
//  HCI557_RayIntersectionTest
//
//  Created by Rafael Radkowski on 11/3/15.
//
//
#pragma once

// stl include
#include <iostream>
#include <string>
#include <vector>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>




#include "GLObjectObj.h"


class RayIntersectionTest
{
public:
    
    /*!
     @brief The ray intersection test method
     @param ray_start: start point of the ray in world coordinates
     @param ray_stop: end point of the ray in world coordinates
     @param object: reference to the object in world coordinates
     @param intersect_list: list with all intersection results
     */
    static bool intersect(const glm::vec3& ray_start, const glm::vec3&  ray_stop, GLObjectObj& object, vector<glm::vec3>& intersect_list);


};