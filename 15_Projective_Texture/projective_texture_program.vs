#version 410 core                                                 
                                                                   
uniform mat4 projectionMatrix;                                    
uniform mat4 viewMatrix;                                           
uniform mat4 modelMatrix;    


uniform mat4 camera_ProjectionMatrix;                                    
uniform mat4 camera_ViewMatrix;    
uniform mat4 camera_ModelMatrix;        
                                      
in vec3 in_Position;                                              
in vec2 in_Texture;                                                
in vec3 in_Normal;     
                                             
out vec3 pass_Normal;                                             
out vec2 pass_Texture;
out vec4 pass_Position;
out vec3 pass_PosWorld;
                                                                 
void main(void)                                                   
{  

    // for projective texturing
    pass_Position =   camera_ProjectionMatrix *  camera_ViewMatrix * modelMatrix * vec4( in_Position, 1.0);    

    // for light
    pass_PosWorld = vec3( modelMatrix * vec4(in_Position, 1.0) );  
    pass_Normal = normalize( vec3(  transpose(inverse(  modelMatrix))  * vec4( in_Normal, 0.0 )));;                                        
    
    // unused
    pass_Texture = in_Texture;   

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  

}                                                                 