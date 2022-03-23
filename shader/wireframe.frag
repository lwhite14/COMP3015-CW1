#version 460

struct LightInfo {
	vec4 Position;
	vec3 Intensity; // A, D, S Intensity
};
uniform LightInfo Light;

struct MaterialInfo  {
	vec3 Ka;			// Ambient reflectivity
	vec3 Kd;			// Diffuse reflectivity
	vec3 Ks;			// Specular reflectivity
	float Shininess;	// Specular shininess factor
}; 
uniform MaterialInfo Material;

uniform struct LineInfo {
	float Width;
	vec4 Color;
} Line;

in vec3 GPosition;
in vec3 GNormal;
noperspective in vec3 GEdgeDistance;

layout (location = 0) out vec4 FragColor;

vec3 phongModel( vec3 position, vec3 normal ) 
{
	vec3 ambient = Material.Ka * Light.Intensity.x;
	vec3 s = normalize(vec3(Light.Position - vec4(position, 1.0f)));
	float sDotN = max( dot(s,normal), 0.0 );
	vec3 diffuse = Material.Kd * Light.Intensity.y * sDotN;
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
	vec4 color = vec4(phongModel (GPosition, GNormal), 1.0 );
//	vec4 color = vec4(1.0, 0.0, 0.0, 1.0 );

	float d = min( GEdgeDistance.x, GEdgeDistance.y );
	d = min( d, GEdgeDistance.z );

	float mixVal;
	if (d < Line.Width - 1)
	{
		mixVal = 1.0;
	}
	else if (d > Line.Width + 1)
	{
		mixVal = 0.0;
	}
	else
	{
		float x = d - (Line.Width - 1);
		mixVal = exp2(-2.0 * (x*x));
	}

	FragColor = mix( color, Line.Color, mixVal );
}