#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>
#include "line3d.h"

class EditObjectWidget;

/**
 * @brief The Mesh class
 * represents an OGL drawable Line-Mesh
 */
class Mesh
{
public:
    /**
     * @brief Mesh ctr
     */
    Mesh();

    /**
     * @brief Mesh dtr
     */
    ~Mesh();

    std::vector<Line3D*> lines; ///< 3D lines of the mesh
    std::vector<float> vertices; ///< Vertices
    std::vector<float> normals; ///< Normals of the Lines/Vertices (for visibility check)
    std::vector<float> normalsSecondary; ///< Every vertex has two normals
    std::vector<unsigned int> indices; ///< Indices of the Vertices

    /**
     * @brief initialize OGL buffers
     */
    void init();

    /**
     * @brief draw the Mesh
     * @param camPos - if nullptr no visibility check is performed
     */
    void draw(glm::vec3* camPos = nullptr);

    glm::vec3 getTranslate() const;
    void setTranslate(const glm::vec3 &value);
    void setTranslateX(const float x);
    void setTranslateY(const float y);
    void setTranslateZ(const float z);

    glm::vec3 getRotate() const;
    void setRotate(const glm::vec3 &value);
    void setRotateX(const float x);
    void setRotateY(const float y);
    void setRotateZ(const float z);

    glm::vec3 getScale() const;
    void setScale(const glm::vec3 &value);
    void setScaleX(const float x);
    void setScaleY(const float y);
    void setScaleZ(const float z);

    float getYaw() const;
    float getRoll() const;
    float getPitch() const;

    /**
     * @brief editObjectWidget the Qt Eidget to edit the mesh tranformation
     * @return
     */
    EditObjectWidget *editObjectWidget() const;
    void setEditObjectWidget(EditObjectWidget *editObjectWidget);

    /**
     * @brief Are normals assigned to the mesh?
     * @return true if normals are assigned
     */
    bool hasNormals() const;

    /**
     * @brief Sets if the mesh has normals
     * @param hasNormals
     */
    void setHasNormals(bool hasNormals);

    /**
     * @brief Sets if a visibility check should be performed
     * @param doCheck toggle the visibility check
     */
    void doVisibilityCheck(bool doCheck);

    /**
     * @brief compute the Model Matrix
     * @return the model matrix
     */
    glm::mat4& computeModelMatrix();

    /**
     * @brief Sets the viewport width for line projection
     * @param value
     */
    void setVPWidth(int value);

    /**
     * @brief Sets the viewport height for line projection
     * @param value
     */
    void setVPHeight(int value);

    /**
     * @brief set the Projection matrix
     * @param value
     */
    void setP(glm::mat4 *value);

    /**
     * @brief set the View matrix
     * @param value
     */
    void setV(glm::mat4 *value);

private:
    unsigned int _vao;  ///> Vertex Array Object
    unsigned int _bufferVert;   ///< Buffer for the Vertices
    unsigned int _bufferIndices;    ///< Buffer for the Indicies

    std::vector<Line3D*> _visibleLines; ///< Visible Lines of the current frame
    std::vector<unsigned int> _visibleIndices; ///< Provide only indices of visible lines to the gpu

    bool _hasNormals; ///< Do the mesh has normals?

    bool _doVisibilityCheck; ///< Do a visibility check?

    glm::vec3 translate;
    glm::vec3 rotate;
    glm::vec3 scale;

    glm::mat4 M; ///< Modelview
    glm::mat4* P; ///< Projection
    glm::mat4* V; ///< Viewmatrix

    int vpWidth; ///< viewport width
    int vpHeight; ///< viewport height

    EditObjectWidget* _editObjectWidget; ///< The qt widget to edit mesh properties
};

#endif // MESH_H
