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

    float grayi = (i.r + i.g + i.b) / 3.0;
    float grayibl = (ibl.r + ibl.g + ibl.b) / 3.0;
    float grayitr = (itr.r + itr.g + itr.b) / 3.0;
    float grayitl = (itl.r + itl.g + itl.b) / 3.0;
    float grayibr = (ibr.r + ibr.g + ibr.b) / 3.0;
    float grayil = (il.r + il.g + il.b) / 3.0;
    float grayir = (ir.r + ir.g + ir.b) / 3.0;
    float grayib = (ib.r + ib.g + ib.b) / 3.0;
    float grayit = (it.r + it.g + it.b) / 3.0;

    //in y Richtung
    float h = grayitl + 2.0 * grayit + grayitr - grayibl - 2.0 * grayib - grayibr;
    //in x Richtung
    float v = grayibl + 2.0 * grayil + grayitl - grayibr - 2.0 * grayir - grayitr;

    //float grayimg = sqrt(h * h + v * v);
    //float grayimg = atan(h,v);
    float graytop = grayitl * 0.045 + grayit * 0.08 + grayitr * 0.045;                                  float graymid = grayil  * 0.08 + grayi  * 0.5 + grayir * 0.08; 
    float graybot = grayibl * 0.045 + grayib * 0.08 + grayibr * 0.045;
    float grayimg = graytop + graymid + graybot;

	fragmentColor = vec3(grayimg);

}

