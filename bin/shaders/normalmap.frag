// a normal map fragment shader
#version 410
#define DIR_LIGHTS_COUNT 1
#define POINT_LIGHTS_COUNT 4

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec4 vPosition;
out vec4 FragColour;

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float ambientStrength;
	float diffuseStrength;
	float specularStrength;

	int specularPower;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	
	int specularPower;
};

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	
	int specularPower;
};

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform DirLight dirLights[DIR_LIGHTS_COUNT];
uniform PointLight pointLights[POINT_LIGHTS_COUNT];

uniform vec3 Ka; // material ambient
uniform vec3 Kd; // material diffuse
uniform vec3 Ks; // material specular
uniform float specularPower; // material specular power

uniform vec3 Ia; // light ambient
uniform vec3 Id; // light diffuse
uniform vec3 Is; // light specular
uniform float ambientStrength; // strength of ambient light
uniform float specularStrength; // strength of specular light

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

// only used when using a directional light
uniform vec3 lightDirection;

vec3 Standard()
{
	vec3 N = normalize( vNormal );
	vec3 T = normalize( vTangent );
	vec3 B = normalize( vBiTangent );
	vec3 L = normalize( vec3( vPosition ) - lightPosition );

	vec3 texDiffuse = texture( diffuseTexture, vTexCoord ).rgb;
	vec3 texSpecular = texture( specularTexture, vTexCoord ).rgb;
	vec3 texNormal = texture( normalTexture, vTexCoord ).rgb;

	mat3 TBN = mat3( T, B, N );
	N = TBN * ( texNormal * 2 - 1 );

	// calculate lambert term
	float lambertTerm = max( 0, dot( N, -L ));
	
	// calculate view vector and reflection vector
	vec3 V = normalize( cameraPosition - vec3( vPosition ));
	vec3 R = reflect( L, N );

	// calculate the specular term
	float specularTerm = pow( max( 0, dot( R, V )), specularPower );

	// calculate each light property
	vec3 ambient = Ia * Ks * ambientStrength;
	vec3 diffuse = Id * Kd * texDiffuse * lambertTerm;
	vec3 specular = Is * Ks * texSpecular * specularTerm * specularStrength;

	// output final colour
	return ambient + diffuse + specular;
}

vec3 CalculateDirLight( DirLight light )
{
	vec3 N = normalize( vNormal );
	vec3 T = normalize( vTangent );
	vec3 B = normalize( vBiTangent );
	vec3 L = normalize( light.direction );

	vec3 texDiffuse = texture( diffuseTexture, vTexCoord ).rgb;
	vec3 texSpecular = texture( specularTexture, vTexCoord ).rgb;
	vec3 texNormal = texture( normalTexture, vTexCoord ).rgb;

	mat3 TBN = mat3( T, B, N );
	N = TBN * ( texNormal * 2 - 1 );

	// calculate lambert term
	float lambertTerm = max( 0, dot( N, -L ));
	
	// calculate view vector and reflection vector
	vec3 V = normalize( cameraPosition - vec3( vPosition ));
	vec3 R = reflect( L, N );

	// calculate the specular term
	float specularTerm = pow( max( 0, dot( R, V )), light.specularPower );

	// calculate each light property
	vec3 ambient = light.ambient * Ks * light.ambientStrength;
	vec3 diffuse = light.diffuse * Kd * texDiffuse * lambertTerm;
	vec3 specular = light.specular * Ks * texSpecular * specularTerm * light.specularStrength;

	// output final colour
	return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight light)
{
	vec3 N = normalize( vNormal );
	vec3 T = normalize( vTangent );
	vec3 B = normalize( vBiTangent );
	vec3 L = normalize( vec3( vPosition ) - light.position );

	vec3 texDiffuse = texture( diffuseTexture, vTexCoord ).rgb;
	vec3 texSpecular = texture( specularTexture, vTexCoord ).rgb;
	vec3 texNormal = texture( normalTexture, vTexCoord ).rgb;

	mat3 TBN = mat3( T, B, N );
	N = TBN * ( texNormal * 2 - 1 );

	// calculate lambert term
	float lambertTerm = max( 0, dot( N, -L ));

	// calculate view vector and reflection vector
	vec3 V = normalize( cameraPosition - vPosition.xyz );
	vec3 R = reflect( L, N );

	// calculate specular term
	float specularTerm = pow( max( 0, dot( R, V )), light.specularPower );

	// calculate each colour property
	vec3 ambient = light.ambient * Ka * light.ambientStrength;
	vec3 diffuse = light.diffuse * Kd * texDiffuse * lambertTerm * 5;
	vec3 specular = light.specular * Ks * texSpecular * specularTerm * light.specularStrength;

	float distance = length( light.position - vec3( vPosition ) );
	float attenuation = 1 / ( light.constant + light.linear * distance + light.quadratic * distance * distance );

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// output final colour
	return ambient + diffuse + specular;
}

void main()
{
	//vec3 norm = normalize(Normal);
	vec3 result;

	for (int i = 0; i < DIR_LIGHTS_COUNT; ++i)
	{
		result += CalculateDirLight( dirLights[i] );
	}

	for (int i = 0; i < POINT_LIGHTS_COUNT; ++i)
	{
		result += CalculatePointLight( pointLights[i] );
	}

	FragColour = vec4(result, 1);
}