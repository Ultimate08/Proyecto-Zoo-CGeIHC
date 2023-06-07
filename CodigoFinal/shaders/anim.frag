#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture1;
uniform vec4 colorAlpha;

void main()
{
 
    vec4 texColor = colorAlpha*texture(texture1,TexCoords);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;


}