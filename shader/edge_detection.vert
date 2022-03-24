#version 450

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main()
{
	vec3 positionEye = normalize( NormalMatrix * VertexPosition );
	vec3 normalEye = normalize( NormalMatrix * VertexNormal );

	Position = positionEye;
	Normal = normalEye;
	TexCoord = VertexTexCoord;

	gl_Position = MVP * vec4(VertexPosition,1.0);
}
