/*
@file: per_fragment_light.fs
@brief: Per-fragment light shader for one light. The fragment shader part

This is example code for ME/CS/CPRE 557 - Computer Graphics. 

Rafael Radkowski
Iowa State University
November 2018
rafael@iastate.edu

MIT License
------------------------------------------------
Last changes:

Oct 10, 2019, RR
- Fixed a bug, a vector was flipped.  
*/

#version 410 core    

// The view matrix. We use it to get the light location into the camera coordinate space / 
// world coordinate space.
uniform mat4 viewMatrix; 

// The next ones are unifrom variables which we use to 
// pass the material and light information to this code. 

// Position of the light source
uniform vec3 locationLight; 

// Direction of the light
uniform vec3 directionLight;

// Cutoff angles
uniform float cutoffAngle;
uniform float cutoffBlend;

// Diffuse light
uniform vec3 diffusecolor;   
uniform float diffuseIntensity;   

// Specular light
uniform vec3 specularColor;  
uniform float speculuar_coeff;
uniform float specular_intensity;  

// Ambient light
uniform vec3 ambientcolor;
uniform float ambientIntensity; 

// the incoming variables
smooth in vec3 pass_Normal;
smooth in vec3 pass_Point;

// These are helper variables to turn on/off
// the individual light components. 
uniform int diff_on;
uniform int spec_on;
uniform int amb_on;

   
// the final color
// This is the output colr that we pass along the remaining gpu pipeline. 
out vec4 color;   

void main(void)                                                  
{   
    // light position if camera frame
	vec4 L_c = viewMatrix * vec4(locationLight, 1.0);

    // Compute the surface to light position
    vec3 surface_to_light =   normalize(  L_c.xyz - pass_Point );       
  
    // Diffuse color                                                                                          
    float diffuse_coefficient = max( dot( normalize(surface_to_light), pass_Normal), 0.0);                         
    vec3 out_diffuse_color = diffusecolor  * diffuse_coefficient * diffuseIntensity;  
    if(diff_on==0) out_diffuse_color = vec3(0.0, 0.0, 0.0);

     // Ambient color                                                                                         
    vec3 out_ambient_color = vec3(ambientcolor) * ambientIntensity;
    if(amb_on==0) out_ambient_color = vec3(0.0, 0.0, 0.0); // set the value to 0 if the component is off

    // specular color                                                          
    vec3 reflectionVector = reflect(   surface_to_light, pass_Normal);                 
    vec3 surfaceToCamera = normalize( -pass_Point );   // we are in camera coordinate space, so the camera is at (0,0,0)                            
    float cosAngle = max( -dot(  reflectionVector, surfaceToCamera), 0.0);   // set the value to 0 if the component is off  
                             
    float specular_coefficient = pow(cosAngle, speculuar_coeff);                                                     
    vec3 out_specular_color = specularColor * specular_coefficient * specular_intensity;  
    if(spec_on==0) out_specular_color = vec3(0.0, 0.0, 0.0);  // set the value to 0 if the component is off


    // Cutoff angle for the spotlight
    vec4 L_d = viewMatrix * vec4(directionLight, 1.0);
    vec3 dir  = normalize( vec3(L_c - L_d)  );
    float a = dot(surface_to_light, dir);

    // smoothstep to attenuate the light between inner and outer cutoff circle. 
    // The first parameter is the outer cutoff circle, the second value is the inner cutoff value.
    // The last parameter is the angle.
    // Note that these values need to go in as cos(cutoff angle)
    float att = smoothstep(cos(radians(cutoffAngle+ cutoffBlend)) , cos(radians(cutoffBlend)) , a); 


    // All together
    // The cutoff angle only attenuates the diffuse and specular component. 
    color = vec4( max( att * (out_specular_color + out_diffuse_color), out_ambient_color), 1.0);                               
}                                                           