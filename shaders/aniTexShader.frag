#version 140

struct Fog {     
	int fogOnOff;
	int fogType; 
	bool fogTime; 
};

uniform float time;           // used for simulation of moving lights (such as sun) and to select proper animation frame
uniform mat4 Vmatrix;         // view (camera) transform --> world to eye coordinates
uniform sampler2D texSampler; // sampler for texture access
uniform Fog fog; //fog data

smooth in vec3 position_v;    // camera space fragment position
smooth in vec2 texCoord_v;    // fragment texture coordinates

out vec4 color_f;             // outgoing fragmsmokeent color

// there are 9 frames in the row, one row total
uniform ivec2 pattern = ivec2(10, 1);
// one frame lasts 0.1s
uniform float frameDuration = 0.1f;

// for textures with 1 row and N columns
vec4 sampleTexture(int frame) {

  vec2 offset = vec2(1.0f) / vec2(pattern);

  vec2 texCoordBase = texCoord_v / vec2(pattern);
  vec2 texCoord = texCoordBase + vec2(frame % pattern.x, /*(pattern.y - 1 )*/- (frame / pattern.x)) * offset;
 // vec2 texCoord = texCoordBase + vec2(frame % pattern.x, (pattern.y - 1 )- (frame / pattern.x)) * offset;

  return texture(texSampler, texCoord);
}

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
  // frame of the texture to be used for explosion drawing 
  int frame = int(time / frameDuration);
  
  // sample proper frame of the texture to get a fragment color  
  color_f = sampleTexture(frame);
  color_f = fogEffect(color_f);
}