#version 410 core                                                 
                                                                   
uniform mat4 projectionMatrix;                                    
uniform mat4 viewMatrix;                                           
uniform mat4 modelMatrix;    


uniform mat4 camera_ProjectionMatrix;                                    
uniform mat4 camera_ViewMatrix;    
uniform mat4 camera_ModelMatrix;    
uniform float blending_index;    
                                      
in vec3 in_Position;                                              
in vec2 in_Texture;                                                
in vec3 in_Normal;   

in vec3 in_Position2;                                              
in vec2 in_Texture2;                                                
in vec3 in_Normal2;   
                                             
out vec3 pass_Normal;                                             
out vec2 pass_Texture;

out vec3 pass_PosWorld;

                                                                 
void main(void)                                                   
{  
    // Calculate the delta position and the delta normal for morphing
    vec3 final_position = in_Position + (in_Position2 - in_Position  ) * blending_index;
    vec3 final_normal = in_Normal + (in_Normal2 - in_Normal ) * blending_index;

    // for light
    pass_PosWorld = vec3( modelMatrix * vec4(final_position, 1.0) );  
    pass_Normal = normalize( vec3(  transpose(inverse(  modelMatrix))  * vec4( final_normal, 0.0 )));;                                        

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(final_position, 1.0);  

}                                                                 