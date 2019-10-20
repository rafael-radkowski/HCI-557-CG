#pragma once


// stl
#include <iostream>
#include <string>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>  // transformation
#include <glm/gtx/quaternion.hpp> // quaternions


using namespace std;


namespace cs557 {


inline void printMat4(glm::mat4& m) {
	
	for (int i = 0; i < 4; i++) {
		cout << m[i][0] << "\t" << m[i][1] << "\t" << m[i][2] << "\t" << m[i][3] << "\n";
	}
	cout << "\n";
}

}