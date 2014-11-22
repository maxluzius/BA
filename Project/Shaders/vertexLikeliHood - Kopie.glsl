#version 330

layout (location = 0) in vec3 in_Position;
//Eingabe x ist die Tiefe, y ist die Höhe und z ist Breite.

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform sampler2D tex;

varying vec4 position;
	
//uniform vec3 cameraPos;

void main() {	
	
	position = P * V * M * vec4(in_Position,1);

	vec3 pos = position.xyz;
	
	pos.x = pos.x / position.w;
	pos.y = pos.y / position.w;
	pos.x = (pos.x + 1.0) / 2.0;
	pos.y = (pos.y + 1.0) / 2.0;

   vec3 color = texture2D(tex, pos.xy).xyz;


   //schwellwert anpassen, wegen sobel, ist kein binärbild
   if(color.x == 0.0)
   	//hier wird bei nicht gesetzter Farbe das Pixel aus dem FOV gerendert
   	gl_Position = vec4(position.xy, -100.0, position.w);
   else
	gl_Position = P * V * M * vec4(in_Position,1);
}
