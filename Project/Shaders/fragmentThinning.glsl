#version 330

in vec2 passUV;

 varying vec2 textureCoordinate;
 varying vec2 leftTextureCoordinate;
 varying vec2 rightTextureCoordinate;

 varying vec2 topTextureCoordinate;
 varying vec2 topLeftTextureCoordinate;
 varying vec2 topRightTextureCoordinate;

 varying vec2 bottomTextureCoordinate;
 varying vec2 bottomLeftTextureCoordinate;
 varying vec2 bottomRightTextureCoordinate;

uniform sampler2D tex;

out vec3 fragmentColor;
layout(location = 0) out vec3 color;

void main() {

    vec3 i   = texture(tex, textureCoordinate).xyz;
    vec3 ibl = texture(tex, bottomLeftTextureCoordinate).xyz;
    vec3 itr = texture(tex, topRightTextureCoordinate).xyz;
    vec3 itl = texture(tex, topLeftTextureCoordinate).xyz;
    vec3 ibr = texture(tex, bottomRightTextureCoordinate).xyz;
    vec3 il = texture(tex, leftTextureCoordinate).xyz;
    vec3 ir = texture(tex, rightTextureCoordinate).xyz;
    vec3 ib = texture(tex, bottomTextureCoordinate).xyz;
    vec3 it = texture(tex, topTextureCoordinate).xyz;

    if(i.y < 0.125 || (i.y <=1.0 && i.y >= 0.875)){
        if(i.x >= ib.x && i.x >= it.x){
            fragmentColor = vec3(i.x, i.y, 0.0);
            //color for the texture we are writing to
            color = vec3(i.x, i.y, 0.0);
        }
    }
    if(i.y < 0.375 && i.y >= 0.125){
        if(i.x >= ibr.x && i.x >= itl.x){
            fragmentColor = vec3(i.x, i.y, 0.0);
            //color for the texture we are writing to
            color = vec3(i.x, i.y, 0.0);
        }
    }
    if(i.y < 0.625 && i.y >= 0.375){
        if(i.x >= il.x && i.x >= ir.x){
            fragmentColor = vec3(i.x, i.y, 0.0);
            //color for the texture we are writing to
            color = vec3(i.x, i.y, 0.0);
        }
    }
    if(i.y < 0.875 && i.y >= 0.625){
        if(i.x >= ib.x && i.x >= it.x){
            fragmentColor = vec3(i.x, i.y, 0.0);
            //color for the texture we are writing to
            color = vec3(i.x, i.y, 0.0);
        }
    }
    else{
        fragmentColor = vec3(0.0);
    }
}

