#version 410 core                                                 
                                                            
uniform mat4 projectionMatrix;                                   
uniform mat4 viewMatrix;                                         
uniform mat4 modelMatrix;                                         
in vec3 in_Position;                                               
in vec3 in_Normal;            

// Position of the light source
uniform vec3 locationLight;  

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
  

out vec3 pass_Color;     


void main(void)                                                  
{  
    vec3 normal =   vec3( transpose(inverse(modelMatrix))  * vec4(in_Normal, 0.0) );
    vec3 transformedNormal =  normalize ( normal);

    vec3 surface_point = vec3( modelMatrix * vec4(in_Position, 1.0));     
    vec3 surface_to_light =  normalize(locationLight  - surface_point) ;       
  
    // Diffuse color   
    // Note that the dot product uses vec3 and not vec4. A cos of a vec4 is undefined.                                                                                       
    float diffuse_coefficient = max( dot(transformedNormal, surface_to_light), 0.0);                         
    vec3 out_diffuse_color = diffusecolor  * diffuse_coefficient * diffuseIntensity;   

     // Ambient color                                                                                         
    vec3 out_ambient_color = vec3(ambientcolor) * ambientIntensity;

    // specular color        
    vec3 light =  (locationLight  - surface_point) ;    
    vec3 reflectDir = normalize(reflect(  light, transformedNormal));   
    //vec3 cameraPosition = vec3( -viewMatrix[3][0], -viewMatrix[3][1], -viewMatrix[3][2]);  
    vec3 cameraPosition = vec3( 0.0, 0.0, 0.0);       
    vec3 surfaceToCamera = normalize(cameraPosition - in_Position);                            
    float cosAngle =  max( dot( reflectDir, surfaceToCamera), 0.0);   
    float specular_coefficient = pow(cosAngle, speculuar_coeff);                                                     
    vec3 out_specular_color = specularColor * specular_coefficient * specular_intensity;            

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0); 
	//pass_Color =  max( out_specular_color + out_diffuse_color,  out_ambient_color);
    pass_Color =  out_specular_color + out_diffuse_color + out_ambient_color;
}                                                                 