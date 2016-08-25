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
#include <math.h>
#include <map>


#include <Eigen/Dense>
#include <Eigen/Eigenvalues> 

#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>


#include "Bezier.h"

using namespace std;

// stores all binomnal coefficents that have been calculated in this program.
// [  n , [k, coeff] ]
map<unsigned int, map< unsigned int,  unsigned int> > binominal_coefficients;





bool eigenvaluessort(pair<float, Eigen::Vector3f > i, pair<float, Eigen::Vector3f > j)
{
    return (i.first > j.first); // largest first
}


/*!
@brief - calculate the eigenvectors for a given point using the points in its surrounding.
@param centroid - the centroid of the eigenvectors
@param points - all the other points
@param eigenvectors - a 4x4 matrix which column vectors are the eigen vectors
*/
//static
void calcEigenvectors(const glm::vec3& centroid, const vector<glm::vec3>& points, vector<glm::vec3>& eigenvectors)
{
    
    
    Eigen::Matrix3f cov;
    cov << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;
    
    size_t size = points.size();
    for (int i=0; i<size; i++) {
        
        glm::vec3 p = points[i];
        cov(0,0) += (p[0] - centroid[0] ) * (p[0] - centroid[0]);
        cov(0,1) += (p[0] - centroid[0] ) * (p[1] - centroid[1]);
        cov(0,2) += (p[0] - centroid[0] ) * (p[2] - centroid[2]);
        
        cov(1,0) += (p[1] - centroid[1] ) * (p[0] - centroid[0]);
        cov(1,1) += (p[1] - centroid[1] ) * (p[1] - centroid[1]);
        cov(1,2) += (p[1] - centroid[1] ) * (p[2] - centroid[2]);
        
        cov(2,0) += (p[2] - centroid[2] ) * (p[0] - centroid[0]);
        cov(2,1) += (p[2] - centroid[2] ) * (p[1] - centroid[1]);
        cov(2,2) += (p[2] - centroid[2] ) * (p[2] - centroid[2]);
    }
    
    cov(0,0) /= size;
    cov(0,1) /= size;
    cov(0,2) /= size;
    
    cov(1,0) /= size;
    cov(1,1) /= size;
    cov(1,2) /= size;
    
    cov(2,0) /= size;
    cov(2,1) /= size;
    cov(2,2) /= size;
    
    
    Eigen::EigenSolver<Eigen::Matrix3f> es(cov);
    
    
    
    // Columns are the eigenvectors
    Eigen::Matrix3f D = es.pseudoEigenvalueMatrix();
    Eigen::Matrix3f V = es.pseudoEigenvectors();
    
    Eigen::Vector3f e0( V(0), V(1), V(2));
    Eigen::Vector3f e1( V(3), V(4), V(5));
    Eigen::Vector3f e2( V(6), V(7), V(8));
    
    //Eigen::Vector3f e0( V(0), V(3), V(6));
    //Eigen::Vector3f e1( V(1), V(4), V(7));
    //Eigen::Vector3f e2( V(2), V(5), V(8));
    
    float ev0 = D(0);
    float ev1 = D(4);
    float ev2 = D(8);
    
    
    vector<pair<float, Eigen::Vector3f > > results(3);
    results[0] = make_pair(ev0, e0);
    results[1] = make_pair(ev1, e1);
    results[2] = make_pair(ev2, e2);
    
    
    // sort the vectors, largest first.
    std::sort (results.begin(), results.end(), eigenvaluessort);
    
    eigenvectors = vector<glm::vec3>(3);
    eigenvectors[0] = glm::vec3(results[0].second(0), results[0].second(1), results[0].second(2));
    eigenvectors[1] = glm::vec3(results[1].second(0), results[1].second(1), results[1].second(2));
    eigenvectors[2] = glm::vec3(results[2].second(0), results[2].second(1), results[2].second(2));
    
    
   // cout << "Covariance matrix:" << endl << cov << endl;
  //  cout << "The eigenvalues of A are:" << endl << es.eigenvalues() << endl;
  //  cout << "The matrix of eigenvectors, V, is:" << endl << es.eigenvectors() << endl << endl;
    
    
    return;
    
}



