#version 330 core

uniform sampler2D tex; //this is the texture

in vec2 pass_TexCoord; //this is the texture coord
in vec4 pass_Color;
out vec4 color;

uniform int texture_blend;



float mip_map_level(in vec2 texture_coordinate) // in texel units
{
    
    vec2 dx_vtc = dFdx(texture_coordinate);
    vec2 dy_vtc = dFdy(texture_coordinate);
    float delta_max_sqr = max(dot(dx_vtc, dx_vtc), dot(dy_vtc, dy_vtc)); float mml = 0.5 * log2(delta_max_sqr);
    return max( 0, mml );
}


void main(void)                                                   
{
    // This calculates the current midmap level/
    // This only works for GLSL > 4.0. See the code for 3.3
    float mipmapLevel = mip_map_level(pass_TexCoord);
    
    // CHANGE THE LEVEL MANUALLY TO SEE THE EFFECT.
    // mipmapLevel = 4;

    // This function finds the color component for each texture coordinate.
    vec4 tex_color =  textureLod(tex, pass_TexCoord, mipmapLevel);
    
    // This mixes the background color with the texture color.
    // The GLSL shader code replaces the former envrionment. It is now up to us
    // to figure out how we like to blend a texture with the background color.
    
    color = 0.10* pass_Color + tex_color;
      
}