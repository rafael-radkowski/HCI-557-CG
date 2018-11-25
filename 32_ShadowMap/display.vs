#version 410 core                                                 
                                                                 
uniform mat4 projectionMatrix;                                    
uniform mat4 viewMatrix;                                           
uniform mat4 modelMatrix;  

uniform float display_scale;

in vec3 in_Position;                                                                                                           
in vec3 in_Normal;   
in vec2 in_Texture;          
                                       
out vec3 pass_Color;
out vec3 pass_Normal;
out vec2 pass_Texture;                                               
                                                                
void main(void)                                                  
{                                                               
	gl_Position = vec4(in_Position, 1.0) * vec4(0.3, 0.3, 1.0, 1.0) -  vec4( display_scale * 0.35, display_scale * 0.35, 0.0, 0.0); 
    pass_Normal = in_Normal.xyz;// vec3( transpose(inverse(modelMatrix))  * vec4(in_Normal, 0.0) );   
	pass_Color = vec3(0.6, 0.6, 0.6);    
	pass_Texture = in_Texture;                              
}                                                    