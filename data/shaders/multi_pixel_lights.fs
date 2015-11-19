#version 410 core                                                 

uniform sampler2D texture_background; //this is the texture
uniform sampler2D texture_foreground; //this is the texture

in vec2 pass_TexCoord; //this is the texture coord
in vec4 pass_Color;
out vec4 color;

uniform int texture_blend;
uniform float texture_delta;

void main(void)                                                   
{
    
    vec4 tex_displacement =  texture(texture_foreground, pass_TexCoord);
    
    vec2 displacement = texture_delta * tex_displacement.xy;
    
    
    // This function finds the color component for each texture coordinate. 
    vec4 tex_color =  texture(texture_background, pass_TexCoord + displacement);
    
    
    

    color = 0.1 * pass_Color + tex_color;
    

}