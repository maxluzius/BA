#version 330

out vec3 fragmentColor;

varying vec4 position;

uniform sampler2D tex;

void main(){
	
	vec3 pos = position.xyz;
	pos.x = pos.x / position.w;
	pos.y = pos.y / position.w;
	pos.x = (pos.x + 1.0) / 2.0;
	pos.y = (pos.y + 1.0) / 2.0;
	//pos.z = 0.0;
   vec3 color = texture2D(tex, pos.xy).xyz;
    fragmentColor = vec3(color.xy, 0.0);
}
