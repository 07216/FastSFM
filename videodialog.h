#ifndef VIDEODIALOG_H
#define VIDEODIALOG_H

#include <QDialog>
#include <QVideoWidget>
#include <QString>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoFrame>
#include <QVideoProbe>
#include <vector>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QLineEdit>

namespace Ui {
class VideoDialog;
}

const int SLIDER_MAXIMUM = 100000;
const int MAX_CLIPS = 10;
class VideoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VideoDialog(QWidget *parent = 0);
    ~VideoDialog();

    void initVideoPath(QString path);
    void reset();

    bool getSelectedFrames(std::vector<qint64> &frames, int &step);

private:
    void createPlayer();
    void createLayout();
    void createSelectFramesWidgets();

    void updateSelectedFrames();

    void updateLabel();

    void createLineEdit();

private slots:
    void pause();
    void play();

    void setSliderPosition(qint64 pos);

    void updateCurrentPosition(int pos);

    void setVideoPosition();

    void selectFrame();

    void changeFirstFrameStatus(int status);

    void changeLastFrameStatus(int status);


private:
    Ui::VideoDialog *ui;

    QVideoWidget *videoWidget_;
    QMediaPlayer *player_;
    QMediaPlaylist *playlist_;
    QPushButton *pushButton_pause_;
    QPushButton *pushButton_play_;

    // select frame widgets
    QPushButton *pushButton_get_frame_;
    QSpinBox    *spinBox_frame_sample_step_;
    QWidget      *widget_disp_selected_frames_;
    QGridLayout *layout_disp_;


    QWidget *widget_select_frames_;

    QSlider *slider_;

    QLabel  *label_position_;

    double position_;

    std::vector<qint64> selected_frames_;
    QCheckBox *checkBox_first_frame_;
    QCheckBox *checkBox_last_frame_;

    QCheckBox *checkBox_auto_step_;

    std::vector<QLineEdit*> lineEdit_display_;

    QPushButton *pushButton_OK_;
    QPushButton *pushButton_Quit_;

};

#endif // VIDEODIALOG_H
