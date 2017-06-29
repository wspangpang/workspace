#ifndef CQMEDIA_H
#define CQMEDIA_H

#include <iostream>
#include <gstreamer-0.10/gst/gst.h>
#include <CQplayerGUI.h>

class CQplayerGUI;

enum PlayingState
{
    PLAYING,
    STOPPED,
    PAUSED,
};

class CQMedia
{
public:
    CQMedia(CQplayerGUI* player,int windowID=0);
    ~CQMedia();
    bool setFilePath(const std::string& URI);
    bool play();
    bool pause();
    bool stop();
    bool forward();
    bool backward();
    int getPlayingState();
    int getPlayingSpeed();
    void setVolume(double volume);
    double getVolume();
    void timeUpdate();
    void seekStart();
    void seekStop();
    void seek(int position);
public:
    GstElement* pipeline;
private:
    GMainLoop* loop;
    PlayingState state;
    int speed;
    int winID;
    double volume;
    std::string URI;
    CQplayerGUI* player;
    gint64 length, position;
    bool seeking;
};

#endif // CQMEDIA_H
