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

 varying vec2 topLeft;
 varying vec2 topSemiLeft;
 varying vec2 top;
 varying vec2 topSemiRight;
 varying vec2 topRight;

 varying vec2 rightSemiTop;
 varying vec2 right;
 varying vec2 rightSemiBot;

 varying vec2 botRight;
 varying vec2 botSemiRight;
 varying vec2 bot;
 varying vec2 botSemiLeft;
 varying vec2 botLeft;

 varying vec2 leftSemiBot;
 varying vec2 left;
 varying vec2 leftSemiTop;

uniform sampler2D tex;

out vec3 fragmentColor;
layout(location = 0) out vec3 color;

void main() {

    //outer circel
    vec3 tl = texture(tex, topLeft).xyz;
    vec3 tsl = texture(tex, topSemiLeft).xyz;
    vec3 t = texture(tex, top).xyz;
    vec3 tsr = texture(tex, topSemiRight).xyz;
    vec3 tr = texture(tex, topRight).xyz;

    vec3 rst = texture(tex, rightSemiTop).xyz;
    vec3 r = texture(tex, right).xyz;
    vec3 rsb = texture(tex, rightSemiBot).xyz;

    vec3 br = texture(tex, botRight).xyz;
    vec3 bsr = texture(tex, botSemiRight).xyz;
    vec3 b = texture(tex, bot).xyz;
    vec3 bsl = texture(tex, botSemiLeft).xyz;
    vec3 bl = texture(tex, botLeft).xyz;

    vec3 lsb = texture(tex, leftSemiBot).xyz;
    vec3 l = texture(tex, left).xyz;
    vec3 lst = texture(tex, leftSemiTop).xyz;

    //inner circle
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

    float graytl = (tl.r + tl.g + tl.b) / 3.0;
    float graytsl = (tsl.r + tsl.g + tsl.b) / 3.0;
    float grayt = (t.r + t.g + t.b) / 3.0;
    float graytsr = (tsr.r + tsr.g + tsr.b) / 3.0;
    float graytr = (tr.r + tr.g + tr.b) / 3.0;

    float grayrst = (rst.r + rst.g + rst.b) / 3.0;
    float grayr = (r.r + r.g + r.b) / 3.0;
    float grayrsb = (rsb.r + rsb.g + rsb.b) / 3.0;

    float graybr = (br.r + br.g + br.b) / 3.0;
    float graybsr = (bsr.r + bsr.g + bsr.b) / 3.0;
    float grayb = (b.r + b.g + b.b) / 3.0;
    float graybsl = (bsl.r + bsl.g + bsl.b) / 3.0;
    float graybl = (bl.r + bl.g + bl.b) / 3.0;

    float graylst = (lst.r + lst.g + lst.b) / 3.0;
    float grayl = (l.r + l.g + l.b) / 3.0;
    float graylsb = (lsb.r + lsb.g + lsb.b) / 3.0;


    //float inner = grayitl + grayit + grayitr + grayil + grayi + grayir + grayibl + grayib + grayibr;
    //float outer = graytl + graytsl + grayt + graytsr + graytr + grayrst + grayr + grayrsb + graybr + graybsr + grayb + graybsl + graybl + graylst + grayl + graylsb;

    vec3 inner = itl * 16.0 + it * 26.0 + itr * 16.0 + il * 26.0 + i * 41.0 + ir * 26.0 + ibl * 16.0 + ib * 26.0 + ibr * 16.0;
    vec3 outer = tl + tsl * 4.0 + t * 7.0 + tsr * 4.0 + tr + rst * 4.0 + r * 7.0 + rsb * 4.0 + br + bsr * 4.0 + b * 7.0 + bsl * 4.0 + bl + lst * 4.0 + l * 7.0 + lsb * 4.0;

    float innerR = itl.r + it.r + itr.r + il.r + i.r + ir.r + ibl.r + ib.r + ibr.r;
    float innerG = itl.g + it.g + itr.g + il.g + i.g + ir.g + ibl.g + ib.g + ibr.g;
    float innerB = itl.b + it.b + itr.b + il.b + i.b + ir.b + ibl.b + ib.b + ibr.b;
    float outerR = tl.r + tsl.r + t.r + tsr.r + tr.r + rst.r + r.r + rsb.r + br.r + bsr.r + b.r + bsl.r + bl.r + lst.r + l.r + lsb.r;
    float outerG = tl.g + tsl.g + t.g + tsr.g + tr.g + rst.g + r.g + rsb.g + br.g + bsr.g + b.g + bsl.g + bl.g + lst.g + l.g + lsb.g;
    float outerB = tl.b + tsl.b + t.b + tsr.b + tr.b + rst.b + r.b + rsb.b + br.b + bsr.b + b.b + bsl.b + bl.b + lst.b + l.b + lsb.b;

    vec3 median = (inner + outer) / 273.0; // 209
    fragmentColor = median;
    color = median;

}

