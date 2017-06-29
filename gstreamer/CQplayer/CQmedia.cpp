#include "CQmedia.h"
#include <gst/interfaces/xoverlay.h>

static gboolean bus_callback(GstBus *bus,GstMessage *msg,gpointer data)
{
   CQMedia *media=(CQMedia*)data;
   switch(GST_MESSAGE_TYPE(msg))
   {
   case GST_MESSAGE_EOS:
       printf("EOS\n");
       media->stop();
       break;
   case GST_MESSAGE_ERROR:
       printf("Error happened");
       media->stop();
       break;
   case GST_MESSAGE_TAG:
       GstTagList* tag_list;
       gst_message_parse_tag(msg,&tag_list);
       printf("tagList\n");
       gst_tag_list_free(tag_list);
       break;
   default:
       printf("Message:%d \n",GST_MESSAGE_TYPE(msg));
   }
   return true;
}
static gboolean time_Update(gpointer data)
{
    ((CQMedia*)data)->timeUpdate();
    return true;
}
static char* formatTime (guint64 time)
{
    if (time >= GST_SECOND * 60 * 60)
    {
        return g_strdup_printf ("%d:%02d:%02d",(int) (time / (GST_SECOND * 60 * 60)),(int) ((time / (GST_SECOND * 60)) % 60),(int) ((time / GST_SECOND) % 60));
    }
    else
    {
        return g_strdup_printf ("%d:%02d",(int) ((time / (GST_SECOND * 60)) % 60),(int) ((time / GST_SECOND) % 60));
    }
}
CQMedia::CQMedia(CQplayerGUI* player,int winID)
{
    //初始化CQMedia各个变量
        this->player=player;
        this->winID=winID;
        this->volume=1.0;
        this->player->ui->sliderVlm->setValue((int)this->volume*10);
        this->length=GST_CLOCK_TIME_NONE;
        this->position=GST_CLOCK_TIME_NONE;
        this->player->ui->sliderProgress->setMinimum(0);
        this->seeking=false;
        this->winID=(winID>0)? winID:0;
        this->URI="";
        GstElement* videosink;
        GstElement* audiosink;
        //初始化gstreamer
        if(gst_init_check(NULL,NULL,NULL)==false)
        {
            return;
        }
        this->loop=g_main_loop_new(NULL,FALSE);
        this->pipeline=gst_element_factory_make("playbin","player");
        videosink=gst_element_factory_make("ximagesink","videosink");
        audiosink=gst_element_factory_make("alsasink","audiosink");
        if(pipeline==NULL||videosink==NULL||audiosink==NULL||videosink==NULL)
        {
            printf("create failed");
        }
        g_object_set(G_OBJECT(this->pipeline),"audio-sink",audiosink,"video-sink",videosink,"volume",this->volume,NULL);
        //添加消息监听函数
        GstBus* bus=gst_pipeline_get_bus(GST_PIPELINE(pipeline));
        gst_bus_add_watch(bus,bus_callback,this);
        gst_element_set_state(pipeline,GST_STATE_READY);
        this->state=STOPPED;
        this->speed=0;
}
CQMedia::~CQMedia()
{
    if(this->getPlayingState()!=STOPPED)
    {
        this->stop();
    }
    gst_object_unref(GST_OBJECT(pipeline));
    gst_deinit();
}
int CQMedia::getPlayingState()
{
    return this->state;
}
int CQMedia::getPlayingSpeed()
{
    return this->speed;
}
double CQMedia::getVolume()
{
    return this->volume;
}
void CQMedia::timeUpdate()
{
    GstFormat fmt=GST_FORMAT_TIME;
    gchar* txt;

    //判断是否正在拖动影片
    if(this->seeking)
    {
        return;
    }
    if(!GST_CLOCK_TIME_IS_VALID(this->length))
    {
        if(gst_element_query_duration(this->pipeline,&fmt,&this->length))
        {
            this->player->ui->sliderProgress->setMaximum(this->length/GST_SECOND);
        }
        else
        {
            return;
        }
    }
    if(!gst_element_query_position(this->pipeline,&fmt,&this->position))
    {
        return;
    }
    if(GST_CLOCK_TIME_IS_VALID(this->length))
    {
        gchar* tempP=formatTime(this->position);
        gchar* tempL=formatTime(this->length);
        txt=g_strdup_printf("%s / %s",tempP,tempL);
        g_free(tempP);
        g_free(tempL);
    }
    else
    {
        txt=formatTime(this->position);
    }
    this->player->ui->labelTiming->setText(txt);
    this->player->ui->sliderProgress->setValue(this->position/GST_SECOND);
    g_free(txt);
}
bool CQMedia::pause()
{
    GstStateChangeReturn ret;

    if (PLAYING != getPlayingState())
    {
      return false;
    }

    ret = gst_element_set_state(this->pipeline, GST_STATE_PAUSED);

    if (GST_STATE_CHANGE_FAILURE != ret)
    {
      this->state = PAUSED;
      return true;
    }
    else
      return false;
}
bool CQMedia::play()
{
    GstStateChangeReturn ret;
    GstElement* videosink;
    //判断播放是否已开始
    if(this->getPlayingState()==PLAYING)
    {
        return true;
    }
    //判断是否有窗口用以播放
    if(this->winID<=0)
    {
        return false;
    }
    g_object_get(this->pipeline,"video-sink",&videosink,NULL);
    if(videosink==NULL)
    {
        return false;
    }
    if(GST_IS_X_OVERLAY(videosink))
    {
        gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(videosink),this->winID);
    }
    ret=gst_element_set_state(this->pipeline,GST_STATE_PLAYING);
    if(GST_STATE_CHANGE_FAILURE!=ret)
    {
        this->state=PLAYING;
        printf("Playing");
        g_timeout_add(500,time_Update,this);
        return true;
    }
    return false;
}
bool CQMedia::stop()
{
    GstStateChangeReturn ret;

    if (STOPPED == getPlayingState()) {
      return false;
    }

    ret = gst_element_set_state(this->pipeline, GST_STATE_READY);

    if (GST_STATE_CHANGE_FAILURE != ret) {
        this->state = STOPPED;
        this->speed = 0;
        g_main_loop_quit(this->loop);

        return true;
    }
    else
        return false;
}
void CQMedia::seek(int position)
{

}
void CQMedia::seekStart()
{
    this->seeking=true;
}
void CQMedia::seekStop()
{
    this->seeking=false;
    gint64 value=this->player->ui->sliderProgress->value();
    gst_element_seek(this->pipeline,1.0,GST_FORMAT_TIME,GST_SEEK_FLAG_FLUSH,GST_SEEK_TYPE_SET,value*GST_SECOND,GST_SEEK_TYPE_NONE,GST_CLOCK_TIME_NONE);
}
bool CQMedia::forward()
{
    gint64 value = this->player->ui->sliderProgress->value();

    if ( this->speed==-1) {
         this->speed = 1;
    } else {
         this->speed++;
    }
    if (PAUSED == getPlayingState()) {
         gst_element_set_state(this->pipeline, GST_STATE_PLAYING);
         this->state = PLAYING;
    }
    gst_element_seek (this->pipeline, this->speed, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                      GST_SEEK_TYPE_SET, value * GST_SECOND,
                      GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);

    return true;
}
bool CQMedia::backward()
{
    gint64 value = this->player->ui->sliderProgress->value();

    if (this->speed==1) {
         this->speed=-1;
    } else {
         this->speed--;
    }
    if (PAUSED == getPlayingState()) {
         gst_element_set_state(this->pipeline, GST_STATE_PLAYING);
         this->state = PLAYING;
    }
    gst_element_seek (this->pipeline, this->speed, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                      GST_SEEK_TYPE_SET, value * GST_SECOND,
                      GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);

    return true;
}
bool CQMedia::setFilePath(const std::string &URI)
{
    this->URI=URI;
    g_object_set(G_OBJECT(this->pipeline), "uri", this->URI.c_str(), NULL);
    return true;
}
void CQMedia::setVolume(double volume)
{
    this->volume=volume/10;
    g_object_set(this->pipeline,"volume",this->volume,NULL);
}
