#include "videodialog.h"
#include "ui_videodialog.h"
#include <QMediaPlaylist>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <sstream>
#include <QLineEdit>

VideoDialog::VideoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoDialog)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Playing video"));
    this->setFixedSize(QSize(900, 600));

    createLineEdit();
    createSelectFramesWidgets();
    createPlayer();
    createLayout();

    updateSelectedFrames();

    position_ = 0;
}

VideoDialog::~VideoDialog()
{
    delete ui;
}

bool VideoDialog::getSelectedFrames(std::vector<qint64> &frames, int &step)
{
    if (selected_frames_.size()%2 == 1)
    {
        return false;
    }
    for (int i = 0; i < selected_frames_.size(); ++i)
    {
        frames.push_back(selected_frames_.at(i));
    }

    if (checkBox_auto_step_->isChecked())
    {
        step = -1;
    }
    else
    {
        step = spinBox_frame_sample_step_->value();
    }
    return true;
}

void VideoDialog::createPlayer()
{
    playlist_ = new QMediaPlaylist;

    player_ = new QMediaPlayer;
    player_->setPlaylist(playlist_);

    videoWidget_ = new QVideoWidget;
    player_->setVideoOutput(videoWidget_);
    player_->setNotifyInterval(100);

    connect(player_,SIGNAL(positionChanged(qint64)), this, SLOT(setSliderPosition(qint64)));

    slider_ = new QSlider;
    slider_->setOrientation(Qt::Horizontal);
    slider_->setMinimum(0);
    slider_->setMaximum(SLIDER_MAXIMUM);
    connect(slider_, SIGNAL(sliderPressed()), this, SLOT(pause()));
    connect(slider_, SIGNAL(sliderMoved(int)), this, SLOT(updateCurrentPosition(int)));
    connect(slider_, SIGNAL(sliderReleased()), this, SLOT(setVideoPosition()));

    label_position_ = new QLabel;
}

void VideoDialog::setSliderPosition(qint64 pos)
{
    double p = (double)pos*SLIDER_MAXIMUM/player_->duration();
    slider_->setValue((int)p);
    slider_->update();
    updateLabel();
}

void VideoDialog::updateCurrentPosition(int pos)
{
    position_ = (double)pos*player_->duration()/SLIDER_MAXIMUM;
    updateLabel();
}

void VideoDialog::updateLabel()
{
    std::stringstream ss;
    ss<<(double)slider_->value()/SLIDER_MAXIMUM*100;
    std::string text = ss.str() + "%";
    label_position_->setText(text.data());
}

void VideoDialog::setVideoPosition()
{
    player_->setPosition((int)position_);
    play();
}

void VideoDialog::initVideoPath(QString path)
{
    playlist_->addMedia(QUrl::fromLocalFile(path));
    playlist_->setCurrentIndex(1);

    videoWidget_->show();
    pushButton_pause_->show();
    pushButton_play_->hide();

    player_->play();
}

void VideoDialog::createSelectFramesWidgets()
{
    spinBox_frame_sample_step_ = new QSpinBox;
    spinBox_frame_sample_step_->setMinimum(1);
    spinBox_frame_sample_step_->setMaximum(100);
    spinBox_frame_sample_step_->setSingleStep(1);
    spinBox_frame_sample_step_->setValue(1);
    spinBox_frame_sample_step_->setAlignment(Qt::AlignHCenter);

    checkBox_auto_step_ = new QCheckBox(tr("Auto-decide step"));
    checkBox_auto_step_->setChecked(false);

    pushButton_get_frame_ = new QPushButton(tr("Select frame"));
    connect(pushButton_get_frame_, SIGNAL(clicked()), this, SLOT(selectFrame()));

    widget_disp_selected_frames_ = new QWidget;
    layout_disp_ = new QGridLayout;
    QLabel *label_start_frame = new QLabel(tr("start"));
    label_start_frame->setAlignment(Qt::AlignHCenter);
    QLabel *label_end_frame = new QLabel(tr("end"));
    label_end_frame->setAlignment(Qt::AlignHCenter);
    layout_disp_->addWidget(label_start_frame, 0, 0);
    layout_disp_->addWidget(label_end_frame, 0, 1);
    widget_disp_selected_frames_->setLayout(layout_disp_);
    for (int i = 0; i < 2*MAX_CLIPS; ++i)
    {
        layout_disp_->addWidget(lineEdit_display_.at(i), i/2+1,i%2);
    }

    checkBox_first_frame_ = new QCheckBox;
    checkBox_first_frame_->setText(tr("First frame"));
    checkBox_first_frame_->setCheckable(true);
    checkBox_first_frame_->setChecked(false);
    checkBox_last_frame_ = new QCheckBox;
    checkBox_last_frame_->setText(tr("Last frame"));
    checkBox_last_frame_->setCheckable(true);
    checkBox_last_frame_->setChecked(false);

    connect(checkBox_first_frame_, SIGNAL(stateChanged(int)), this, SLOT(changeFirstFrameStatus(int)));
    connect(checkBox_last_frame_, SIGNAL(stateChanged(int)), this, SLOT(changeLastFrameStatus(int)));

    pushButton_OK_ = new QPushButton("OK");
    connect(pushButton_OK_, SIGNAL(clicked()), this, SLOT(accept()));

    pushButton_Quit_ = new QPushButton("Quit");
    connect(pushButton_Quit_, SIGNAL(clicked()), this, SLOT(reject()));

    layout_disp_->addWidget(pushButton_OK_, MAX_CLIPS+1, 0, 1, 2);
    layout_disp_->addWidget(pushButton_Quit_, MAX_CLIPS+2, 0, 1, 2);

}

