#version 330

layout (location = 0) in vec2 positionAttribute;
out vec2 passUV;

 varying vec2 textureCoordinate;
 varying vec2 leftTextureCoordinate;
 varying vec2 rightTextureCoordinate;

 varying vec2 topTextureCoordinate;
 varying vec2 topLeftTextureCoordinate;
 varying vec2 topRightTextureCoordinate;

 varying vec2 bottomTextureCoordinate;
 varying vec2 bottomLeftTextureCoordinate;
 varying vec2 bottomRightTextureCoordinate;

void main() {
    gl_Position = vec4(positionAttribute, 0, 1); 
    passUV = (positionAttribute * 0.5) + 0.5;

    vec2 widthStep = vec2(1.0/720, 0.0);
     vec2 heightStep = vec2(0.0, 1.0/480);
     vec2 widthHeightStep = vec2(1.0/720, 1.0/480);
     vec2 widthNegativeHeightStep = vec2(1.0/720, -1.0/480);

     textureCoordinate = passUV.xy;
     leftTextureCoordinate = passUV.xy - widthStep;
     rightTextureCoordinate = passUV.xy + widthStep;

     topTextureCoordinate = passUV.xy + heightStep;
     topLeftTextureCoordinate = passUV.xy - widthNegativeHeightStep;
     topRightTextureCoordinate = passUV.xy + widthHeightStep;

     bottomTextureCoordinate = passUV.xy - heightStep;
     bottomLeftTextureCoordinate = passUV.xy - widthHeightStep;
     bottomRightTextureCoordinate = passUV.xy + widthNegativeHeightStep;
}