/*!
@brief Function to calculate all eigenvectors for all points
@param all_matches -  the matches for all points in "points". The index of each matchs complies with the point index.
@param points - all points
@param eigenvectors_per_point - the result eigenvectors per point. The index of each matchs complies with the point index.
*/
void computeReferenceFrames( const int n, const int m, glm::vec3 up, const vector< vector<float> >& points, vector< vector<glm::vec3> >& eigenvector_per_point )
{
    

    for(int i=0; i<m; i++)
    {
        for(int j=0; j<n; j++)
        {
            int current = i*n + j;
            vector<float> p = points[current];
            
            // find neighbors.
            vector< glm::vec3 > neighbors;
            
            const int box[8] = { -n-1, -1 ,n-1,
                                -n ,       n,
                                -n+1, +1, n+1};
            
            for(int k=0; k<8; k++)
            {
                int idx = i + box[k];
                if(idx > 0 && idx < points.size() ) neighbors.push_back( glm::vec3(points[idx][0],points[idx][1], points[idx][2] ));
            }
            
            glm::vec3 centroid(p[0], p[1], p[2]);
            vector<glm::vec3> eigenvectors;
            
            calcEigenvectors(centroid, neighbors, eigenvectors);
            
            
             ///////////////////////////////////////////////////////////////////
            //
            // Check for right-hand coordinate system.
            //
            // Calc the cross product between x, and y
            glm::vec3 cp = cross(eigenvectors[0], eigenvectors[1]);
            float angle = acos( dot(cp, eigenvectors[2]) );
        
            if( angle != angle ) // Check for NaN - tiny value, good.
            {
                // do not do anything. vector is fine.
            }
            else if(angle > 3.00)
            {
            //   cout << "replace " << i << " a: " << angle << endl;
                eigenvectors[2] = cp; //replace with cp
            }
            
            
            ///////////////////////////////////////////////////////////////////
            //
            // Verify alignment and flip the coordinate system if it is not aligned with
            // the alignment axis. (Should be the camera axis later. )
            //
            static glm::vec3 flip(-1,-1,-1);
        
            float angle_alignment = acos( dot(eigenvectors[2], up) );
    
            if(angle_alignment > 1.57)
            {
                eigenvectors[1] = flip * eigenvectors[1];
                eigenvectors[2] = flip * eigenvectors[2];
            
            }

            
            // store the eigenvectors
            eigenvector_per_point.push_back(eigenvectors);
        }
    }


}



/*!
@brief - transfers an eigenvector into a matrix in homogenous coordinates.
@param eigenvectors  - vector with eigenvectors sorted corresponding to the eigenvalues, largest first ev1 > ev2 > ev3
@param matrix - the glm matrix.
Notice that all matrix types are column-major rather than row-major.
*/
//static
void Eigenvec2matrix(const vector< glm::vec3 >& eigenvectors, glm::mat4& matrix)
{
    matrix = glm::mat4();
    
    matrix[0][0] = eigenvectors[0][0];
    matrix[0][1] = eigenvectors[0][1];
    matrix[0][2] = eigenvectors[0][2];
    
    matrix[1][0] = eigenvectors[1][0];
    matrix[1][1] = eigenvectors[1][1];
    matrix[1][2] = eigenvectors[1][2];
    
    matrix[2][0] = eigenvectors[2][0];
    matrix[2][1] = eigenvectors[2][1];
    matrix[2][2] = eigenvectors[2][2];
    
    matrix[3][0] = 0.0;
    matrix[3][1] = 0.0;
    matrix[3][2] = 0.0;

}




/*!
Calculates the factorial of a number
@param x - the value that should be factorized.
@return - the factor.
*/
int factorial(int x, int result) {
  if (x == 1  || x == 0) return result;
  else return factorial(x - 1, x * result);
}



