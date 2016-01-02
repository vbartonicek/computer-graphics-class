#version 140

struct Material {           // structure that describes currently used material
  vec3  ambient;            // ambient component
  vec3  diffuse;            // diffuse component
  vec3  specular;           // specular component
  float shininess;          // sharpness of specular reflection

  bool  useTexture;         // defines whether the texture is used or not
};

struct Fog {      
	int fogOnOff; 
	int fogType; 
	bool fogTime;  
};

struct Light {         // structure describing light parameters
  vec3  ambient;       // intensity & color of the ambient component
  vec3  diffuse;       // intensity & color of the diffuse component
  vec3  specular;      // intensity & color of the specular component
  vec3  position;      // light position
  vec3  pointDirection; // pointlight direction
  float pointCosCutOff; // cosine of the pointlight's half angle
  float pointExponent;  // distribution of the light energy within the reflector's cone (center->cone's edge)
};

uniform sampler2D texSampler;  // sampler for the texture access

uniform float time;         // time used for simulation of moving lights (such as sun)
uniform Material material;     // current material
uniform mat4 Vmatrix;       // View                       --> world to eye coordinates

uniform vec3 reflectorPosition;   // reflector position (world coordinates)
uniform vec3 reflectorDirection;  // reflector direction (world coordinates)

uniform Fog fog; //fog data

uniform bool sunOnOff;
uniform bool pointOnOff;

smooth in vec3 vertexPosition;	// incoming vertex position
smooth in vec3 vertexNormal;	// incoming vertex normal
smooth in vec2 vertexTexCoord;	// incoming texture coordinates
in vec4 viewSpace;

out vec4 color_f;			   // outgoing fragment color

//vec4 finalColor = vec4(0,0,0,1);


//Fog calculation 
vec4 fogEffect(vec4 color) {

	vec4 fogColor = vec4(0.5, 0.5,0.5,1);
	
	float fogDensity = 0.3;	//fog density used in exponentional fog
	float fogStart = 1;		//fog minimum distance
	float fogEnd = 4;		//fog maximum distance
	float fogDistance = 0;	//fog distance
	float fogFactor = 0;	//fog factor
	float fogSpeed= 0.25;	//fog speed
	vec4 outputColor;

	fogDistance = abs(viewSpace.z); //brat vzdalenost od kamery

	if (fog.fogOnOff==1){

	if (fog.fogType==1) fogFactor = (fogEnd - fogDistance)/(fogEnd - fogStart);//linear
	if (fog.fogType==2) fogFactor = 1.0f / (exp(fogDensity * fogDistance));//exponential

	if (fog.fogTime==true) fogFactor = clamp( fogFactor, 0.0, 1.0 )*abs(sin(time*fogSpeed));
	else  fogFactor = clamp( fogFactor, 0.0, 1.0 );

	  outputColor = mix(fogColor, color, fogFactor);
	  return outputColor;
	} 

	else return color; // return input color without any change
}

Light sun;
Light camera;



// Point light setup
void setCameraLight(){
  camera.ambient       = vec3(0.4f);
  camera.diffuse       = vec3(1.0f, 0.0f, 0.0f);
  camera.specular      = vec3(1.0f);
  camera.pointCosCutOff = 0.95f;
  camera.pointExponent  = 0.0f;

  camera.position = (Vmatrix * vec4(reflectorPosition, 1.0f)).xyz;
  camera.pointDirection = normalize((Vmatrix * vec4(reflectorDirection, 0.0f)).xyz);
}

// Sun light setup
void setSunLight() {
  float sunSpeed = 0.25f;

  sun.ambient  = vec3(0.5f);
  sun.diffuse  = vec3(1.0f, 1.0f, 0.5f);
  sun.specular = vec3(1.0f);

  sun.position = (Vmatrix * vec4(cos(time * sunSpeed), 0.0f, sin(time * sunSpeed), 0.0f)).xyz;
}


vec4 pointLight(Light light, Material material, vec3 vertexPosition, vec3 vertexNormal) {

    vec3 ret = vec3(0.0f);
  vec3 newLight_position = (Vmatrix * vec4(light.position, 1.0f)).xyz;

  vec3 L = normalize(newLight_position - vertexPosition);
  vec3 R = reflect(-L, vertexNormal);
  vec3 V = normalize(-vertexPosition);

  float NdotL = max(0.0f, dot(vertexNormal, L));
  float RdotV = max(0.0f, dot(R, V));

  ret += material.ambient * light.ambient + material.diffuse * light.diffuse * NdotL + material.specular * light.specular * pow(RdotV, material.shininess);

  return vec4(ret, 1.0f);
}

vec4 directionalLight(Light light, Material material, vec3 vertexPosition, vec3 vertexNormal) {

  vec3 outputColor = vec3(0.0f);

  vec3 L = normalize(light.position);
  vec3 R = reflect(-L, vertexNormal);
  vec3 V = normalize(-vertexPosition);
  float NdotL = max(0.0f, dot(vertexNormal, L));
  float RdotV = max(0.0f, dot(R, V));

  outputColor += material.ambient * light.ambient;
  outputColor += material.diffuse * light.diffuse * NdotL;
  outputColor += material.specular * light.specular * pow(RdotV, material.shininess);

  return vec4(outputColor, 1.0f);
}

void main() {


	setCameraLight();
	setSunLight();

  vec3 globalAmbientLight = vec3(0.5f);
  vec4 color = vec4(material.ambient * globalAmbientLight, 0.0f);

  if(sunOnOff==true) color += directionalLight(sun, material, vertexPosition, vertexNormal);

  if(pointOnOff==true) color +=pointLight(camera, material, vertexPosition, vertexNormal);


  // if material has a texture -> apply it
  if(material.useTexture) color_f =  color * texture(texSampler, vertexTexCoord);

  //color_f=finalColor;
  color_f = fogEffect(color_f);


}
