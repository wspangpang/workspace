#ifndef CQPLAYERGUI_H
#define CQPLAYERGUI_H

#include <QDialog>
#include <CQmedia.h>
#include <iostream>
#include <QString>
#include <ui_CQplayerGUI.h>
#include <QFileDialog>

class CQMedia;

class CQplayerGUI : public QDialog
{
    Q_OBJECT

public:
    explicit CQplayerGUI(QWidget *parent = 0);
    ~CQplayerGUI();

public:
    Ui::CQplayerGUI* ui;
    CQMedia *media;
    QString URI;
private slots:
    void on_openFileButton_clicked();
    void on_sliderVlm_sliderMoved(int position);
    void on_sliderProgress_sliderReleased();
    void on_sliderProgress_sliderPressed();
    void on_backwardButton_clicked();
    void on_forwardButton_clicked();
    void on_pauseButton_clicked();
    void on_stopButton_clicked();
    void on_playButton_clicked();
};

#endif // CQPLAYERGUI_H
