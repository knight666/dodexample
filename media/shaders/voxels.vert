#version 330 core

in vec3 attrPosition;
in vec3 attrColor;

out vec3 vertexColor;

void main()
{
	gl_Position = vec4(attrPosition, 1.0);
	vertexColor = attrColor;
}