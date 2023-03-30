#version 330
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vColor;
out vec2 TexCoord;

void main()
{
	vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	TexCoord = tex;

	gl_Position = projection * view * model * vec4(pos, 1.0f);
}