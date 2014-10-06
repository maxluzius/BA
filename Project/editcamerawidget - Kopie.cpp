#include "editcamerawidget.h"
#include "ui_editcamerawidget.h"
#include "camera.h"

EditCameraWidget::EditCameraWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditCameraWidget)
{
    ui->setupUi(this);
    move(100,0);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);

    setWindowTitle("Edit Camera Parameters");
}

EditCameraWidget::~EditCameraWidget()
{
    delete ui;
}

void EditCameraWidget::on_spinFov_valueChanged(double arg1)
{
    camera->setFov(arg1);
}

void EditCameraWidget::on_spinUpX_valueChanged(double arg1)
{
    camera->setUpX(arg1);
}

void EditCameraWidget::on_spinUpZ_valueChanged(double arg1)
{
    camera->setUpZ(arg1);
}

void EditCameraWidget::on_spinUpY_valueChanged(double arg1)
{
    camera->setUpY(arg1);
}

void EditCameraWidget::on_spinLookX_valueChanged(double arg1)
{
    camera->setLookAtX(arg1);
}

void EditCameraWidget::on_spinLookY_valueChanged(double arg1)
{
    camera->setLookAtY(arg1);
}

void EditCameraWidget::on_spinLookZ_valueChanged(double arg1)
{
    camera->setLookAtZ(arg1);
}

void EditCameraWidget::on_spinCenterX_valueChanged(double arg1)
{
    camera->setCenterX(arg1);
}

void EditCameraWidget::on_spinCenterY_valueChanged(double arg1)
{
    camera->setCenterY(arg1);
}

void EditCameraWidget::on_spinCenterZ_valueChanged(double arg1)
{
    camera->setCenterZ(arg1);
}

Camera *EditCameraWidget::getCamera() const
{
    return camera;
}

void EditCameraWidget::setCamera(Camera *value)
{
    camera = value;

    ui->spinCenterX->setValue(camera->center().x);
    ui->spinCenterY->setValue(camera->center().y);
    ui->spinCenterZ->setValue(camera->center().z);

    ui->spinLookX->setValue(camera->lookAt().x);
    ui->spinLookY->setValue(camera->lookAt().y);
    ui->spinLookZ->setValue(camera->lookAt().z);

    ui->spinUpX->setValue(camera->up().x);
    ui->spinUpY->setValue(camera->up().y);
    ui->spinUpZ->setValue(camera->up().z);

    ui->spinFov->setValue(camera->fov());
}


void EditCameraWidget::on_Trackball_toggled(bool checked)
{
    camera->setTrackball(checked);
}
