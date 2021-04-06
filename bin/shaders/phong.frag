// classic Phong fragment shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

out vec4 FragColour;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform int lightType; // the type of light used

uniform vec3 Ia; // ambient light colour
uniform vec3 Id; // diffuse light colour
uniform vec3 Is; // specular light colour
uniform float ambientStrength; // strength of ambient light
uniform float specularStrength; // strength of specular light

uniform vec3 Ka; // ambient material colour
uniform vec3 Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float specularPower; // material specular power

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
	vec3 L = normalize( vec3( vPosition ) - lightPosition );

	// calculate lambert term
	float lambertTerm = max( 0, dot( N, -L ));

	// calculate view vector and reflection vector
	vec3 V = normalize( cameraPosition - vPosition.xyz );
	vec3 R = reflect( L, N );

	// calculate specular term
	float specularTerm = pow( max( 0, dot( R, V )), specularPower );

	// calculate each colour property
	vec3 ambient = Ia * Ka * ambientStrength;
	vec3 diffuse = Id * Kd * lambertTerm;
	vec3 specular = Is * Ks * specularTerm * specularStrength;

	// output final colour
	return vec4( ambient + diffuse + specular, 1 );
}

vec4 Directional()
{
	vec3 N = normalize( vNormal );
	vec3 L = normalize( lightDirection );

	// calculate lambert term
	float lambertTerm = max( 0, dot( N, -L ));

	// calculate view vector and reflection vector
	vec3 V = normalize( cameraPosition - vPosition.xyz );
	vec3 R = reflect( L, N );

	// calculate specular term
	float specularTerm = pow( max( 0, dot( R, V )), specularPower );

	// calculate each colour property
	vec3 ambient = Ia * Ka * ambientStrength;
	vec3 diffuse = Id * Kd * lambertTerm;
	vec3 specular = Is * Ks * specularTerm * specularStrength;

	// output final colour
	return vec4( ambient + diffuse + specular, 1 );
}

vec4 Point()
{
	vec3 N = normalize( vNormal );
	vec3 L = normalize( vec3( vPosition ) - lightPosition );

	// calculate lambert term
	float lambertTerm = max( 0, dot( N, -L ));

	// calculate view vector and reflection vector
	vec3 V = normalize( cameraPosition - vPosition.xyz );
	vec3 R = reflect( L, N );

	// calculate specular term
	float specularTerm = pow( max( 0, dot( R, V )), specularPower );

	// calculate each colour property
	vec3 ambient = Ia * Ka * ambientStrength;
	vec3 diffuse = Id * Kd * lambertTerm;
	vec3 specular = Is * Ks * specularTerm * specularStrength;

	float distance = length( lightPosition - vec3( vPosition ) );
	float attenuation = 1.0 / ( constant + linear * distance + quadratic * distance * distance );

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

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