#include "readdialog.h"
#include "ui_readdialog.h"
#include <QGridLayout>
#include <QLabel>

ReadDialog::ReadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadDialog)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Read files"));
    this->setFixedSize(QSize(600,200));

    read_type_ = NOTSELECTED;
    createButton();
    createLayout();
}

ReadDialog::~ReadDialog()
{
    delete ui;
}

void ReadDialog::createButton()
{
    pushButton_readImages_ = new QPushButton;
    pushButton_readImages_->setIcon(QIcon("../USFM/Icons/readImages.png"));
    pushButton_readImages_->setIconSize(QSize(100,100));
    pushButton_readImages_->setStatusTip(tr("Read images..."));
    connect(pushButton_readImages_, SIGNAL(clicked()), this, SLOT(setTypeImages()));

    pushButton_readFolder_ = new QPushButton;
    pushButton_readFolder_->setIcon(QIcon("../USFM/Icons/readFolder.png"));
    pushButton_readFolder_->setIconSize(QSize(100,100));
    pushButton_readFolder_->setStatusTip(tr("Read images from a folder..."));
    connect(pushButton_readFolder_, SIGNAL(clicked()), this, SLOT(setTypeFolder()));

    pushButton_readVideo_ = new QPushButton;
    pushButton_readVideo_->setIcon(QIcon("../USFM/Icons/readVideo.png"));
    pushButton_readVideo_->setIconSize(QSize(100,100));
    pushButton_readVideo_->setStatusTip(tr("Read a video..."));
    connect(pushButton_readVideo_, SIGNAL(clicked()), this, SLOT(setTypeVideo()));

    pushButton_quit_ = new QPushButton(tr("Quit"));
    connect(pushButton_quit_, SIGNAL(clicked()), this, SLOT(setTypeNone()));
}

void ReadDialog::createLayout()
{
    QGridLayout *layout_main = new QGridLayout;
    this->setLayout(layout_main);
    layout_main->setRowStretch(0, 5);
    layout_main->setRowStretch(1, 1);
    layout_main->setRowStretch(2, 1);
    layout_main->addWidget(pushButton_readImages_, 0, 0);
    layout_main->addWidget(pushButton_readFolder_, 0, 1);
    layout_main->addWidget(pushButton_readVideo_, 0, 2);

    QLabel *label_readImages = new QLabel(tr("read images"));
    label_readImages->setAlignment(Qt::AlignHCenter);

    QLabel *label_readFolder = new QLabel(tr("read a folder"));
    label_readFolder->setAlignment(Qt::AlignHCenter);

    QLabel *label_readVideo = new QLabel(tr("read a video"));
    label_readVideo->setAlignment(Qt::AlignHCenter);

    layout_main->addWidget(label_readImages, 1, 0);
    layout_main->addWidget(label_readFolder, 1, 1);
    layout_main->addWidget(label_readVideo, 1, 2);

    layout_main->addWidget(pushButton_quit_, 2, 2);
}

void ReadDialog::setTypeImages()
{
    read_type_ = READIMAGES;
    this->close();
}

void ReadDialog::setTypeFolder()
{
    read_type_ = READFOLDER;
    this->close();
}

void ReadDialog::setTypeVideo()
{
    read_type_ = READVIDEO;
    this->close();
}

void ReadDialog::setTypeNone()
{
    read_type_ = NOTSELECTED;
    this->close();
}
