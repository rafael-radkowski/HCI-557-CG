#pragma once

void computeMatricesFromInputs();



glm::mat4 getViewMatrix();


glm::mat4 getProjectionMatrix();




/*
 Implementation of a trakball
 */
class Trackball {
    
public:
    Trackball( int window_width, int window_height, GLfloat roll_speed = 0.5f, bool x_axis = true, bool y_axis = true );
    glm::vec3 toScreenCoord( double x, double y );
    
    void mouseButtonCallback( GLFWwindow * window, int button, int action, int mods );
    void cursorCallback( GLFWwindow *window, double x, double y );
    
    
    /*
     * Create a rotation matrix within the camera coordinate,
     */
    glm::mat4 getRotationMatrix();
    
    
    glm::mat4 createModelRotationMatrix( glm::mat4& view_matrix );
    
private:
    
    int     _windowWidth;
    int     _windowHeight;
    int     _mouseEvent;
    
    GLfloat _rollingSpeed;
    GLfloat _current_angle ;
    
    glm::vec3   _prevPos;
    glm::vec3   _currPos;
    glm::vec3   _camAxis;
    
	glm::mat4	_last_rotation;

    bool        _xAxis;
    bool        _yAxis;
    
};


