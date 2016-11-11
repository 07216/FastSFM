#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <string>
#include <sstream>
#include <QByteArray>
#include <QBuffer>
#include <istream>
#include <QMessageBox>
#include <omp.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(200,50,800,600);

    this->setWindowTitle(tr("USFM"));

    createPushButton();
    createGL();
    createLayout();
    createStatusBar();
    createDialog();
    initVariables();
    /*Py_Initialize();
    pModule = PyImport_ImportModule("video");
    pFun= PyObject_GetAttrString(pModule,"vframes");*/
}

MainWindow::~MainWindow()
{
    delete ui;
    clearImages();
//    Py_Finalize();
}


void MainWindow::createPushButton()
{
    pushButton_readFiles_ = new QPushButton;
    pushButton_readFiles_->setIcon(QIcon("../USFM/Icons/import.png"));
    pushButton_readFiles_->setIconSize(QSize(40,40));
    pushButton_readFiles_->setStatusTip(tr("Import files..."));
    connect(pushButton_readFiles_, SIGNAL(clicked()), this, SLOT(readFile()));

    pushButton_video2Images_ = new QPushButton;
    pushButton_video2Images_->setIcon(QIcon("../USFM/Icons/video2images.png"));
    pushButton_video2Images_->setIconSize(QSize(40,40));
    pushButton_video2Images_->setStatusTip(tr("Selected video clips and convert them to images..."));
    connect(pushButton_video2Images_, SIGNAL(clicked()), this, SLOT(selectVideoFrames()));

    pushButton_clearImages_ = new QPushButton;
    pushButton_clearImages_->setIcon(QIcon("../USFM/Icons/delete.png"));
    pushButton_clearImages_->setIconSize(QSize(40,40));
    pushButton_clearImages_->setStatusTip(tr("Clear all images..."));
    connect(pushButton_clearImages_, SIGNAL(clicked()), this, SLOT(clearImages()));

    pushButton_saveMesh_ = new QPushButton;
    pushButton_saveMesh_->setIcon(QIcon("../USFM/Icons/save.png"));
    pushButton_saveMesh_->setIconSize(QSize(40,40));
    pushButton_saveMesh_->setStatusTip(tr("Save mesh..."));
    connect(pushButton_saveMesh_, SIGNAL(clicked()), this, SLOT(saveMesh()));

    pushButton_SFM_ = new QPushButton;
    pushButton_SFM_->setIcon(QIcon("../USFM/Icons/SFM.png"));
    pushButton_SFM_->setIconSize(QSize(40,40));
    pushButton_SFM_->setStatusTip(tr("Start SFM..."));
    connect(pushButton_SFM_, SIGNAL(clicked()), this, SLOT(SFM()));

    pushButton_settings_ = new QPushButton;
    pushButton_settings_->setIcon(QIcon("../USFM/Icons/settings.png"));
    pushButton_settings_->setIconSize(QSize(40,40));
    pushButton_settings_->setStatusTip(tr("Detailed settings..."));
    connect(pushButton_settings_, SIGNAL(clicked()), this, SLOT(openSettings()));

}

void MainWindow::createGL()
{
    //QGLFormat glf = QGLFormat::defaultFormat();
    //glf.setSampleBuffers(true);
    //glWidget_ = new COpenGL(glf);
}

