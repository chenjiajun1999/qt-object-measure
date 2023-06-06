#version 330 core
attribute highp vec3 aPos;
attribute highp vec2 aTexCoord;
out highp vec2 texCoord;

void main(void)
{
    texCoord = vec2(aTexCoord.x,aTexCoord.y);
    gl_Position = vec4(aPos,1.0);
}