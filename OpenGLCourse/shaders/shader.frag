#version 330
in vec4 vColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D texture0;

void main() 
{
	color = texture(texture0, TexCoord) * vColor;
}