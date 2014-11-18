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

void main() {
    gl_Position = vec4(positionAttribute, 0, 1); 
    passUV = (positionAttribute * 0.5) + 0.5;

     vec2 widthStep = vec2(1.0/720, 0.0);
     vec2 widthStepD = vec2(2.0/720, 0.0);
     vec2 heightStep = vec2(0.0, 1.0/480);
     vec2 heightStepD = vec2(0.0, 2.0/480);
     vec2 widthHeightStep = vec2(1.0/720, 1.0/480);
     vec2 widthHeightStepD = vec2(2.0/720, 2.0/480);
     vec2 widthNegativeHeightStep = vec2(1.0/720, -1.0/480);
     vec2 widthNegativeHeightStepD = vec2(2.0/720, -2.0/480);

     textureCoordinate = passUV.xy;
     leftTextureCoordinate = passUV.xy - widthStep;
     rightTextureCoordinate = passUV.xy + widthStep;

     topTextureCoordinate = passUV.xy + heightStep;
     topLeftTextureCoordinate = passUV.xy - widthNegativeHeightStep;
     topRightTextureCoordinate = passUV.xy + widthHeightStep;

     bottomTextureCoordinate = passUV.xy - heightStep;
     bottomLeftTextureCoordinate = passUV.xy - widthHeightStep;
     bottomRightTextureCoordinate = passUV.xy + widthNegativeHeightStep;

	 topLeft = passUV.xy - widthNegativeHeightStepD;
	 topSemiLeft = passUV.xy - widthNegativeHeightStepD + widthStep;
	 top = passUV.xy + heightStepD;
	 topSemiRight = passUV.xy + widthNegativeHeightStepD - widthStep;
	 topRight = passUV.xy + widthHeightStepD;

	 rightSemiTop = passUV.xy + widthStepD + heightStep;
	 right = passUV.xy + widthStepD;
	 rightSemiBot = passUV.xy + widthStepD - heightStep;

	 botRight = passUV.xy + widthNegativeHeightStepD;
	 botSemiRight = passUV.xy - heightStepD + widthStep;
	 bot = passUV.xy - heightStepD;
	 botSemiLeft = passUV.xy - heightStepD - widthStep;
	 botLeft = passUV.xy - widthHeightStepD;

	 leftSemiBot = passUV.xy - widthStepD - heightStep;
	 left = passUV.xy - widthStepD;
	 leftSemiTop = passUV.xy - widthHeightStepD + heightStep;
}