void MainWindow::createLayout()
{   
    QVBoxLayout *layout_button = new QVBoxLayout;
    layout_button->setAlignment(Qt::AlignCenter);
    layout_button->addWidget(pushButton_readFiles_);
    QLabel *label_readFiles = new QLabel;
    label_readFiles->setAlignment(Qt::AlignHCenter);
    label_readFiles->setText(tr("Import files"));
    layout_button->addWidget(label_readFiles);

    layout_button->addSpacing(10);
    layout_button->addWidget(pushButton_video2Images_);
    QLabel *label_video2Image = new QLabel;
    label_video2Image->setAlignment(Qt::AlignHCenter);
    label_video2Image->setText(tr("Convert video to images"));
    layout_button->addWidget(label_video2Image);

    layout_button->addSpacing(10);
    layout_button->addWidget(pushButton_saveMesh_);
    QLabel *label_saveMesh = new QLabel;
    label_saveMesh->setAlignment(Qt::AlignHCenter);
    label_saveMesh->setText(tr("Save mesh"));
    layout_button->addWidget(label_saveMesh);

    layout_button->addSpacing(10);
    layout_button->addWidget(pushButton_SFM_);
    QLabel *label_SFM = new QLabel;
    label_SFM->setAlignment(Qt::AlignHCenter);
    label_SFM->setText(tr("Start SFM"));
    layout_button->addWidget(label_SFM);

    layout_button->addSpacing(10);
    layout_button->addWidget(pushButton_settings_);
    QLabel *label_settings = new QLabel;
    label_settings->setAlignment(Qt::AlignHCenter);
    label_settings->setText(tr("Settings"));
    layout_button->addWidget(label_settings);

    layout_button->addSpacing(10);
    layout_button->addWidget(pushButton_clearImages_);
    QLabel *label_clear = new QLabel;
    label_clear->setAlignment(Qt::AlignHCenter);
    label_clear->setText(tr("Clear all images"));
    layout_button->addWidget(label_clear);


    QHBoxLayout *layout_main = new QHBoxLayout;
    layout_main->addLayout(layout_button);
    //layout_main->addWidget(glWidget_);
    layout_main->setStretch(1, 1);
    this->centralWidget()->setLayout(layout_main);
}

void MainWindow::createStatusBar()
{
    this->statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));

    label_numImages_ = new QLabel;
    this->statusBar()->addWidget(label_numImages_);

    label_numVertices_ = new QLabel;
    this->statusBar()->addWidget(label_numVertices_);

    label_numFaces_ = new QLabel;
    this->statusBar()->addWidget(label_numFaces_);
}

void MainWindow::createDialog()
{
    settingDialog_ = new SettingDialog;

    readDialog_ = new ReadDialog;

    videoDialog_ = new VideoDialog;
}

void MainWindow::initVariables()
{
    data_type_ = UNKNOWNTYPE;
    numImages_ = 0;
    has_mesh_ = false;

    camera_paras_.fx = 0;
    camera_paras_.fy = 0;
    camera_paras_.s = 0;
    camera_paras_.px = 0;
    camera_paras_.py = 0;
    camera_paras_.use = false;

    updateLabels();
}

void MainWindow::updateLabels()
{
    std::stringstream ss;
    ss << numImages_;
    std::string text = "Images: "+ss.str()+"  ";
    label_numImages_->setText(text.data());

    // need to change to mesh info
    text = "Vertices: " + ss.str() + "  ";
    label_numVertices_->setText(text.data());

    text = "Faces: " + ss.str() + "  ";
    label_numFaces_->setText(text.data());
}

void MainWindow::readFile()
{
    readDialog_->exec();
    switch (readDialog_->getType())
    {
    case READIMAGES:
        readImages();
        break;
    case READFOLDER:
        readFolder();
        break;
    case READVIDEO:
        readVideo();
        break;
    default:
        break;
    }
}

void MainWindow::readImages()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Image Files (*.png *.jpg *.bmp)"));
    if (dialog.exec())
    {
        QStringList filenames = dialog.selectedFiles();
        copyImages(filenames);
        data_type_ = DATATYPE::IMAGES;
    }
    updateLabels();
}

