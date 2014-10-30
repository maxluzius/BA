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

    float max = ibl.y;
    vec3 direction = ibl;
    if(itr.y > max){max = itr.y; direction = itr;}
    if(itl.y > max){max = itl.y; direction = itl;}
    if(ibr.y > max){max = ibr.y; direction = ibr;}
    if(il.y > max){max = il.y; direction = il;}
    if(ir.y > max){max = ir.y; direction = ir;}
    if(ib.y > max){max = ib.y; direction = ib;}
    if(it.y > max){max = it.y; direction = it;}
        

    if(i.x < direction.x)
        fragmentColor = vec3(0.0);
    else{
    fragmentColor = vec3(i.x, i.y, 0.0);
    //color for the texture we are writing to
    color = vec3(i.x, i.y, 0.0);
    }
}