/*!
Calculate the binominal coefficient for n and k
@param n - the number of line segments where the number of supporting points is n+1
@param k - the current point
*/
int BinomialCoeff(int n, int k)
{
    int c = binominal_coefficients[n][k];
    
    if(c==0) // binominal coefficient is min = 1, 0! = 1
    {
        c = factorial(n)/ ( factorial(k) * factorial(n-k) );
        binominal_coefficients[n][k] = c;
    
        return c;
        
    }
    return c;
    //return factorial(n)/ ( factorial(k) * factorial(n-k) );
}



/*!
Calculate the bernstein polynom for P(x) = BC(n,k)  x^k (1-x)^{n-k}
@param x - the interpolation variable x = [0,1]
@param n - the number of line segments where the number of supporting points is n+1
@param k - the current point
*/
float ComputeBernsteinP(float x, int n, int k)
{
    int c = BinomialCoeff(n,k);
    
    float p_x = float(c) * pow(x, k) *  pow((1.0-x), n-k);
    
    return p_x;
}



/*!
Computes the points for a cubic spline
@param control_points - a vector of four points in x,y or x,y,z.
@param result - the output points
@param num - the number of points that should be generated along the spline
*/
bool ComputeCubicSpline(const int num, const vector< vector<float> >& control_points, vector< vector<float> >& result)
{
    result.clear();
    
    float increment = 1.0/float(num-1);
    
    
    vector<float> cp0 = control_points[0];
    vector<float> cp1 = control_points[1];
    vector<float> cp2 = control_points[2];
    vector<float> cp3 = control_points[3];
    
    
    for (int i=0; i<num; i++) {
        
        double t = increment * i;
        
        vector<float>p(3);
        p[0] = cp0[0] * ComputeBernsteinP(t,3,0) + cp1[0] * ComputeBernsteinP(t,3,1) + cp2[0] * ComputeBernsteinP(t,3,2) + cp3[0] * ComputeBernsteinP(t,3,3);
        p[1] = cp0[1] * ComputeBernsteinP(t,3,0) + cp1[1] * ComputeBernsteinP(t,3,1) + cp2[1] * ComputeBernsteinP(t,3,2) + cp3[1] * ComputeBernsteinP(t,3,3);
        p[2] = cp0[2] * ComputeBernsteinP(t,3,0) + cp1[2] * ComputeBernsteinP(t,3,1) + cp2[2] * ComputeBernsteinP(t,3,2) + cp3[2] * ComputeBernsteinP(t,3,3);
        
        cout << t << " :\t" << p[0] << "\t" << p[1] << "\t" << p[2] << endl;
    
        result.push_back(p);
    }
    
    return true;
}



