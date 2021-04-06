// a normal map fragment shader
#version 410

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec4 vPosition;

out vec4 FragColour;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform int lightType; // the type of light used

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

// only used when using a point light
uniform float constant;
uniform float linear;
uniform float quadratic;

vec4 Standard()
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
	return vec4( ambient + diffuse + specular, 1 );
}

vec4 Directional()
{
	vec3 N = normalize( vNormal );
	vec3 T = normalize( vTangent );
	vec3 B = normalize( vBiTangent );
	vec3 L = normalize( lightDirection );

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
	return vec4( ambient + diffuse + specular, 1 );
}

vec4 Point()
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
	vec3 V = normalize( cameraPosition - vPosition.xyz );
	vec3 R = reflect( L, N );

	// calculate specular term
	float specularTerm = pow( max( 0, dot( R, V )), specularPower );

	// calculate each colour property
	vec3 ambient = Ia * Ka * ambientStrength;
	vec3 diffuse = Id * Kd * texDiffuse * lambertTerm;
	vec3 specular = Is * Ks * texSpecular *specularTerm * specularStrength;

	float distance = length( lightPosition - vec3( vPosition ) );
	float attenuation = 1 / ( constant + linear * distance + quadratic * distance * distance );

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// output final colour
	return vec4( ambient + diffuse + specular, 1 );
}

void main()
{
	if ( lightType == 1 )
	{
		FragColour = Directional();
	}
	else if ( lightType == 2 )
	{
		FragColour = Point();
	}
	else
	{
		FragColour = Standard();
	}
}