#version 330 core

out vec4 FragColor;

in vec3 oColor;
in vec2 oTexCoord;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

void main()
{
    // FragColor = texture(uTexture1, oTexCoord) * vec4(oColor, 1.0);
    FragColor = mix(texture(uTexture1, oTexCoord), texture(uTexture2, oTexCoord), 0.7);
}