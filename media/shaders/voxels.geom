#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 36) out;

uniform VertexUniforms
{
	mat4 modelViewProjection;
} Uniforms;

uniform float halfSize;

in vec3 vertexColor[];

out vec3 geometryColor;

void main()
{
	for (int i = 0; i < gl_in.length(); ++i)
	{
		geometryColor = vertexColor[i];

		// Left

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4(-halfSize,  halfSize,  halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4(-halfSize, -halfSize,  halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4( halfSize,  halfSize,  halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4( halfSize, -halfSize,  halfSize, 0.0));
		EmitVertex();

		EndPrimitive();

		// Right

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4( halfSize,  halfSize, -halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4( halfSize, -halfSize, -halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4(-halfSize,  halfSize, -halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4(-halfSize, -halfSize, -halfSize, 0.0));
		EmitVertex();

		EndPrimitive();

		// Top

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4( halfSize,  halfSize,  halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4( halfSize,  halfSize, -halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4(-halfSize,  halfSize,  halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4(-halfSize,  halfSize, -halfSize, 0.0));
		EmitVertex();

		EndPrimitive();

		// Bottom

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4(-halfSize, -halfSize,  halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4(-halfSize, -halfSize, -halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4( halfSize, -halfSize,  halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4( halfSize, -halfSize, -halfSize, 0.0));
		EmitVertex();

		EndPrimitive();

		// Front

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4( halfSize, -halfSize,  halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4( halfSize, -halfSize, -halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4( halfSize,  halfSize,  halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4( halfSize,  halfSize, -halfSize, 0.0));
		EmitVertex();

		EndPrimitive();

		// Back

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4(-halfSize,  halfSize,  halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4(-halfSize,  halfSize, -halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4(-halfSize, -halfSize,  halfSize, 0.0));
		EmitVertex();

		gl_Position = Uniforms.modelViewProjection * (gl_in[i].gl_Position + vec4(-halfSize, -halfSize, -halfSize, 0.0));
		EmitVertex();

		EndPrimitive();
	}
}