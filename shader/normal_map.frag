#version 450

in vec3 LightDir;
in vec2 TexCoord;
in vec3 ViewDir;

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

layout(binding=0) uniform sampler2D ColorTex;
layout(binding=1) uniform sampler2D NormalMapTex;

vec3 blinnPhong( vec3 position, vec3 normal ) 
{
	vec3 texColor = texture(ColorTex, TexCoord).rgb;
	vec3 ambient = Material.Ka * Light.La * texColor;
	vec3 s = normalize(vec3(Light.Position - vec4(position, 1.0f)));
	float sDotN = max( dot(s,normal), 0.0 );
	vec3 diffuse = Material.Kd * Light.Ld * sDotN * texColor;
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
	// Unpack the normal and set it to a range between 0 and 1
	vec3 norm = texture(NormalMapTex, TexCoord).xyz;
	norm.xy = 2.0 * norm.xy - 1.0;

	FragColor = vec4 ( blinnPhong(ViewDir, norm), 1.0f );

}