#version 410 core                                                 

uniform sampler2D tex; //this is the texture

in vec2 pass_TexCoord; //this is the texture coord
in vec4 pass_Color;
out vec4 color;

uniform int texture_blend;

void main(void)                                                   
{
    // This calculates the current midmap level/
    // This only works for GLSL > 4.0. See the code for 3.3
    float mipmapLevel = textureQueryLod(tex, pass_TexCoord).x;
    
    // CHANGE THE LEVEL MANUALLY TO SEE THE EFFECT.
    //mipmapLevel = 4;

    // This function finds the color component for each texture coordinate.
    vec4 tex_color =  textureLod(tex, pass_TexCoord, mipmapLevel);
    
    // This mixes the background color with the texture color.
    // The GLSL shader code replaces the former envrionment. It is now up to us
    // to figure out how we like to blend a texture with the background color.
    
    color = 0.10* pass_Color + tex_color;
    
    
}