#version 410 core                                                 
                                                                   
uniform mat4 projectionMatrix;                                    
uniform mat4 viewMatrix;                                           
uniform mat4 modelMatrix;    
                                      
in vec3 in_Position;                                              
in vec2 in_Texture;                                                
in vec3 in_Normal;     

out vec3 pass_Position;                                             
out vec3 pass_Normal;                                             
out vec2 pass_Texture;

                                                                 
void main(void)                                                   
{                                        
    pass_Texture = in_Texture;  
    
    pass_Normal = vec3(  transpose(inverse(  modelMatrix))  * vec4( in_Normal, 0.0 ));
    pass_Position = vec3(modelMatrix * vec4(in_Position, 1.0));
                                                       
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  

}                                                                 