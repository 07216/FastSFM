#ifndef READDIALOG_H
#define READDIALOG_H

#include <QDialog>
#include <QPushButton>

enum READTYPE
{
    NOTSELECTED,
    READIMAGES,
    READFOLDER,
    READVIDEO
};

namespace Ui {
class ReadDialog;
}

class ReadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReadDialog(QWidget *parent = 0);
    ~ReadDialog();

    READTYPE getType()
    {
        return read_type_;
    }

private:
    void createButton();
    void createLayout();

private slots:
    void setTypeImages();
    void setTypeFolder();
    void setTypeVideo();
    void setTypeNone();

private:
    Ui::ReadDialog *ui;

    QPushButton *pushButton_readImages_;
    QPushButton *pushButton_readFolder_;
    QPushButton *pushButton_readVideo_;
    QPushButton *pushButton_quit_;

    READTYPE    read_type_;
};

#endif // READDIALOG_H
