#version 330

layout(location = 0) out vec4 outColour;

in vec2 TexCoord;

uniform sampler2D sTexture;

void main()
{
    outColour = texture(sTexture, TexCoord);
    //outColour = vec4(TexCoord.x, TexCoord.y, 1.0f, 1.0f);
}

