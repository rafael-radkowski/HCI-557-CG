#version 410 core                                                 

uniform sampler2D tex; //this is the texture

in vec2 pass_TexCoord; //this is the texture coord
in vec4 pass_Color;
out vec4 color;

uniform int texture_blend;



//-----------------------------------------------------------------------------
// FOR ENVIRONMENT MAPPING
// the vector from which we fetch our color component for environment mapping.
in vec2 vN;


void main(void)                                                   
{
    // This function finds the color component for each texture coordinate. 
    vec4 tex_color =  texture(tex, vN);
    
    // This mixes the background color with the texture color.
    // The GLSL shader code replaces the former envrionment. It is now up to us
    // to figure out how we like to blend a texture with the background color.
    if(texture_blend == 0)
    {
        color = 0.5 * pass_Color + 0.5 * tex_color;
    }
    else if(texture_blend == 1)
    {
        color = pass_Color * tex_color;
    }
    else if(texture_blend == 2)
    {
        color = (1-pass_Color.w)*pass_Color + tex_color;
    }
    else
    {
        color = pass_Color + tex_color;
    }
    
}