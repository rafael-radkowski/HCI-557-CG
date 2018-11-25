#version 410 core     


uniform mat4 projectionMatrix;                                    
uniform mat4 viewMatrix;                                           
uniform mat4 modelMatrix;  
                                                                   
in vec3 pass_Normal;     
in vec3 pass_Position;   
in vec4 pass_Color;   

uniform vec3 light_position;
uniform vec3 light_direction;

uniform vec3  diffColor;
uniform float diffInt;
uniform vec3  ambColor;
uniform float ambInt;
uniform vec3  specColor;
uniform float specInt;
uniform float shininess;
			                              
out vec4 color;        

                       
void main(void)                                                  
{          

	// vectors
	vec3 L = normalize( light_position - pass_Position ); 
	vec3 E = normalize( vec3(viewMatrix[3][0], viewMatrix[3][1], viewMatrix[3][2]) );

	// diffuse light
	vec3 Idiff =  diffInt * diffColor *  max(dot(L, pass_Normal), 0.0); 
	Idiff = clamp(Idiff, 0.0, 1.0); 

	// ambient light
	vec4 Iamb = vec4( ambColor, 1.0) * ambInt;

	// specular light
	vec3 R = reflect(L, pass_Normal);
	vec3 Ispec =  specInt * specColor *  pow(max(dot(R, E), 0.0), shininess); 
    
	                                             
	color = max(vec4(Idiff + Ispec, 1.0), Iamb) ;                            
}                                                      