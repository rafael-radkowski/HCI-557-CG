#version 330 core                                                 


in vec3 pass_Color;                                                 
out vec4 color;   

void main(void)                                                   
{                                                                 
    color = vec4(pass_Color, 1.0);                               
}                           