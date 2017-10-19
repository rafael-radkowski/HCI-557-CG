#version 410 core                                                 


in vec4 pass_Color;
out vec4 color;   

void main(void)                                                   
{                                                                 
    color = vec4(pass_Color);
}                           