#version 330 core

uniform sampler2D texDiffuse;

in vertexOut
{
	vec2 TextureCoordinate;
} In;

out vec4 Color;

void main()
{
	Color = texture(texDiffuse, In.TextureCoordinate);
}