/*!
Computes the points for a cubic spline
@param control_points - a vector of four points in x,y or x,y,z.
@param result - the output points
@param num - the number of points that should be generated along the spline
*/
bool ComputeCubicSplineC(const int num, const vector< vector<float> >& control_points, vector< vector<float> >& result)
{
    result.clear();
    
    float increment = 1.0/float(num-1);
    
    
    vector<float> cp0 = control_points[0];
    vector<float> cp1 = control_points[1];
    vector<float> cp2 = control_points[2];
    vector<float> cp3 = control_points[3];
    
    // the bezier matrix
    static vector< vector<float> > bm( 4, vector<float>(4, 0.0));
    bm[0][0] = -1; bm[0][1] = 3;  bm[0][2] = -3; bm[0][3] = 1;
    bm[1][0] = 3;  bm[1][1] = -6; bm[1][2] = 3;  bm[1][3] = 0;
    bm[2][0] = -3; bm[2][1] = 3;  bm[2][2] = 0;  bm[2][3] = 0;
    bm[3][0] = 1; bm[3][1] = 0;  bm[3][2] = 0;  bm[3][3] = 0;

    
    
    
    for (int i=0; i<num; i++) {
        
        float t = increment * i;
        
        float C0 = bm[0][0] * pow(t, 3) + bm[1][0] * pow(t, 2) + bm[2][0] * t + bm[3][0];
        float C1 = bm[0][1] * pow(t, 3) + bm[1][1] * pow(t, 2) + bm[2][1] * t + bm[3][1];
        float C2 = bm[0][2] * pow(t, 3) + bm[1][2] * pow(t, 2) + bm[2][2] * t + bm[3][2];
        float C3 = bm[0][3] * pow(t, 3) + bm[1][3] * pow(t, 2) + bm[2][3] * t + bm[3][3];
        
        vector<float>p(3);
        p[0] = C0 * cp0[0] + C1 * cp1[0] + C2 * cp2[0] +  C3 * cp3[0];
        p[1] = C0 * cp0[1] + C1 * cp1[1] + C2 * cp2[1] +  C3 * cp3[1];
        p[2] = C0 * cp0[2] + C1 * cp1[2] + C2 * cp2[2] +  C3 * cp3[2];
        
        cout << t << " :\t" << p[0] << "\t" << p[1] << "\t" << p[2] << endl;
        result.push_back(p);
    }
    
    return true;
}




