#version 410 core                                                 

// Transformations for the projections
uniform mat4 projectionMatrixBox;                                    
uniform mat4 viewMatrixBox;                                          
uniform mat4 modelMatrixBox;     

// The material parameters 
uniform vec3 diffuse_color;                                        
uniform vec3 ambient_color;                                         
uniform vec3 specular_color;   

// Position of the light source
uniform vec3 light_position;   

// The intensity values for the reflection equations
uniform float diffuse_intensity;                                          
uniform float ambient_intensity;                                        
uniform float specular_intensity;                                       
uniform float shininess;                                        
                                             
// The vertex buffer input                                    
in vec3 in_Color; 
in vec3 in_Position; 
in vec3 in_Normal;  

// The output color
out vec3 pass_Color;                                            
                                                                 
                                                               
                                                                                                                               
void main(void)                                                 
{                                                               
	vec3 normal = normalize(in_Normal);                                                                   
    vec4 transformedNormal =  normalize(transpose(inverse(modelMatrixBox)) * vec4( normal, 1.0 ));            
    vec4 surfacePostion = modelMatrixBox * vec4(in_Position, 1.0);                          
                                                                                                            
    vec4 surface_to_light =   normalize( vec4(light_position,1.0) -  surfacePostion );                      
                                                                                                            
    // Diffuse color                                                                                          
    float diffuse_coefficient = max( dot(transformedNormal, surface_to_light), 0.0);                         
    vec3 out_diffuse_color = diffuse_color  * diffuse_coefficient * diffuse_intensity;                        
                                                                                                              
    // Ambient color                                                                                         
    vec3 out_ambient_color = vec3(ambient_color) * ambient_intensity;                                        
                                                                                                             
    // Specular color                                                                                        
    vec3 incidenceVector = -surface_to_light.xyz;                                                             
    vec3 reflectionVector = reflect(incidenceVector, transformedNormal.xyz);                                   
    vec3 cameraPosition = vec3( -viewMatrixBox[3][0], -viewMatrixBox[3][1], -viewMatrixBox[3][2]);            
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePostion.xyz);                                   
    float cosAngle = max( dot(surfaceToCamera, reflectionVector), 0.0);                                       
    float specular_coefficient = pow(cosAngle, shininess);                                                     
    vec3 out_specular_color = specular_color * specular_coefficient * specular_intensity;                    
        
	// Writes the color
	pass_Color = vec3(out_diffuse_color + out_ambient_color + out_specular_color);  
	
	
	// Passes the projected position to the fragment shader / rasterization process. 
    gl_Position = projectionMatrixBox * viewMatrixBox * modelMatrixBox * vec4(in_Position, 1.0);             
                                                                                                             
                          
}                                                                                                            