//
//  RayIntersectionTest.cpp
//  HCI557_RayIntersectionTest
//
//  Created by Rafael Radkowski on 11/3/15.
//
//

#include "RayIntersectionTest.h"



/*!
 @brief The ray intersection test method
 @param ray_start: start point of the ray in world coordinates
 @param ray_stop: end point of the ray in world coordinates
 @param object: reference to the object in world coordinates
 @param intersect_list: list with all intersection results
 */
//static
bool RayIntersectionTest::intersect(const glm::vec3& ray_start, const glm::vec3&  ray_stop, GLObjectObj& object, vector<glm::vec3>& intersect_list)
{
//#define TEST
#ifdef TEST
    glm::vec3 s(2,0,10);
    glm::vec3 e(2,0,-60);
    
    
    glm::mat4 m = glm::mat4();
    glm::mat4 mInv = glm::inverse(m);
    
    // get all verticees
    vector<glm::vec3> vertices;

    glm::vec3 v0(-5,-5,0); vertices.push_back(v0);
    glm::vec3 v1(5, -5,0); vertices.push_back(v1);
    glm::vec3 v2(2,5,0); vertices.push_back(v2);
    
  //  glm::vec3 v0(50,-5,0); vertices.push_back(v0);
   // glm::vec3 v1(100, -5,0); vertices.push_back(v1);
  //  glm::vec3 v2(70,5,0); vertices.push_back(v2);
    
#else
    
    // get the current model matrix.
    glm::mat4 m = object._modelMatrix;
    glm::mat4 mInv = glm::inverse(m);
    
    // get all verticees
    vector<glm::vec3> vertices = object._vertices;
    
    
    glm::vec3 s = ray_start;
    glm::vec3 e = ray_stop;
    
#endif

    bool ret = false;
    
    // for all vertices
    vector<glm::vec3>::iterator itr = vertices.begin();
    
    while (itr != vertices.end())
    {
        // get one vertex
        glm::vec3 p0 = (*itr);itr++;
        glm::vec3 p1 = (*itr);itr++;
        glm::vec3 p2 = (*itr);itr++;
        
     
        glm::vec4 v0 = m * glm::vec4(p0.x, p0.y, p0.z, 1.0);
        glm::vec4 v1 = m * glm::vec4(p1.x, p1.y, p1.z, 1.0);
        glm::vec4 v2 = m * glm::vec4(p2.x, p2.y, p2.z, 1.0);
        
        p0.x = v0.x; p0.y = v0.y; p0.z = v0.z;
        p1.x = v1.x; p1.y = v1.y; p1.z = v1.z;
        p2.x = v2.x; p2.y = v2.y; p2.z = v2.z;
        
        
        // prepare the matrix a;
        glm::mat3 A = glm::mat3();
        A[0][0] = s.x - e.x;  A[0][1] = p1.x - p0.x;  A[0][2] = p2.x - p0.x;
        A[1][0] = s.y - e.y;  A[1][1] = p1.y - p0.y;  A[1][2] = p2.y - p0.y;
        A[2][0] = s.z - e.z;  A[2][1] = p1.z - p0.z;  A[2][2] = p2.z - p0.z;
        
        // invert A;
        glm::mat3 invA = glm::inverse(A);
        
        // prepare the vector [t,u,v]
        glm::vec3 tuv;
        tuv.x = s.x - p0.x;
        tuv.y = s.y - p0.y;
        tuv.z = s.z - p0.z;
        
        // multiply
        glm::vec3 result = glm::transpose(invA) * tuv;

        
        float t = result.x;
        float u = result.y;
        float v = result.z;
        
        
        
        // Check if we are still within the triangle.
        if(u>=0.0 && u<= 1.0 && v >=0.0 & v<=1.0)
        {
            float uv = u + v;
#ifdef TEST
            cout << p0.x << " : " << p0.y << " : " << p0.z << endl;
            cout << p1.x << " : " << p1.y << " : " << p1.z << endl;
            cout << p2.x << " : " << p2.y << " : " << p2.z << endl;
            cout << u << " : " << v << " : " << t << endl;
#endif
            
            if(uv >= 0.0 && uv <= 1.0  && t >= 0.0 && t <= 1.0)
            {

                // intersetcion
                glm::vec3 x = s + (e - s)*t;
        
                // cout << x.x << " : " << x.y << " : " << x.z << endl;
                //cout << t << " ->  "<< x.x << " : " << x.y << " : " << x.z << endl;
                intersect_list.push_back(x);
                
                ret = true;
            }
        }
    }
    
    return ret;
    
}