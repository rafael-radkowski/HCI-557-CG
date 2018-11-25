#version 410 core                                             
                                                                   
in vec3 pass_Color;     
in vec2 pass_Texture;  
in vec3 pass_Normal;

// this on contains the shadow map
uniform sampler2D tex;  
                                        
// the position to compare with the shadow map
in vec4 pass_Position; 

out vec4 color;   


                           
void main(void)                                                  
{                    


	// Column major order             
	const mat4 corr = mat4( 0.5, 0.0, 0.0, 0.0, 
							0.0, 0.5, 0.0, 0.0,
				 			0.0, 0.0, 0.5, 0.0,
				 			0.5, 0.5, 0.5, 0.0);  

	// Normalize the position 
	vec4 norm = ( pass_Position / pass_Position.w ); // get all values into a range [-1, 1] 
	norm = corr * norm; // transforms all values into a range [0,1]

	// get the shadow map depth value
	float shadow_dist =  texture(tex, norm.xy).x;

	// compare it with the value stored in the shadow map. 
	float visibility = 1.0;
	if( shadow_dist < norm.z )
		visibility = 0.3;
	                                            
	color =   vec4( 0.8, 0.8, 0.8, 1.0) * visibility + (1-visibility) * vec4(0.0, 0.0, 0.0, 1.0);                 
}                                                              