#include "CQplayerGUI.h"
#include "ui_CQplayerGUI.h"
#include <iostream>

CQplayerGUI::CQplayerGUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CQplayerGUI)
{
    ui->setupUi(this);
    media=new CQMedia(this,this->ui->frameMovie->winId());
}

CQplayerGUI::~CQplayerGUI()
{
    delete ui;
    delete media;
}

void CQplayerGUI::on_playButton_clicked()
{
    if(this->URI.isEmpty())
    {
        this->URI=QFileDialog::getOpenFileName(this,"open file dialog","/","ALL MOVIE FILES(*.avi *.rm *.mp4);;AVI files(*.avi);;RM files(*.rm);;MP4 files(*.mp4)");
    }

    QString path=this->URI;
    char buf[128];
    //判断path为URI，还是本地路径
    if(path.startsWith("/"))
    {
        path="file://"+path;
    }
    //当前目录下
    else if(path.indexOf(":///")==-1)
    {
        getcwd(buf,128);
        strcat(buf,"/");
        path=QString("file://")+buf+path;
    }
    if(media->getPlayingState()==STOPPED)
    {
        media->setFilePath(path.toStdString());
    }
    this->media->play();
}

void CQplayerGUI::on_stopButton_clicked()
{
    this->media->stop();
}

void CQplayerGUI::on_pauseButton_clicked()
{
    this->media->pause();
}

void CQplayerGUI::on_forwardButton_clicked()
{
    this->media->forward();
}

void CQplayerGUI::on_backwardButton_clicked()
{
    this->media->backward();
}

void CQplayerGUI::on_sliderProgress_sliderMoved(int position)
{
    media->seek(position);
}

void CQplayerGUI::on_sliderProgress_sliderPressed()
{
    media->seekStart();
}

void CQplayerGUI::on_sliderProgress_sliderReleased()
{
    media->seekStop();
}

void CQplayerGUI::on_sliderVlm_sliderMoved(int volume)
{
    media->setVolume(volume);
}

void CQplayerGUI::on_openFileButton_clicked()
{
    this->URI=QFileDialog::getOpenFileName(this,"open file dialog","/","ALL MOVIE FILES(*.avi *.rm *.mp4);;AVI files(*.avi);;RM files(*.rm);;MP4 files(*.mp4)");
}
