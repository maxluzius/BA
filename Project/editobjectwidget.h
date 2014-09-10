#ifndef EDITOBJECTWIDGET_H
#define EDITOBJECTWIDGET_H

#include <QWidget>
#include "mesh.h"

namespace Ui {
class EditObjectWidget;
}

/**
 * @brief The EditObjectWidget class.
 * This class represents an QT Widget to edit the transformation of the mesh in the scene. It is not documented as well as other classes as it only delegates values to the Mesh class
 */
class EditObjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditObjectWidget(QWidget *parent = 0);
    ~EditObjectWidget();

    Mesh *getMesh() const;
    void setMesh(Mesh *value);

private slots:
    void on_spinPositionX_valueChanged(double arg1);
    void on_spinPositionY_valueChanged(double arg1);
    void on_spinPositionZ_valueChanged(double arg1);

    void on_spinRotX_valueChanged(double arg1);
    void on_spinRotY_valueChanged(double arg1);
    void on_spinRotZ_valueChanged(double arg1);

    void on_spinScaleX_valueChanged(double arg1);
    void on_spinScaleY_valueChanged(double arg1);
    void on_spinScaleZ_valueChanged(double arg1);

private:
    Ui::EditObjectWidget *ui;
    Mesh* mesh;
};

#endif // EDITOBJECTWIDGET_H