void VideoDialog::changeFirstFrameStatus(int status)
{
    if (selected_frames_.empty())
    {
        selected_frames_.push_back(0);
        updateSelectedFrames();
        return;
    }
    std::sort(selected_frames_.begin(), selected_frames_.end());
    if (selected_frames_.front() != 0 && status != 0)
    {
        selected_frames_.insert(selected_frames_.begin(), 0);
    }
    else if (selected_frames_.front() == 0 && status == 0)
    {
        selected_frames_.erase(selected_frames_.begin());
    }
    updateSelectedFrames();
}

void VideoDialog::changeLastFrameStatus(int status)
{
    if (selected_frames_.empty())
    {
        selected_frames_.push_back(player_->duration()-1);
        updateSelectedFrames();
        return;
    }
    std::sort(selected_frames_.begin(), selected_frames_.end());
    if (selected_frames_.back() != player_->duration()-1 && status != 0)
    {
        selected_frames_.push_back(player_->duration()-1);
    }
    else if (selected_frames_.back() == player_->duration()-1 && status == 0)
    {
        selected_frames_.pop_back();
    }
    updateSelectedFrames();
}

void VideoDialog::createLineEdit()
{
    for (int i = 0; i < 2*MAX_CLIPS; ++i)
    {
        QLineEdit *lineEdit = new QLineEdit;
        lineEdit->setMinimumWidth(60);
        lineEdit->setReadOnly(true);
        lineEdit_display_.push_back(lineEdit);
    }
}

void VideoDialog::createLayout()
{
    QGridLayout *layout_main = new QGridLayout;
    this->setLayout(layout_main);
    layout_main->setRowStretch(0, 8);
    layout_main->setRowStretch(1, 1);
    layout_main->setColumnStretch(0, 10);
    layout_main->setColumnStretch(1, 1);

    layout_main->addWidget(videoWidget_, 0, 0);

    QHBoxLayout *layout_control = new QHBoxLayout;
    layout_control->setAlignment(Qt::AlignLeft);
    QWidget *widget_control = new QWidget;
    layout_main->addWidget(widget_control, 1, 0);
    widget_control->setLayout(layout_control);

    pushButton_pause_ = new QPushButton;
    pushButton_pause_->setIcon(QIcon(tr("../USFM/Icons/pause.png")));
    pushButton_pause_->setIconSize(QSize(30,30));
    connect(pushButton_pause_, SIGNAL(clicked()),this,SLOT(pause()));

    pushButton_play_ = new QPushButton();
    pushButton_play_->setIcon(QIcon(tr("../USFM/Icons/play.png")));
    pushButton_play_->setIconSize(QSize(30,30));
    connect(pushButton_play_, SIGNAL(clicked()),this,SLOT(play()));

    layout_control->addWidget(pushButton_play_);
    layout_control->addWidget(pushButton_pause_);
    layout_control->addWidget(slider_);
    layout_control->addWidget(label_position_);

    pushButton_play_->hide();

    QWidget *widget_select_frames = new QWidget;
    QVBoxLayout *layout_select_frames = new QVBoxLayout;
    layout_select_frames->setAlignment(Qt::AlignTop);
    layout_main->addWidget(widget_select_frames, 0, 1);
    widget_select_frames->setLayout(layout_select_frames);

    QLabel  *label_step = new QLabel(tr("Sample step"));
    label_step->setAlignment(Qt::AlignHCenter);
    layout_select_frames->addWidget(label_step);
    layout_select_frames->addWidget(spinBox_frame_sample_step_);
    layout_select_frames->addWidget(checkBox_auto_step_);
    layout_select_frames->addSpacing(20);
    layout_select_frames->addWidget(checkBox_first_frame_);
    layout_select_frames->addWidget(checkBox_last_frame_);
    layout_select_frames->addSpacing(20);
    layout_select_frames->addWidget(pushButton_get_frame_);
    layout_select_frames->addWidget(widget_disp_selected_frames_);

}

void VideoDialog::pause()
{
    player_->pause();
    pushButton_pause_->hide();
    pushButton_play_->show();
}

void VideoDialog::play()
{
    player_->play();
    pushButton_pause_->show();
    pushButton_play_->hide();
}

void VideoDialog::reset()
{
    player_->stop();
    selected_frames_.clear();
    position_ = 0;
    slider_->setValue(0);
    pushButton_pause_->show();
    pushButton_play_->hide();
    for (int i = 0; i < MAX_CLIPS; ++i)
    {
        lineEdit_display_.at(i)->setText("");
    }
}

void VideoDialog::selectFrame()
{
    if (selected_frames_.size() < 2*MAX_CLIPS)
    {
        selected_frames_.push_back(player_->position());
        updateSelectedFrames();
    }
}

void VideoDialog::updateSelectedFrames()
{
    sort(selected_frames_.begin(), selected_frames_.end());
    for (int i = 0; i < selected_frames_.size(); ++i)
    {
        std::stringstream ss;
        ss << selected_frames_.at(i);
        std::string s = ss.str();
        lineEdit_display_.at(i)->setText(s.data());
    }
    for (int i = selected_frames_.size(); i < 2*MAX_CLIPS; ++i)
    {
        lineEdit_display_.at(i)->setText("");
    }
}




