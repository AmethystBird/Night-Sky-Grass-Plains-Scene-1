#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec4 position;
out vec3 normal;
out vec2 textureCoordinates;
out vec3 vertexPositionFrag;
out vec4 projectedTextureCoordinates;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
//uniform mat4 RotationMatrix;

//For projected texture
uniform mat4 ProjectorMatrix;
uniform mat4 ModelMatrix;

uniform mat4 MVP;

//Conversion of vertex normal to eye coordinates
//Inference: Transforms object into correct position in relation to camera position
void GetCameraSpaceValues(out vec3 normal, out vec3 position)
{
	normal = normalize(NormalMatrix * VertexNormal);
	position = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;
}

void main()
{
	textureCoordinates = VertexTexCoord;
	GetCameraSpaceValues(normal, position.xyz);

	projectedTextureCoordinates = ProjectorMatrix * (ModelMatrix * vec4(VertexPosition, 1.0));
	//projectedTextureCoordinates = ProjectorMatrix * (ModelMatrix * position); //projected texture coordinates

	vertexPositionFrag = VertexPosition;
	gl_Position = MVP * vec4(VertexPosition, 1.0);// * RotationMatrix; //multiply by rotation offset
}