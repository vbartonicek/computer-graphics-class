#version 140

struct Fog {     
	int fogOnOff;
	int fogType; 
	bool fogTime; 
};

uniform sampler2D texSampler; // sampler for texture access
uniform Fog fog; //fog data
uniform float time;

smooth in vec2 texCoord_v;    // fragment texture coordinates

out vec4 color_f;             // outgoing fragment color

//Fog calculation 
vec4 fogEffect(vec4 color) {

	vec4 fogColor = vec4(0.5, 0.5,0.5,1);
	
	float fogDensity = 0.3;	//fog density used in exponentional fog
	float fogStart = 1;		//fog minimum distance
	float fogEnd = 4;		//fog maximum distance
	float fogDistance = 20;	//fog distance - set for skybox
	float fogFactor = 0;	//fog factor
	float fogSpeed= 0.25;	//fog speed
	vec4 outputColor;

	if (fog.fogOnOff==1){

	if (fog.fogType==1) fogFactor = (fogEnd - fogDistance)/(fogEnd - fogStart);//linear
	if (fog.fogType==2)fogFactor = 1.0f / (exp(fogDensity * fogDistance));//exponential

	if (fog.fogTime==true) fogFactor = clamp( fogFactor, 0.0, 1.0 )*abs(sin(time*fogSpeed));
	else  fogFactor = clamp( fogFactor, 0.0, 1.0 );

	  outputColor = mix(fogColor, color, fogFactor);
	  return outputColor;
	} 

	else return color; // return input color without any change
}


void main() {
  color_f=texture(texSampler, texCoord_v);//*vec4(1.0,0.0,0.0,0.0);
  color_f = fogEffect(color_f);
}