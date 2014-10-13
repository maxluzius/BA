#include "Line.h"
#include "line3d.h"

Line::Line(cv::Point p0, cv::Point p1) : _p0(p0),_p1(p1)
{
	init();
}

Line::Line(cv::Vec4i v) : _p0(cv::Point(v[0],v[1])), _p1(cv::Point(v[2],v[3]))
{
	init();
}

Line::Line()
{
}

Line::~Line(void)
{
	
}

void Line::init()
{
	// Compute the angle theta.
    _theta = std::atan2((float)(_p1.y - _p0.y),(float)(_p1.x  - _p0.x));

    // "normalize" the angle to be in (0,2pi).
	if(_theta < 0)
		_theta = 2*CV_PI + _theta; 
}

cv::Point& Line::operator[](short const& index)
{
	assert(index == 0 || index == 1);

	if(index == 0)
		return _p0;
	if(index == 1)
		return _p1;
}

const cv::Point& Line::operator[](short const& index) const
{
	assert(index == 0 || index == 1);

	if(index == 0)
		return _p0;
	if(index == 1)
		return _p1;
}

Line::operator cv::Vec4i()
{
	return cv::Vec4i(_p0.x,_p0.y,_p1.x,_p1.y);
}

Line3D *Line::lineIn3D() const
{
    return _lineIn3D;
}

void Line::setLineIn3D(Line3D *lineIn3D)
{
    _lineIn3D = lineIn3D;
}
