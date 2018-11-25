#version 410 core                                                 
                                                                 
uniform mat4 projectionMatrix;                                    
uniform mat4 viewMatrix;                                           
uniform mat4 modelMatrix;  

// this is the model-view-projection matrix that we used to create the shadow map
uniform mat4 shadow_map_p;  
uniform mat4 shadow_map_v;  

in vec3 in_Position;                                                                                                           
in vec3 in_Normal;   
in vec2 in_Texture;          
                                       
out vec3 pass_Color;
out vec3 pass_Normal;
out vec2 pass_Texture; 

// the position to compare with the shadow map
out vec4 pass_Position; 

                                                                
void main(void)                                                  
{                                                               
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0); 

    pass_Normal = vec3( transpose(inverse(modelMatrix))  * vec4(in_Normal, 0.0) ); 

	pass_Color = vec3(0.6, 0.6, 0.6);   

	pass_Texture = in_Texture;    

	pass_Position = shadow_map_p * shadow_map_v * modelMatrix * vec4(in_Position, 1.0);                      
}                                                    