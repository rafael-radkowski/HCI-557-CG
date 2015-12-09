//
//  main_picking.cpp
//  HCI 557 Picking example
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>
#include <vector>



// GLM include files
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


using namespace std;



/*!
@brief - calculate the eigenvectors for a given point using the points in its surrounding.
@param centroid - the centroid of the eigenvectors
@param points - all the other points
@param eigenvectors - a 4x4 matrix which column vectors are the eigen vectors
*/
//static
void calcEigenvectors(const glm::vec3& centroid, const vector<glm::vec3>& points, vector<glm::vec3>& eigenvectors);



/*!
@brief Function to calculate all eigenvectors for all points
@param n,m, the number of points in the grid
@param up, specify a general up-vector for this surface. This is requried to align all the normal vectors.
@param points - all points
@param eigenvectors_per_point - the result eigenvectors per point. The index of each matchs complies with the point index.
*/
void computeReferenceFrames( const int n, const int m, glm::vec3 up,  const vector< vector<float> >& points, vector< vector<glm::vec3> >& eigenvector_per_point );


/*!
@brief - transfers an eigenvector into a matrix in homogenous coordinates.
@param eigenvectors  - vector with eigenvectors sorted corresponding to the eigenvalues, largest first ev1 > ev2 > ev3
@param matrix - the glm matrix.
Notice that all matrix types are column-major rather than row-major.
*/
void Eigenvec2matrix(const vector< glm::vec3 >& eigenvectors, glm::mat4& matrix);


/*!
Calculates the factorial of a number
@param x - the value that should be factorized.
@return - the factor.
*/
int factorial(int x, int result = 1);



/*!
Calculate the binominal coefficient for n and k
@param n - the number of line segments where the number of supporting points is n+1
@param k - the current point
*/
int BinomialCoeff(int n, int k);



/*!
Calculate the bernstein polynom for P(x) = BC(n,k)  x^k (1-x)^{n-k}
@param x - the interpolation variable x = [0,1]
@param n - the number of line segments where the number of supporting points is n+1
@param k - the current point
*/
float ComputeBernsteinP(float x, int n, int k);



/*!
Computes the points for a cubic spline
@param control_points - a vector of four points in x,y or x,y,z. 
@param result - the output points
@param num - the number of points that should be generated along the spline
*/
bool ComputeCubicSpline(const int num, const vector< vector<float> >& control_points, vector< vector<float> >& result);

/*!
Computes the points for a cubic spline
@param control_points - a vector of four points in x,y or x,y,z.
@param result - the output points
@param num - the number of points that should be generated along the spline
*/
bool ComputeCubicSplineC(const int num, const vector< vector<float> >& control_points, vector< vector<float> >& result);


/*!
Computes a cubic surface patch.
@param points - a vector of four points in x,y or x,y,z.
    The  points are assumed in a grid such as this:
     
     0------1-------2-------3
     |      |       |       |
     4------5-------6-------7
     |      |       |       |
     8------9-------10------11
     |      |       |       |
     12-----13------14------15

@param result - the output points
@param num - the number of points that should be generated along the spline
*/
bool ComputeCubicPatch(const int num, const vector< vector<float> >& points, vector< vector<float> >& result);



/*!
Computes / reorganizes all points so they can be used as triangles to render a mesh
@param n, m, the size of the mesh in number of points as columns n and rows m
@param control_points, control points as vec3 <float> [x, y, x].

                m
     0------1-------2-------3
     |      |       |       |
     4------5-------6-------7
n    |      |       |       |
     8------9-------10------11
     |      |       |       |
     12-----13------14------15

@param up - specify a general up direction for your surface. The surface normal vectors are sometimes flipped. 
    In order to figure out which is the "right" direction, specify what is in general ok.
@param vertices, the vertices in an vector. Three vertices in a row are one triangle
@param normals, a normal vector for each point
*/
bool TriangulateCubicPatch(const int n, const int m, const vector< vector<float> >& control_points, const glm::vec3 up, vector< glm::vec3 >& vertices, vector< glm::vec3 >& normals );




