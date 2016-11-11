#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();
    bool useCameraParas();
    void getCameraParameters(double &fx, double &fy, double &s, double &px, double &py);


private:
    void createLayouts();
    void createGroupBoxs();
    void createParas();
    void initVariables();

private slots:
    void closeOK();
    void closeQUIT();

private:
    Ui::SettingDialog *ui;

    QDoubleSpinBox *dSpinBox_fx_;
    QDoubleSpinBox *dSpinBox_fy_;
    QDoubleSpinBox *dSpinBox_s_;
    QDoubleSpinBox *dSpinBox_px_;
    QDoubleSpinBox *dSpinBox_py_;
    QGroupBox   *groupBox_cameraParas_;

    QCheckBox *checkBox_useCameraParas_;


private:
    // see Jianxiong's sildes for definitions
    double fx_, fy_, s_, px_, py_;

    bool use_camera_paras_;

};

#endif // SETTINGDIALOG_H
