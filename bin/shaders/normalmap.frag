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

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColour[MAX_LIGHTS];
uniform vec3 PointLightPosition[MAX_LIGHTS];

vec3 diffuse(vec3 direction, vec3 colour, vec3 normal)
{
	return colour * max( 0, dot( normal, -direction ) );
}

vec3 specular(vec3 direction, vec3 colour, vec3 normal, vec3 view)
{
	vec3 R = reflect( direction, normal );
	float specularTerm = pow( max( 0, dot( R, view ) ), specularPower );
	return specularTerm * colour;
}

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


	// calculate diffuse lighting from sunlight
	vec3 diffuseTotal = diffuse(L, LightColour, N);

	// calculate lambert term
	float lambertTerm = max( 0, dot( N, -L ) );


	// calculate view vector and reflection vector
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );

	// calculate specular light from directional light
	vec3 specularTotal = specular(L, LightColour, N, V);


	// calculate specular term
	float specularTerm = pow( max( 0, dot( R, V ) ), specularPower );



	for (int i=0; i<numLights; i++)
	{
		vec3 direction = vPosition.xyz - PointLightPosition[i];
		float distance = length(direction);
		direction = direction/distance;
		// attenuate the light intensity with inverse square law
		vec3 colour = PointLightColour[i]/(distance * distance);
		diffuseTotal += diffuse(direction, colour, N);
		specularTotal += specular(direction, colour, N, V);
	}

	// calculate each light property
	vec3 ambient = AmbientColour * Ka * texDiffuse;
	vec3 diffuse0 = Kd * texDiffuse * diffuseTotal;
	vec3 specular0 = Ks * texSpecular * specularTotal;


	FragColour = vec4(ambient + diffuse0 + specular0, 1);
	
	//FragColour = vec4(N, 1);
}