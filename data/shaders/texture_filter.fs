#version 410 core                                                 

uniform sampler2D tex; //this is the texture

in vec2 pass_TexCoord; //this is the texture coord
in vec4 pass_Color;
out vec4 color;

uniform int texture_blend;


const float textureSize = 512.0; //size of the texture
const float texelSize = 1.0 / textureSize; //size of one texel


vec4 texture2DBilinear( sampler2D textureSampler, vec2 uv )
{
    vec4 tl = texture(textureSampler, uv);
    vec4 tr = texture(textureSampler, uv + vec2(texelSize, 0));
    vec4 bl = texture(textureSampler, uv + vec2(0, texelSize));
    vec4 br = texture(textureSampler, uv + vec2(texelSize , texelSize));
    
    vec2 f = fract( uv.xy * textureSize ); //  the decimal part
    vec4 tA = mix( tl, tr, f.x ); //  interpolate the red dot in the image
    vec4 tB = mix( bl, br, f.x ); //  interpolate the blue dot in the image
    return mix( tA, tB, f.y ); //  interpolate the green dot in the image
}


void main(void)                                                   
{
    // This function finds the color component for each texture coordinate. 
    vec4 tex_color =  texture2DBilinear(tex, pass_TexCoord);
    
    // This mixes the background color with the texture color.
    // The GLSL shader code replaces the former envrionment. It is now up to us
    // to figure out how we like to blend a texture with the background color.
    if(texture_blend == 0)
    {
        color = pass_Color + tex_color;
    }
    else if(texture_blend == 1)
    {
        color = pass_Color * tex_color;
    }
    else if(texture_blend == 3)
    {
        color = (1-pass_Color.w)*pass_Color + tex_color;
    }
    else
    {
        color = pass_Color + tex_color;
    }
    
}