#include "editobjectwidget.h"
#include "ui_editobjectwidget.h"

EditObjectWidget::EditObjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditObjectWidget)
{
    ui->setupUi(this);

    move(100,100);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);

    setWindowTitle("Edit Transform");
}

EditObjectWidget::~EditObjectWidget()
{
    delete ui;
}

void EditObjectWidget::on_spinPositionX_valueChanged(double arg1)
{
    mesh->setTranslateX(arg1);
}

void EditObjectWidget::on_spinPositionY_valueChanged(double arg1)
{
    mesh->setTranslateY(arg1);
}

void EditObjectWidget::on_spinPositionZ_valueChanged(double arg1)
{
    mesh->setTranslateZ(arg1);
}

void EditObjectWidget::on_spinRotX_valueChanged(double arg1)
{
    mesh->setRotateX(arg1);
}

void EditObjectWidget::on_spinRotY_valueChanged(double arg1)
{
    mesh->setRotateY(arg1);
}

void EditObjectWidget::on_spinRotZ_valueChanged(double arg1)
{
    mesh->setRotateZ(arg1);
}

void EditObjectWidget::on_spinScaleX_valueChanged(double arg1)
{
    mesh->setScaleX(arg1);
}

void EditObjectWidget::on_spinScaleY_valueChanged(double arg1)
{
    mesh->setScaleY(arg1);
}

void EditObjectWidget::on_spinScaleZ_valueChanged(double arg1)
{
    mesh->setScaleZ(arg1);
}

Mesh *EditObjectWidget::getMesh() const
{
    return mesh;
}

void EditObjectWidget::setMesh(Mesh *value)
{

    mesh = value;

    ui->spinPositionX->setValue(mesh->getTranslate().x);
    ui->spinPositionY->setValue(mesh->getTranslate().y);
    ui->spinPositionZ->setValue(mesh->getTranslate().z);

    ui->spinRotX->setValue(mesh->getRotate().x);
    ui->spinRotY->setValue(mesh->getRotate().y);
    ui->spinRotZ->setValue(mesh->getRotate().z);

    ui->spinScaleX->setValue(mesh->getScale().x);
    ui->spinScaleY->setValue(mesh->getScale().y);
    ui->spinScaleZ->setValue(mesh->getScale().z);
}

