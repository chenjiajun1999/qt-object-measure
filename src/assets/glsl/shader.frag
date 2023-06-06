#version 330 core
in highp vec2 texCoord;
uniform sampler2D texType;

void main(void)
{
    gl_FragColor = texture2D(texType, texCoord.st);
}