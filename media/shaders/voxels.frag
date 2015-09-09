#version 330 core

in vec4 vertexColor;

out vec4 fragmentColor;

void main()
{
	//fragmentColor = vertexColor;
	fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
}