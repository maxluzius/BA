#ifndef EDITCANNYDIALOG_H
#define EDITCANNYDIALOG_H

#include <QDialog>
#include "edgedetectorcanny.h"
#include "openglwidget.h"

namespace Ui {
class EditCannyDialog;
}

/**
 * @brief The EditCannyDialog class.
 * This purpose represents an QT Widget to edit the canny properties. It is not documented as well as other classes as it only delegates values to the EdgeDetectorCanny class
 */
class EditCannyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditCannyDialog(QWidget *parent = 0);
    ~EditCannyDialog();

    EdgeDetectorCanny *getCanny() const;
    void setCanny(EdgeDetectorCanny *value);

    OpenGLWidget *getOglWidget() const;
    void setOglWidget(OpenGLWidget *value);

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_kernelY_valueChanged(int arg1);

    void on_kernelX_valueChanged(int arg1);

    void on_t1_valueChanged(double arg1);

    void on_t2_valueChanged(double arg1);

    void on_sobel_valueChanged(int arg1);

    void on_accurateGradient_clicked(bool checked);

private:
    Ui::EditCannyDialog *ui;
    EdgeDetectorCanny* canny;
    OpenGLWidget* oglWidget;
};

#endif // EDITCANNYDIALOG_H
