#include "settingdialog.h"
#include "ui_settingdialog.h"
#include <QGridLayout>
#include <QPushButton>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Settings"));

    createParas();
    createGroupBoxs();
    createLayouts();
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

bool SettingDialog::useCameraParas()
{
    return use_camera_paras_;
}

void SettingDialog::getCameraParameters(double &fx, double &fy, double &s, double &px, double &py)
{
    fx = fx_;
    fy = fy_;
    s = s_;
    px = px_;
    py = py_;
}

void SettingDialog::initVariables()
{
    fx_ = 0;
    fy_ = 0;
    s_ = 0;
    px_ = 0;
    py_ = 0;
    use_camera_paras_ = false;
}

void SettingDialog::createParas()
{
    // create QDoubleSpinBoxs
    dSpinBox_fx_ = new QDoubleSpinBox;
    dSpinBox_fx_->setMaximum(10000);
    dSpinBox_fx_->setMinimum(0);
    dSpinBox_fx_->setDecimals(4);
    dSpinBox_fx_->setSingleStep(100);
    dSpinBox_fx_->setValue(0);
    dSpinBox_fx_->setAlignment(Qt::AlignHCenter);

    dSpinBox_fy_ = new QDoubleSpinBox;
    dSpinBox_fy_->setMaximum(10000);
    dSpinBox_fy_->setMinimum(0);
    dSpinBox_fy_->setDecimals(4);
    dSpinBox_fy_->setSingleStep(100);
    dSpinBox_fy_->setValue(0);
    dSpinBox_fy_->setAlignment(Qt::AlignHCenter);

    dSpinBox_s_ = new QDoubleSpinBox;
    dSpinBox_s_->setMaximum(10000);
    dSpinBox_s_->setMinimum(0);
    dSpinBox_s_->setDecimals(4);
    dSpinBox_s_->setSingleStep(10);
    dSpinBox_s_->setValue(0);
    dSpinBox_s_->setAlignment(Qt::AlignHCenter);

    dSpinBox_px_ = new QDoubleSpinBox;
    dSpinBox_px_->setMaximum(10000);
    dSpinBox_px_->setMinimum(0);
    dSpinBox_px_->setDecimals(4);
    dSpinBox_px_->setSingleStep(10);
    dSpinBox_px_->setValue(0);
    dSpinBox_px_->setAlignment(Qt::AlignHCenter);

    dSpinBox_py_ = new QDoubleSpinBox;
    dSpinBox_py_->setMaximum(10000);
    dSpinBox_py_->setMinimum(0);
    dSpinBox_py_->setDecimals(4);
    dSpinBox_py_->setSingleStep(10);
    dSpinBox_py_->setValue(0);
    dSpinBox_py_->setAlignment(Qt::AlignHCenter);


    // create QCheckBoxes
    checkBox_useCameraParas_ = new QCheckBox;
    checkBox_useCameraParas_->setCheckable(true);
    checkBox_useCameraParas_->setCheckState(Qt::Unchecked);

}

void SettingDialog::createGroupBoxs()
{
    // camera parameter
    groupBox_cameraParas_ = new QGroupBox(tr("Camera: Intrinsic Parameters"));
    QGridLayout *layout_cameraParas = new QGridLayout;
    layout_cameraParas->setSpacing(5);
    layout_cameraParas->setColumnStretch(0, 1);
    layout_cameraParas->setColumnStretch(1, 4);
    layout_cameraParas->setColumnStretch(2, 4);
    groupBox_cameraParas_->setLayout(layout_cameraParas);

    layout_cameraParas->addWidget(new QLabel(tr("fx")), 0, 0);
    layout_cameraParas->addWidget(dSpinBox_fx_, 0, 1);

    layout_cameraParas->addWidget(new QLabel(tr("fy")), 1, 0);
    layout_cameraParas->addWidget(dSpinBox_fy_, 1, 1);

    layout_cameraParas->addWidget(new QLabel(tr("s")), 2, 0);
    layout_cameraParas->addWidget(dSpinBox_s_, 2, 1);

    layout_cameraParas->addWidget(new QLabel(tr("px")), 3, 0);
    layout_cameraParas->addWidget(dSpinBox_px_, 3, 1);

    layout_cameraParas->addWidget(new QLabel(tr("py")), 4, 0);
    layout_cameraParas->addWidget(dSpinBox_py_, 4, 1);

    QLabel *label_image = new QLabel;
    //label_image->setScaledContents(true);
    QPixmap pixelmap;
    pixelmap.load(tr("../USFM/Icons/cameraPara.png"));
    label_image->setPixmap(pixelmap);

    layout_cameraParas->addWidget(label_image, 0, 2, 5, 1);
}

void SettingDialog::createLayouts()
{
    QGridLayout *layout_main = new QGridLayout;
    this->setLayout(layout_main);

    layout_main->addWidget(groupBox_cameraParas_, 0, 0, 1, 4);

    layout_main->addWidget(new QLabel(tr("use camera parameters")), 1, 0);
    layout_main->addWidget(checkBox_useCameraParas_, 1, 1);

    QPushButton *pushButton_OK = new QPushButton(tr("OK"));
    QPushButton *pushButton_QUIT = new QPushButton(tr("QUIT"));

    connect(pushButton_OK, SIGNAL(clicked()), this, SLOT(closeOK()));
    connect(pushButton_QUIT, SIGNAL(clicked()), this, SLOT(closeQUIT()));

    layout_main->addWidget(pushButton_OK, 5, 2);
    layout_main->addWidget(pushButton_QUIT, 5, 3);
}

void SettingDialog::closeOK()
{
    fx_ = dSpinBox_fx_->value();
    fy_ = dSpinBox_fy_->value();
    s_ = dSpinBox_s_->value();
    px_ = dSpinBox_px_->value();
    py_ = dSpinBox_py_->value();
    use_camera_paras_ = checkBox_useCameraParas_->isChecked();

    this->accept();
}

void SettingDialog::closeQUIT()
{
    dSpinBox_fx_->setValue(fx_);
    dSpinBox_fy_->setValue(fy_);
    dSpinBox_s_->setValue(s_);
    dSpinBox_px_->setValue(px_);
    dSpinBox_py_->setValue(py_);
    checkBox_useCameraParas_->setChecked(use_camera_paras_);

    this->reject();
}
