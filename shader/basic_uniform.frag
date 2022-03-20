#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout( location = 0 ) out vec4 FragColor;

 //light information struct
uniform struct LightInfo 
{
	vec4 Position;	// Light position in eye coords.
	vec3 La;		// Ambient light intensity
	vec3 Ld;			// Specular light intensity
	vec3 Ls;			// Diffuse light intensity
} Light;

//material information struct
uniform struct MaterialInfo 
{
	vec3 Ka;			// Ambient reflectivity
	vec3 Kd;			// Diffuse reflectivity
	vec3 Ks;			// Specular reflectivity
	float Shininess;	// Specular shininess factor
} Material;

layout(binding=0) uniform sampler2D BrickTex;
layout(binding=1) uniform sampler2D MossTex;

vec3 blinnPhong( vec3 position, vec3 normal ) 
{
	vec3 texColorBrick = texture(BrickTex, TexCoord).rgb;
	vec4 texColorMoss= texture(MossTex, TexCoord).rgba;
	vec3 col = mix(texColorBrick.rgb, texColorMoss.rgb, texColorMoss.a);

	//calculate ambient here, to access each light La value use this:
	vec3 ambient = Material.Ka * Light.La * col;

	//calculate diffuse here
	vec3 s = normalize(vec3(Light.Position - vec4(position, 1.0f)));
	float sDotN = max( dot(s,normal), 0.0 );
	vec3 diffuse = Material.Kd * Light.Ld * sDotN * col;

	//calculate specular here
	vec3 spec = vec3(0.0);
	if( sDotN > 0.0 )
	{
		vec3 v = normalize(-position.xyz);
		vec3 h = normalize( v + s ); 
		spec = Material.Ks * pow( max( dot(h,normal), 0.0 ), Material.Shininess );
	}
	return ambient + diffuse + spec;
}

void main()
{
	FragColor = vec4(blinnPhong(Position, normalize(Normal)), 1.0f);
}