#ifndef LINE3D_H
#define LINE3D_H

#include <glm/glm.hpp>
#include "opencv2/opencv.hpp"

class Line;

/**
 * @brief The Line3D class
 * Represents a line in the OpenGL 3D coordinate system. Is Projectable in 2d image space of opencv
 */
class Line3D
{
public:
    /**
     * @brief Line3D Ctr
     */
    Line3D();

    /**
     * @brief project the 3D line in 2D image space of opencv
     * @param M OGL ModelM
     * @param V OGL ViewM
     * @param P OGL ProjM
     * @param w Viewport width in pixels
     * @param h Viewport height in pixels
     * @return The 3D Line in 2D space as an instance of the Line class
     */
    Line* project(glm::mat4 &M, glm::mat4 &V,glm::mat4 &P,int w,int h);

    /**
     * @brief isVisible. Determines whether or not the
     * @param cameraPos Center point of the camera
     * @param M ModelMatrix of the mesh
     * @return true if the line is visible
     */
    bool isVisible(glm::vec3 &cameraPos, glm::mat4 &M);

    /**
     * @brief sets the start and endpoint of the 3d line
     * @param p0
     * @param p1
     */
    void setStartEnd(cv::Point3f p0, cv::Point3f p1);

    /**
     * @brief Get the center of the Line
     * @return The center of the line
     */
    glm::vec3 center() const;

    /**
     * @brief Sets the center of the line
     * @param center the center of the line
     */
    void setCenter(const glm::vec3 &center);

    /**
     * @brief indices
     * @return
     */
    cv::Vec2i indices() const;

    /**
     * @brief set the Indices
     * @param indices
     */
    void setIndices(const cv::Vec2i &indices);

    /**
     * @brief normal of the line (defined through Blender!)
     * @return
     */
    glm::vec3 normal() const;

    /**
     * @brief sets the Normal of the line
     * @param normal
     */
    void setNormal(const glm::vec3 &normal);

    /**
     * @brief Gets the second Normal of the line - if any
     * @return nullptr if no second normal is set.
     */
    glm::vec3 normalSec() const;

    /**
     * @brief sets the second normal of the line
     * @param normalSec
     */
    void setNormalSec(const glm::vec3 &normalSec);

    /**
     * @brief Returns the projected Line in 2D - if any
     * @return
     */
    Line *projected() const;

    /**
     * @brief sets the projected Line
     * @param projected the line in 2d opencv image space
     */
    void setProjected(Line *projected);

    /**
     * @brief p0 the first point of the line
     * @return
     */
    cv::Point3f p0() const;

    /**
     * @brief sets the first point
     * @param p0
     */
    void setP0(const cv::Point3f &p0);

    /**
     * @brief p1 the second point of the line
     * @return
     */
    cv::Point3f p1() const;

    /**
     * @brief sets the second point
     * @param p1
     */
    void setP1(const cv::Point3f &p1);

private:
    cv::Point3f _p0,_p1;    ///< start- and endpoint of the line.
    glm::vec3 _center;    ///< center point of the line.
    Line* _projected;   ///< the projected line in 2D.

    cv::Vec2i _indices;     ///< The indices (x,y) of the Vertices on the GPU.
    glm::vec3 _normal;      ///< The normal of the line used for the visibility check.
    glm::vec3 _normalSec;   ///< The second normal of the line - if any.
};

#endif // LINE3D_H