void MainWindow::readFolder()
{
    QString path;
    QFileDialog dialog(this);
    path = dialog.getExistingDirectory(this,tr("Read a folder"));
    if (!path.isEmpty())
    {
        QDir dir;
        dir.setPath(path);
        QStringList filters;
        filters << "*.jpg" << "*.png" << "*.bmp";
        dir.setNameFilters(filters);
        QStringList names = dir.entryList();
        QStringList fullnames;
        for (int i = 0; i < names.size(); ++i)
        {
            std::string temp = path.toStdString() + "/"  + names.at(i).toStdString();
            fullnames.push_back(QString(temp.data()));
        }
        copyImages(fullnames);
        data_type_ = DATATYPE::IMAGES;
    }
    updateLabels();
}

void MainWindow::readVideo()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Vedio File (*.mp4 *.avi *.flv)"));
    if (dialog.exec())
    {
        QStringList filenames = dialog.selectedFiles();
        video_name_ = filenames.at(0).toStdString();
        data_type_ = DATATYPE::VIDEO;
    }
    updateLabels();
}

void MainWindow::saveMesh()
{

}

void MainWindow::copyImages(QStringList filenames)
{
#pragma omp parallel for
    for (int i = 0; i < filenames.size(); ++i)
    {
        QString name = filenames.at(i);
        QImage *image = new QImage(name);
        std::stringstream ss;
        ss << numImages_ + i;
        std::string newname = "./Images/"+ss.str()+".jpg";
        image->save(newname.data(), "JPG");
    }

    numImages_ += filenames.size();
}

void MainWindow::video2Images(std::vector<qint64> frames, int step)
{
    // convert video to images, video path can be got from video_name_
    int a,b,i;
    int df,fps;
    cv::VideoCapture cap;
    std::vector<qint64>::iterator iter_;
    i=0;
//    cap.open(video_name_);
//    fps=cap.get(CV_CAP_PROP_FPS);
    for(iter_=frames.begin();iter_!=frames.end();iter_+=2)
    {
        i++;
        a=*iter_;
        b=*(iter_+1);
//        PyObject_CallFunction(pFun,"iiisi",a,b,step,video_name_,i);
    }
}

void MainWindow::selectVideoFrames()
{
    // ...
    if (data_type_ == VIDEO)
    {
        videoDialog_->initVideoPath(QString(video_name_.data()));
        if (!videoDialog_->exec())
        {
            videoDialog_->reset();
            return;
        }
        // get sample and frames
        // frames are stored like: (1st begin, 1st end, 2nd begin, 2nd end.....);
        // if step equals -1, auto choose step
        int step;
        std::vector<qint64> frames;
        if (videoDialog_->getSelectedFrames(frames, step))
        {
            videoDialog_->reset();
            video2Images(frames, step);
        }
        else
        {
            videoDialog_->reset();
            QMessageBox::information(this, tr("Convert won't start!"), tr("frames must be paired!"), QMessageBox::Ok);
        }
    }
}

void MainWindow::clearImages()
{
    numImages_ = 0;
    QDir dir;
    dir.setPath("./Images");
    QStringList filenames = dir.entryList();
    for (int i = 0; i < filenames.size(); ++i)
    {
        dir.remove(filenames.at(i));
    }
    data_type_ = DATATYPE::UNKNOWNTYPE;
    updateLabels();
}

void MainWindow::openSettings()
{
    if(settingDialog_->exec())
    {
        if (settingDialog_->useCameraParas())
        {
            settingDialog_->getCameraParameters(
                        camera_paras_.fx, camera_paras_.fy, camera_paras_.s, camera_paras_.px, camera_paras_.py);
            camera_paras_.use = true;
        }

    }
}

void MainWindow::SFM()
{
    if (numImages_ < 2)
    {
        QMessageBox::information(this,tr("SFM won't start!"),tr("At least 2 images are required."), QMessageBox::Ok);
        return;
    }
    // do MVE or other things and return a mesh

    readMesh();
}

void MainWindow::readMesh()
{
    //glWidget_->read_mesh("./output/result.ply");
}

//void MainWindow::readMesh(mve::TriangleMesh &mesh)
//{
//    glWidget_->read_mesh(mesh);
//}
