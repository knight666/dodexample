#version 330 core

uniform VertexUniforms
{
	mat4 modelViewProjection;
} Uniforms;

in vec3 attrPosition;
in vec4 attrColor;

out vec4 vertexColor;

void main()
{
	gl_Position = Uniforms.modelViewProjection * vec4(attrPosition, 1.0);
	vertexColor = attrColor;
}