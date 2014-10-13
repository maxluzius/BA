#ifndef EDITCAMERAWIDGET_H
#define EDITCAMERAWIDGET_H

#include <QWidget>
#include "camera.h"

namespace Ui {
class EditCameraWidget;
}

/**
 * @brief The EditCameraWidget class.
 * This class represents an QT Widget to edit the camera. It is not documented as well as other classes as it only delegates values to the Camera class
 */
class EditCameraWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditCameraWidget(QWidget *parent = 0);
    ~EditCameraWidget();

    Camera *getCamera() const;
    void setCamera(Camera *value);

private slots:
    void on_spinFov_valueChanged(double arg1);

    void on_spinUpX_valueChanged(double arg1);

    void on_spinUpZ_valueChanged(double arg1);

    void on_spinUpY_valueChanged(double arg1);

    void on_spinLookX_valueChanged(double arg1);

    void on_spinLookY_valueChanged(double arg1);

    void on_spinLookZ_valueChanged(double arg1);

    void on_spinCenterX_valueChanged(double arg1);

    void on_spinCenterY_valueChanged(double arg1);

    void on_spinCenterZ_valueChanged(double arg1);

    void on_Trackball_toggled(bool checked);

private:
    Ui::EditCameraWidget *ui;
    Camera* camera;
};

#endif // EDITCAMERAWIDGET_H
