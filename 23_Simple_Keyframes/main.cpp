//
//  main.cpp
//
//  This is example code for HCI/ME/CS/CprE 557 Computer Graphics
//
//  Keyframes
//  The purpose of this code is to introduce and discuss keyframes and keyframe animations.
//  IT DOES NOT RENDER ANY OBJECTS. IT OPENS AN EMPTY GLFW WINDOW.
//
//  The code shows
//  - how a keyframe and a keyframe animation can be represented with glm
//  - how to find the two closest keyframes for a given time and an give animation interval
//  - how to interpolate between two keyframes using glm / linear interpolation and slerp.
//
//  Created by Rafael Radkowski on 5/28/15.
//  Iowa State University
//
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>
#include <map>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE 
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

// GLFW include
#include <GLFW/glfw3.h>


using namespace std;
using namespace glm;


// The main window
GLFWwindow* window;


/*!
 A struct to define keyframe
 */
typedef struct _keyframe
{
    float               _t; // the time fraction
    glm::vec3           _p; // the position
    glm::quat           _q; // the orientation
    
    /*
     Constructor
     */
    _keyframe(float t, glm::vec3 p, glm::quat q)
    {
        _t = t;
        _p = p;
        _q = q;
    }
    
    /*
     Default constructor
     */
    _keyframe()
    {
        _t = -1.0;
        _p = glm::vec3(0.0,0.0,0.0);
        _q = glm::quat(0.0,0.0,0.0,0.0);
    }
    
    // prints the data into a terminal
    void print(void)
    {
        cout << "t: " << _t << "\tp: " << _p.x << ", " << _p.y << ", " << _p.z << "\tq: " << _q.x  << ", " << _q.y  << ", " << _q.z  << ", " << _q.w  << endl;
    }

}Keyframe;


/*
 Type for the keyframe animation
 */
typedef map<double, Keyframe> KeyframeAnimation;

// Variable to store the keyframes
KeyframeAnimation myKeyframes;


/*!
 @brief returns the time fraction for a given time and animation duration
 @param time - the current animation time, application runtime, etc. in seconds
 @param duration - the duration of the animation in seconds
 @return the time fraction in an interval between 0 and 1.
 */
float getTimeFraction(const float time, const float duration)
{
    // we cast to an int. this results in the number of
    float interval = floor(time/duration);
    
    // return the current interval time
    float current_interval = time - interval*duration;
    
    // return the fraction / position in our current timeline
    float fraction = current_interval / duration;

    return fraction;
}



/*!
 @brief returns the two keyframes for a given time. 
 @param keyframes - a map with all keyframes of type KeyframeAnimation
 @param time - the time fraction between 0 and 1. 
 @param k0, reference to the first keyframe
 @param k2, reference to the second keyframe
 @return the number of keyframes. 1 if the time is equal to a keyframe, otherwise 2.
 */
int getKeyframes( KeyframeAnimation& keyframes, const double time, Keyframe& k0, Keyframe& k1)
{
    int num_keyframes = 0;
    
    // get a keyframe iterator
    KeyframeAnimation::iterator k_itr = keyframes.lower_bound(time);
    
    Keyframe k0_temp, k1_temp;
    
    // Obtain the first keyframe
    k1 = (*k_itr).second; num_keyframes++;
    
    
    // Check whether we are not at the beginning of this map
    if(k_itr != keyframes.begin())
    {
        k_itr--;  // decrement
        k0 = (*k_itr).second; // obtain the second keyframe
        num_keyframes++;
    }
  
    // write the first keyframe into k0 if we only have one
    if(num_keyframes == 1)
    {
        k0 = k1;
    }
  
    return num_keyframes;
    
}

/*!
 @brief Interpolate between two keyframes
 @param fraction - the time fraction for the interpolation / the location between two keyframes.
                    The value must be between 0 and 1.
 @param k0, the start keyframe
 @param k1, the end keyframe, 
 @param res, reference to a variable for the result.
 */
