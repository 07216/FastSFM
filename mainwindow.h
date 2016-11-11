#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <Python.h>
#include <QMainWindow>
#include <QPushButton>
#include <QToolButton>
#include <QGroupBox>
#include <QLabel>
#include "settingdialog.h"
#include "readdialog.h"
//#include "OpenGL.h"
#include "videodialog.h"
#include "opencv2/opencv.hpp"
//#include "mve/libs/mve/mesh.h"

namespace Ui {
class MainWindow;
}

enum DATATYPE
{
    UNKNOWNTYPE,
    IMAGES,
    VIDEO
};

struct CameraIntrinsicParas
{
    double fx, fy, s, px, py;
    bool use;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void createPushButton();
    void createLayout();
    void createGL();
    void createStatusBar();
    void createDialog();

    void initVariables();

    void copyImages(QStringList filenames);

    void video2Images(std::vector<qint64> frames, int step);

    void updateLabels();

    void readMesh();

//    void readMesh(mve::TriangleMesh &mesh);
    void readImages();
    // read all the images under a folder
    void readFolder();
    void readVideo();

private slots:

    void readFile();

    void saveMesh();

    void SFM();

    void openSettings();

    void clearImages();

    void selectVideoFrames();

private:
    Ui::MainWindow *ui;

    //COpenGL *glWidget_;

    QPushButton *pushButton_saveMesh_;

    QPushButton *pushButton_clearImages_;

    QPushButton *pushButton_settings_;

    QPushButton *pushButton_readFiles_;

    QPushButton *pushButton_SFM_;

    QPushButton *pushButton_video2Images_;

    QLabel *label_numImages_;
    QLabel *label_numVertices_;
    QLabel *label_numFaces_;

    QMenu *menu_file_;

    ReadDialog  *readDialog_;
    SettingDialog *settingDialog_;
    VideoDialog *videoDialog_;

    DATATYPE data_type_;

    int numImages_;

    bool has_mesh_;

    std::string video_name_;
    std::vector<int> selected_frames_;

    CameraIntrinsicParas camera_paras_;

    /*PyObject* pModule;
    PyObject* pFun;
    int framenum;*/

};

#endif // MAINWINDOW_H
