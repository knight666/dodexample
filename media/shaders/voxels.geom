#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 36) out;

uniform float halfSize;

in vec4 vertexColor[];

out vec4 geometryColor;

void main()
{
	for (int i = 0; i < gl_in.length(); ++i)
	{
		vec4 boxMinimum = gl_in[i].gl_Position + vec4(vec3(-halfSize), 0.0);
		vec4 boxMaximum = gl_in[i].gl_Position + vec4(vec3(halfSize), 0.0);

		gl_Position = boxMinimum;
		geometryColor = vertexColor[i];
		EmitVertex();

		gl_Position = vec4(boxMaximum.x, boxMinimum.y, boxMinimum.z, 1.0);
		geometryColor = vertexColor[i];
		EmitVertex();

		gl_Position = vec4(boxMaximum.xy, boxMinimum.z, 1.0);
		geometryColor = vertexColor[i];
		EmitVertex();
	}

	EndPrimitive();
}