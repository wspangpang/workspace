#include <gst/gst.h>
#include <string.h>
void error_quit(GstBus *bus,GstMessage *message,GMainLoop *loop)
{
    GError *error;
    gst_message_parse_error(message,&error,NULL);
    printf("Error:%s\n",error->message);
    g_main_loop_quit(loop);
}
void end_of_streamer(GstBus *bus,GstMessage *message,GMainLoop *loop)
{
    printf("End Of Streamer. . .\n");
    g_main_loop_quit(loop);
}
void print_pos(GstElement *pipe)
{
    GstFormat fm=GST_FORMAT_TIME;
    gint64 pos,len;
    gst_element_query_position(pipe,&fm,&pos);
    gst_element_query_duration(pipe,&fm,&len);
    printf("Time:%u:%02u:%02u/%u:%02u:%02u:%02u\n",GST_TIME_ARGS(pos),GST_TIME_ARGS(len));
}
int main(int argc,char **argv)
{
    GMainLoop *loop;
    GstElement *playpipe;
    //GstElement *source;
    GstBus *bus;
    //gchar *file_name;
    gst_init(&argc,&argv);
    loop=g_main_loop_new(NULL,FALSE);
    playpipe=gst_element_factory_make("playbin","play-source");
    //playpipe=gst_pipeline_new("play-pipe");
    //gst_bin_add(GST_BIN(playpipe),source);
    //gst_element_link(playpipe,source);
    //g_object_set(G_OBJECT(playpipe),"location",argv[1],NULL);
    //file_name=malloc(sizeof(gchar)*7+strlen(argv[1])+1);
    //strcpy(file_name,"file://");
    //strcat(file_name,argv[1]);
    g_object_set(G_OBJECT(playpipe),"uri",argv[1],NULL);
    bus=gst_pipeline_get_bus(GST_PIPELINE(playpipe));
    gst_bus_add_signal_watch(bus);
    g_signal_connect(G_OBJECT(bus),"message::error",G_CALLBACK(error_quit),loop);
    g_signal_connect(G_OBJECT(bus),"message::eos",G_CALLBACK(end_of_streamer),loop);
    printf("Start. . .\n");
    gst_element_set_state(playpipe,GST_STATE_PLAYING);
    g_timeout_add(1000,(void *)print_pos,playpipe);
    g_main_loop_run(loop);
    gst_element_set_state(playpipe,GST_STATE_NULL);
    return 0;
}