/*!
Computes a cubic surface patch
@param control_points - a vector of four points in x,y or x,y,z. 
@param result - the output points
@param num - the number of points that should be generated along the spline.

NOTE: This code is not fast, it is written that way for clarity. 
Usually, one would precomupute all values for BEZ * BEZ and put them into a table.

*/
bool ComputeCubicPatch(const int num, const vector< vector<float> >& control_points, vector< vector<float> >& result)
{
    result.clear();
    float increment = 1.0/float(num-1);
    
    vector<float> cp00 = control_points[0];
    vector<float> cp01 = control_points[1];
    vector<float> cp02 = control_points[2];
    vector<float> cp03 = control_points[3];
    
    
    vector<float> cp10 = control_points[4];
    vector<float> cp11 = control_points[5];
    vector<float> cp12 = control_points[6];
    vector<float> cp13 = control_points[7];
    
    vector<float> cp20 = control_points[8];
    vector<float> cp21 = control_points[9];
    vector<float> cp22 = control_points[10];
    vector<float> cp23 = control_points[11];
    
    
    vector<float> cp30 = control_points[12];
    vector<float> cp31 = control_points[13];
    vector<float> cp32 = control_points[14];
    vector<float> cp33 = control_points[15];
    
    
    // runs along the first coordinate
    for (int i=0; i<num; i++) {
        float t = increment * i;
        
        // runs along the second corrdinate
        for (int j=0; j<num; j++) {
            float v = increment * j;
            
            vector<float>p(3);
        
            // x-coordinate
            float px0 =  cp00[0] * ComputeBernsteinP(v,3,0)  * ComputeBernsteinP(t,3,0) + cp01[0] * ComputeBernsteinP(v,3,0)  * ComputeBernsteinP(t,3,1) + cp02[0] * ComputeBernsteinP(v,3,0)  * ComputeBernsteinP(t,3,2) + cp03[0] * ComputeBernsteinP(v,3,0)  * ComputeBernsteinP(t,3,3);
            float px1 =  cp10[0] * ComputeBernsteinP(v,3,1)  * ComputeBernsteinP(t,3,0) + cp11[0] * ComputeBernsteinP(v,3,1)  * ComputeBernsteinP(t,3,1) + cp12[0] * ComputeBernsteinP(v,3,1)  * ComputeBernsteinP(t,3,2) + cp13[0] * ComputeBernsteinP(v,3,1)  * ComputeBernsteinP(t,3,3);
            float px2 =  cp20[0] * ComputeBernsteinP(v,3,2)  * ComputeBernsteinP(t,3,0) + cp21[0] * ComputeBernsteinP(v,3,2)  * ComputeBernsteinP(t,3,1) + cp22[0] * ComputeBernsteinP(v,3,2)  * ComputeBernsteinP(t,3,2) + cp23[0] * ComputeBernsteinP(v,3,2)  * ComputeBernsteinP(t,3,3);
            float px3 =  cp30[0] * ComputeBernsteinP(v,3,3)  * ComputeBernsteinP(t,3,0) + cp31[0] * ComputeBernsteinP(v,3,3)  * ComputeBernsteinP(t,3,1) + cp23[0] * ComputeBernsteinP(v,3,3)  * ComputeBernsteinP(t,3,2) + cp33[0] * ComputeBernsteinP(v,3,3)  * ComputeBernsteinP(t,3,3);
            p[0] = px0 + px1 + px2 + px3;
            
            // y-coordinate
            float py0 =  cp00[1] * ComputeBernsteinP(v,3,0)  * ComputeBernsteinP(t,3,0) + cp01[1] * ComputeBernsteinP(v,3,0)  * ComputeBernsteinP(t,3,1) + cp02[1] * ComputeBernsteinP(v,3,0)  * ComputeBernsteinP(t,3,2) + cp03[1] * ComputeBernsteinP(v,3,0)  * ComputeBernsteinP(t,3,3);
            float py1 =  cp10[1] * ComputeBernsteinP(v,3,1)  * ComputeBernsteinP(t,3,0) + cp11[1] * ComputeBernsteinP(v,3,1)  * ComputeBernsteinP(t,3,1) + cp12[1] * ComputeBernsteinP(v,3,1)  * ComputeBernsteinP(t,3,2) + cp13[1] * ComputeBernsteinP(v,3,1)  * ComputeBernsteinP(t,3,3);
            float py2 =  cp20[1] * ComputeBernsteinP(v,3,2)  * ComputeBernsteinP(t,3,0) + cp21[1] * ComputeBernsteinP(v,3,2)  * ComputeBernsteinP(t,3,1) + cp22[1] * ComputeBernsteinP(v,3,2)  * ComputeBernsteinP(t,3,2) + cp23[1] * ComputeBernsteinP(v,3,2)  * ComputeBernsteinP(t,3,3);
            float py3 =  cp30[1] * ComputeBernsteinP(v,3,3)  * ComputeBernsteinP(t,3,0) + cp31[1] * ComputeBernsteinP(v,3,3)  * ComputeBernsteinP(t,3,1) + cp23[1] * ComputeBernsteinP(v,3,3)  * ComputeBernsteinP(t,3,2) + cp33[1] * ComputeBernsteinP(v,3,3)  * ComputeBernsteinP(t,3,3);
            p[1] = py0 + py1 + py2 + py3;
            
            // z-coordinate
            float pz0 =  cp00[2] * ComputeBernsteinP(v,3,0)  * ComputeBernsteinP(t,3,0) + cp01[2] * ComputeBernsteinP(v,3,0)  * ComputeBernsteinP(t,3,1) + cp02[2] * ComputeBernsteinP(v,3,0)  * ComputeBernsteinP(t,3,2) + cp03[2] * ComputeBernsteinP(v,3,0)  * ComputeBernsteinP(t,3,3);
            float pz1 =  cp10[2] * ComputeBernsteinP(v,3,1)  * ComputeBernsteinP(t,3,0) + cp11[2] * ComputeBernsteinP(v,3,1)  * ComputeBernsteinP(t,3,1) + cp12[2] * ComputeBernsteinP(v,3,1)  * ComputeBernsteinP(t,3,2) + cp13[2] * ComputeBernsteinP(v,3,1)  * ComputeBernsteinP(t,3,3);
            float pz2 =  cp20[2] * ComputeBernsteinP(v,3,2)  * ComputeBernsteinP(t,3,0) + cp21[2] * ComputeBernsteinP(v,3,2)  * ComputeBernsteinP(t,3,1) + cp22[2] * ComputeBernsteinP(v,3,2)  * ComputeBernsteinP(t,3,2) + cp23[2] * ComputeBernsteinP(v,3,2)  * ComputeBernsteinP(t,3,3);
            float pz3 =  cp30[2] * ComputeBernsteinP(v,3,3)  * ComputeBernsteinP(t,3,0) + cp31[2] * ComputeBernsteinP(v,3,3)  * ComputeBernsteinP(t,3,1) + cp23[2] * ComputeBernsteinP(v,3,3)  * ComputeBernsteinP(t,3,2) + cp33[2] * ComputeBernsteinP(v,3,3)  * ComputeBernsteinP(t,3,3);
            p[2] = pz0 + pz1 + pz2 + pz3;
        
            cout << t << ", " << v << " :\t" << p[0] << "\t" << p[1] << "\t" << p[2] << endl;
            result.push_back(p);
            
        }
    }
    
    return true;
}



