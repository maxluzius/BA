#ifndef LINE_H
#define LINE_H

#include <opencv2/opencv.hpp>

class Line3D;

/**
	\brief A line in 2D

    Represents a line in 2D space. A line is described through two points p0 and p1

	@author Jochen Hunz
*/
class Line
{
public:
	/**
		Ctor of a line.
		
		@param p0 the start point of the line in pixel coordinates.
		@param p1 the end point of the line in poxel coordinates.
 
	*/
	Line(cv::Point p0, cv::Point p1);

	/**
		Default Ctor
	*/
	Line();

	/**
		Ctor of a line.
		
		@param v a cv::vec4i where the first two components holds the start- and the last two components holds the endpoint of the line in pixel coordinates.
 
	*/
	Line(cv::Vec4i v);

	/**
		Dtor of a line.
	*/
	virtual ~Line(void);

	/**
		Operator [] for access of the line startpoint (index==0) and the line endpoint (index == 1).
	*/
	cv::Point& operator[](short const& index);

	/**
		Operator [] for access of the const line startpoint (index==0) and the const line endpoint (index == 1).
	*/
	const cv::Point& operator[](short const& index) const;

	/**
		Allows implicit casts from Line to cv::Vec4i.
	*/
	operator cv::Vec4i ();

	/**
        Get the angle between p0 and p1.

		@return the angle between p0 and p1.
	*/
	const inline float getTheta() const {return _theta;};

	/**
		Returns the startpoint p0 for index == 0 and the endpoint p1 for index == 1.

		@param index p0 if index == 0 and p1 if index == 1.
		@return the start- or endpoint of the line.
	*/
	inline cv::Point get(int index) 
	{
		if(index==0)
			return _p0; 
		else 
			return _p1;
	};

    /**
        Sets the startpoint p0 for index == 0 and the endpoint p1 for index == 1.

        @param index p0 if index == 0 and p1 if index == 1.
        @param p the point to set.
    */
    inline void set(int index, cv::Point p)
    {
        if(index==0)
            _p0 = p;
        else
            _p1 = p;
    };

    /**
     * @brief lineIn3D the line in 3d - if any
     * @return the Line in 3D - nullprt if no 3d line was set
     */
    Line3D *lineIn3D() const;

    /**
     * @brief Sets the corresponding 3D line
     * @param lineIn3D
     */
    void setLineIn3D(Line3D *lineIn3D);

    /**
        Initializes the line.
    */
    void init();

private:
    cv::Point _p0,_p1;      ///< start- and endpoint of the line.

    float _theta;           ///< angle between _p0 and _p1.

    Line3D* _lineIn3D;      ///< Corresponding Line in 3D - if any
};

#endif