bool interpolateKeyframe(const float fraction, const Keyframe& k0, const Keyframe& k1, Keyframe& res)
{
    /////////////////////////////////////////////////////////////////////////
    // 1. Check the time delta
    
    // delta time
    float delta_t = k1._t - k0._t;
    
    // Check whether we have a delta time. Otherwise, we are at the location of exactly one keyframe
    if(delta_t == 0.0f){
        res = k0;
        return true;
    }
    
    /////////////////////////////////////////////////////////////////////////
    // 2. Interpolat the position
    
    // get the delta
    glm::vec3 delta_p = k1._p - k0._p;
    
    // position interpolation
    glm::vec3 p_int =  k0._p + delta_p * (fraction- k0._t)/(delta_t);
    
    
    /////////////////////////////////////////////////////////////////////////
    // 3. Rotation interpolation
    
    // Calculate the distance between the target angle and the current angle.
    float delta_angle = sqrt((k1._q.x - k0._q.x)*(k1._q.x - k0._q.x)  +
                             (k1._q.y - k0._q.y)*(k1._q.y - k0._q.y)  +
                             (k1._q.z - k0._q.z)*(k1._q.z - k0._q.z)  +
                             (k1._q.w - k0._q.w)*(k1._q.w - k0._q.w));
    
    
    // Linear interpolation of the rotation using slerp
    glm::quat r_int = glm::slerp(k0._q, k1._q, (fraction- k0._t)/(delta_t));
    
    
    /////////////////////////////////////////////////////////////////////////
    // 4. Write the result
    res = Keyframe(fraction, p_int, r_int);
    
    return true;
}

/*!
 This initializes the keyframes.
 */
void initKeyframeAnimation(void)
{
    myKeyframes[0.0] = Keyframe(0.0, glm::vec3(0.0,0.0,0.0), angleAxis(0.0f, glm::vec3(0.0,0.0,1.0) ) );
    myKeyframes[0.5] = Keyframe(0.5, glm::vec3(0.5,0.0,0.0), angleAxis(0.57f, glm::vec3(0.0,0.0,1.0) ) );
    myKeyframes[0.7] = Keyframe(0.7, glm::vec3(0.7,0.5,0.0), angleAxis(1.28f, glm::vec3(0.0,0.0,1.0) ) );
    myKeyframes[0.8] = Keyframe(0.8, glm::vec3(0.35,0.7,0.0), angleAxis(1.53f, glm::vec3(0.0,0.0,1.0) ) );
    myKeyframes[1.0] = Keyframe(1.0, glm::vec3(0.2,0.9,0.0), angleAxis(1.98f, glm::vec3(0.0,0.1,1.0) ) );
    
  

}


int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        cout <<  "Failed to initialize GLFW\n" << endl;
        system("pause");
        return -1;
    }
    
    
    // Set the version which we expect, here, 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Simple Window", NULL, NULL);
    if( window == NULL ){
        cout <<  "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible." << endl;
        glfwTerminate();
        system("pause");
        return -1;
    }
    
    // Switch the window as the object that we work with.
    glfwMakeContextCurrent(window);
    
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        cout <<  "Failed to initialize GLEW\n" << endl;
        system("pause");
        return -1;
    }
    
    // Ensure that we can notice the escape key being pressed
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Set up our blue background color
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This inits the keyframe animation
    initKeyframeAnimation();
    
    
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 )
    {
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        
        //////////////////////////////////////////////////////////////////
        // Interpolate between keyframes
        Keyframe k0, k1, k_res;
        
        float time = glfwGetTime();
        
        float f = getTimeFraction(time, 8.0); // we assume that the animation takes 8 seconds
        
        int num = getKeyframes(myKeyframes, f, k0, k1);
        
        bool ret = interpolateKeyframe(f, k0, k1, k_res);
        
        k_res.print();
        
        //
        //////////////////////////////////////////////////////////////////
        
        // [this is the place where we need to draw some objects]
        
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    } // Check if the ESC key was pressed or the window was closed
    
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}

