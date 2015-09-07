#version 330 core

uniform vertexUniforms
{
	mat4 modelViewProjection;
} Uniforms;

in vec2 attrPosition;
in vec2 attrTextureCoordinate;

out vertexOut
{
	vec2 TextureCoordinate;
} Out;

void main()
{	
	Out.TextureCoordinate = attrTextureCoordinate;
	gl_Position = Uniforms.modelViewProjection * vec4(attrPosition, 0.0, 1.0);
}