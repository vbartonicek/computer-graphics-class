#version 140

struct Fog {     
	int fogOnOff;
	int fogType; 
};

uniform samplerCube skyboxSampler;
uniform float time;           // used for simulation of moving lights (such as sun) and to select proper animation frame
in vec3 texCoord_v;
out vec4 color_f;

uniform Fog fog; //fog data

//Fog calculation 
vec4 fogEffect(vec4 color) {

	vec4 fogColor = vec4(0.5, 0.5,0.5,1);
	
	float fogDensity = 0.3;	//fog density used in exponentional fog
	float fogStart = 1;		//fog minimum distance
	float fogEnd = 4;		//fog maximum distance
	float fogDistance = 20;	//fog distance - set for skybox
	float fogFactor = 0;	//fog factor
	vec4 outputColor;

	if (fog.fogOnOff==1){

	if (fog.fogType==1) fogFactor = (fogEnd - fogDistance)/(fogEnd - fogStart);//linear
	if (fog.fogType==2)fogFactor = 1.0f / (exp(fogDensity * fogDistance));//exponential

	fogFactor = clamp( fogFactor, 0.0, 1.0 );

	  outputColor = mix(fogColor, color, fogFactor);
	  return outputColor;
	} 

	else return color; // return input color without any change
}

void main() {

  color_f = texture(skyboxSampler, texCoord_v);
  color_f = fogEffect(color_f);
}