#version 330 core

uniform vertexUniforms
{
	mat4 modelViewProjection;
} Uniforms;

in vec3 attrPosition;

void main()
{
	gl_Position = Uniforms.modelViewProjection * vec4(attrPosition, 1.0);
}