/*!
Computes / reorganizes all points so they can be used as triangles to render a mesh
@param n, m, the size of the mesh in number of points as columns n and rows m
@param control_points, control points as vec3 <float> [x, y, x].

     0------1-------2-------3
     |      |       |       |
     4------5-------6-------7
     |      |       |       |
     8------9-------10------11
     |      |       |       |
     12-----13------14------15

@param vertices, the vertices in an vector. Three vertices in a row are one triangle
@param normals, a normal vector for each point
*/
bool TriangulateCubicPatch(const int n, const int m, const vector< vector<float> >& points, const glm::vec3 up, vector< glm::vec3 >& vertices, vector< glm::vec3 >& normals )
{
    int num = (n * m) - n;
    
    vertices.clear();
    normals.clear();
    
    // Here, we compute the normal vectors.
    // Note, this can also be optimized in order to speedup the process.
    // However, I compute everything in single steps so you guys can easily find and distinguish them.
    vector< vector<glm::vec3> > eigenvector_per_point;
    computeReferenceFrames( n, m, up, points, eigenvector_per_point);
    
    
    // lets find the vertices and let's associate one normal vector per vertex.
    for(int i=0; i<num; i++)
    {
        // suppose we have four control points per quad. We need two triangles per quad
        /*
            0--n--2n--3n
            | /|
            |/ |
            1--n+1
         
        */
        
        // triangle 0
        if( i%(n) < n-1 )
        {
            int idx0 = i;
            int idx1 = i+1;
            int idx2 = i + n;
            
            //cout << 0 <<": " << idx0 << "\t:" << idx1 << "\t" << idx2 << endl;
            
            vertices.push_back( glm::vec3( points[idx0][0], points[idx0][1], points[idx0][2]  ));
            vertices.push_back( glm::vec3( points[idx1][0], points[idx1][1], points[idx1][2]  ));
            vertices.push_back( glm::vec3( points[idx2][0], points[idx2][1], points[idx2][2]  ));
    
            normals.push_back(eigenvector_per_point[idx0][2]);
            normals.push_back(eigenvector_per_point[idx1][2]);
            normals.push_back(eigenvector_per_point[idx2][2]);
        }
        
        // triangle 1
        if( i%(n) < n-1 )
        {
            int idx0 = i+1;
            int idx1 = idx0+n;
            int idx2 = idx0+n-1;
            
            //cout << 1 <<": " << idx0 << "\t:" << idx1 << "\t" << idx2 << endl;
            
            vertices.push_back( glm::vec3( points[idx0][0], points[idx0][1], points[idx0][2]  ));
            vertices.push_back( glm::vec3( points[idx1][0], points[idx1][1], points[idx1][2]  ));
            vertices.push_back( glm::vec3( points[idx2][0], points[idx2][1], points[idx2][2]  ));
            
            normals.push_back(eigenvector_per_point[idx0][2]);
            normals.push_back(eigenvector_per_point[idx1][2]);
            normals.push_back(eigenvector_per_point[idx2][2]);
        }
    }
    
    
    
    return true;
}

