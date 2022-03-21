// a normal map fragment shader
#version 410

in vec3 vNormal;
in vec4 vPosition;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

out vec4 FragColour;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform vec3 Ka; // material ambient
uniform vec3 Kd; // material diffuse
uniform vec3 Ks; // material specular
uniform float specularPower;
uniform vec3 AmbientColour; // ambient light colour
uniform vec3 LightColour; // light colour
uniform vec3 LightDirection;
uniform vec3 cameraPosition;

void main() {
	vec3 N = normalize(vNormal);
	vec3 T = normalize(vTangent);
	vec3 B = normalize(vBiTangent);
	vec3 L = normalize(LightDirection);

	mat3 TBN = mat3(T,B,N);

	vec3 texDiffuse = texture( diffuseTexture, vTexCoord ).rgb;
	vec3 texSpecular = texture( specularTexture, vTexCoord ).rgb;
	vec3 texNormal = texture( normalTexture, vTexCoord ).rgb;
	N = TBN * (texNormal * 2 - 1);


	// calculate lambert term
	float lambertTerm = max( 0, dot( N, -L ) );
	// calculate view vector and reflection vector
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );
	// calculate specular term
	float specularTerm = pow( max( 0, dot( R, V ) ), specularPower );

	// calculate each light property
	vec3 ambient = AmbientColour * Ka * texDiffuse;
	vec3 diffuse = LightColour * Kd * texDiffuse * lambertTerm;
	vec3 specular = LightColour * Ks * texSpecular * specularTerm;

	FragColour = vec4(ambient + diffuse + specular, 1);
	
	//FragColour = vec4(N, 1);
}
