#ifndef UI_CQPLAYERGUI_H
#define UI_CQPLAYERGUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CQplayerGUI
{
public:
    QFrame *frameMovie;
    QSlider *sliderProgress;
    QLabel *labelTiming;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QPushButton *forwardButton;
    QPushButton *backwardButton;
    QPushButton *openFileButton;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_8;
    QLabel *labelVolume;
    QSlider *sliderVlm;

    void setupUi(QDialog *CQplayerGUI)
    {
        if (CQplayerGUI->objectName().isEmpty())
            CQplayerGUI->setObjectName(QString::fromUtf8("CQplayerGUI"));
        CQplayerGUI->resize(504, 358);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CQplayerGUI->sizePolicy().hasHeightForWidth());
        CQplayerGUI->setSizePolicy(sizePolicy);
        CQplayerGUI->setFocusPolicy(Qt::NoFocus);
        frameMovie = new QFrame(CQplayerGUI);
        frameMovie->setObjectName(QString::fromUtf8("frameMovie"));
        frameMovie->setGeometry(QRect(0, 0, 504, 261));
        frameMovie->setFrameShape(QFrame::StyledPanel);
        frameMovie->setFrameShadow(QFrame::Raised);
        sliderProgress = new QSlider(CQplayerGUI);
        sliderProgress->setObjectName(QString::fromUtf8("sliderProgress"));
        sliderProgress->setGeometry(QRect(10, 270, 371, 29));
        sliderProgress->setOrientation(Qt::Horizontal);
        labelTiming = new QLabel(CQplayerGUI);
        labelTiming->setObjectName(QString::fromUtf8("labelTiming"));
        labelTiming->setGeometry(QRect(390, 270, 111, 29));
        horizontalLayoutWidget = new QWidget(CQplayerGUI);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 310, 321, 41));
        horizontalLayout_7 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        playButton = new QPushButton(horizontalLayoutWidget);
        playButton->setObjectName(QString::fromUtf8("playButton"));
        playButton->setFocusPolicy(Qt::StrongFocus);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        playButton->setIcon(icon);

        horizontalLayout_7->addWidget(playButton);

        pauseButton = new QPushButton(horizontalLayoutWidget);
        pauseButton->setObjectName(QString::fromUtf8("pauseButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        pauseButton->setIcon(icon1);

        horizontalLayout_7->addWidget(pauseButton);

        stopButton = new QPushButton(horizontalLayoutWidget);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        stopButton->setIcon(icon2);

        horizontalLayout_7->addWidget(stopButton);

        forwardButton = new QPushButton(horizontalLayoutWidget);
        forwardButton->setObjectName(QString::fromUtf8("forwardButton"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/step.png"), QSize(), QIcon::Normal, QIcon::Off);
        forwardButton->setIcon(icon3);

        horizontalLayout_7->addWidget(forwardButton);

        backwardButton = new QPushButton(horizontalLayoutWidget);
        backwardButton->setObjectName(QString::fromUtf8("backwardButton"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/backward.png"), QSize(), QIcon::Normal, QIcon::Off);
        backwardButton->setIcon(icon4);

        horizontalLayout_7->addWidget(backwardButton);

        openFileButton = new QPushButton(horizontalLayoutWidget);
        openFileButton->setObjectName(QString::fromUtf8("openFileButton"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/openfile.png"), QSize(), QIcon::Normal, QIcon::Off);
        openFileButton->setIcon(icon5);

        horizontalLayout_7->addWidget(openFileButton);

        horizontalLayoutWidget_2 = new QWidget(CQplayerGUI);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(350, 310, 141, 41));
        horizontalLayout_8 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        labelVolume = new QLabel(horizontalLayoutWidget_2);
        labelVolume->setObjectName(QString::fromUtf8("labelVolume"));

        horizontalLayout_8->addWidget(labelVolume);

        sliderVlm = new QSlider(horizontalLayoutWidget_2);
        sliderVlm->setObjectName(QString::fromUtf8("sliderVlm"));
        sliderVlm->setMaximum(100);
        sliderVlm->setOrientation(Qt::Horizontal);

        horizontalLayout_8->addWidget(sliderVlm);


        retranslateUi(CQplayerGUI);

        QMetaObject::connectSlotsByName(CQplayerGUI);
    } // setupUi

    void retranslateUi(QDialog *CQplayerGUI)
    {
        CQplayerGUI->setWindowTitle(QApplication::translate("CQplayerGUI", "CQplayerGUI", 0, QApplication::UnicodeUTF8));
        labelTiming->setText(QApplication::translate("CQplayerGUI", "00:00 / 00:00", 0, QApplication::UnicodeUTF8));
        playButton->setText(QString());
        pauseButton->setText(QString());
        stopButton->setText(QString());
        forwardButton->setText(QString());
        backwardButton->setText(QString());
        openFileButton->setText(QString());
        labelVolume->setText(QApplication::translate("CQplayerGUI", "volume", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CQplayerGUI: public Ui_CQplayerGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CQPLAYERGUI_H
