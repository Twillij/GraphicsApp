// post-processing fragment shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;

uniform int postEffect;

out vec4 FragColour;

// just output the colour unchanged
vec4 Default( vec2 texCoord )
{
	return texture( colourTarget, texCoord );
}

// simple box blur
vec4 BoxBlur( vec2 texCoord )
{
	vec2 texel = 1.0f / textureSize( colourTarget, 0 );

	// 9-tap box kernel
	vec4 colour = texture( colourTarget, texCoord );
	colour += texture( colourTarget, texCoord + texel * vec2( -1, 1 ));
	colour += texture( colourTarget, texCoord + texel * vec2( -1, 0 ));
	colour += texture( colourTarget, texCoord + texel * vec2( -1, -1 ));
	colour += texture( colourTarget, texCoord + texel * vec2( 0, 1 ));
	colour += texture( colourTarget, texCoord + texel * vec2( 0, -1 ));
	colour += texture( colourTarget, texCoord + texel * vec2( 1, 1 ));
	colour += texture( colourTarget, texCoord + texel * vec2( 1, 0 ));
	colour += texture( colourTarget, texCoord + texel * vec2( 1, -1 ));
	
	return colour / 9;
}

// simple distortion effect
vec4 Distort( vec2 texCoord )
{
	vec2 mid = vec2( 0.5f );

	float distanceFromCentre = distance( texCoord, mid );
	vec2 normalizedCoord = normalize( texCoord - mid );
	float bias = distanceFromCentre + sin( distanceFromCentre * 15 ) * 0.05f;
	vec2 newCoord = mid + bias * normalizedCoord;

	return texture( colourTarget, newCoord );
}

// sobel edge detection
vec4 Sobel( vec2 texCoord )
{
	mat3 sx = mat3
	(
		vec3( 1, 2, 1 ),
		vec3( 0, 0, 0 ),
		vec3( -1, -2, -1 )
	);

	mat3 sy = mat3
	(
		vec3( 1, 0, -1 ),
		vec3( 2, 0, -2 ),
		vec3( 1, 0, -1 )
	);

	vec3 diffuse = texture( colourTarget, texCoord.st ).rgb;
    mat3 I;

    for ( int i = 0; i < 3; ++i )
	{
        for ( int j = 0; j < 3; ++j )
		{
            vec3 texel  = texelFetch( colourTarget, ivec2( gl_FragCoord ) + ivec2( i - 1, j - 1 ), 0 ).rgb;
            I[i][j] = length( texel ); 
		}
	}

	float gx = dot( sx[0], I[0] ) + dot( sx[1], I[1] ) + dot( sx[2], I[2] ); 
	float gy = dot( sy[0], I[0] ) + dot( sy[1], I[1] ) + dot( sy[2], I[2] );

	float g = sqrt( pow( gx, 2 ) + pow( gy, 2 ));

	return vec4( diffuse - vec3( g ), 1.0 );
}

void main()
{
	// calculate texel size
	vec2 texSize = textureSize( colourTarget, 0 );
	vec2 texelSize = 1.0f / texSize;

	// adjust texture coordinate
	vec2 scale = ( texSize - texelSize ) / texSize;
	vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

	// sample post effect
	if ( postEffect == 1 )
	{
		FragColour = BoxBlur( texCoord );
	}
	else if ( postEffect == 2 )
	{
		FragColour = Distort( texCoord );
	}
	else if ( postEffect == 3 )
	{
		FragColour = Sobel( texCoord );
	}
	else
	{
		FragColour = Default( texCoord );
	}
}