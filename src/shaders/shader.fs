
#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;
vec2 changedTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixFloat;

void main()
{
    changedTexCoord = vec2(TexCoord.x,-TexCoord.y);
    FragColor = mix(texture(texture1,changedTexCoord),
                    texture(texture2, changedTexCoord),mixFloat);
                    
}