#include <gst/gst.h>
#include <SDL.h>
#include <SDL_thread.h>
#include <gst/interfaces/xoverlay.h>
#include <SDL_syswm.h>

static gboolean
bus_callback(GstBus *bus,
             GstMessage *msg,
             gpointer data)
{
  GMainLoop *loop = data;

  switch (GST_MESSAGE_TYPE(msg)) {
  case GST_MESSAGE_EOS:
    g_print("End-of-stream\n");
    g_main_loop_quit(loop);
    break;
  case GST_MESSAGE_ERROR: 
    {
      gchar *debug;
      GError *err;

      gst_message_parse_error(msg, &err, &debug);
      g_free(debug);

      g_print("Error: %s\n", err->message);
      g_error_free(err);

      g_main_loop_quit(loop);
      break;
    }
  default:
    break;
  }

  return TRUE;
}

int func_loop(void *data)
{
  GMainLoop *loop = data;
  g_main_loop_run(loop);

  return 0;
}

gint
main(gint argc,
     gchar *argv[])
{
  GMainLoop *loop;
  GstElement *play;
  int quit = 0;
  SDL_Event event;
  int paused = 0;
  double volumeValue;
  GstElement *videosink, *audiosink;
  SDL_SysWMinfo info;
  
  

  /* init GStreamer */
  gst_init(&argc, &argv);

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    /* Failed, exit */
    fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
    return -1;
  }
  atexit(SDL_Quit);
  
  if (NULL == SDL_SetVideoMode(400, 300, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) {
    fprintf(stderr, "Set video mode failed: %s\n", SDL_GetError());
    return -1;
  }
  if (0 == SDL_GetWMInfo(&info)) {
    fprintf(stderr, "Get info error\n");
  }
#ifdef linux
  printf("window id: %d\n", info.info.x11.window);
#endif
  loop = g_main_loop_new(NULL, FALSE);

  /* make sure we have a URI */
  if (2 != argc) {
    g_print("Usage: %s <URI>\n", argv[0]);
    return -1;
  }

  /* set up */
  play = gst_element_factory_make("playbin", "play");
  videosink = gst_element_factory_make("ximagesink", "videosink");
  audiosink = gst_element_factory_make("alsasink", "audiosink");
  g_object_set(G_OBJECT(play), "audio-sink", audiosink, NULL);
  g_object_set(G_OBJECT(play), "video-sink", videosink, NULL);

  if (videosink && GST_IS_X_OVERLAY(videosink)) {
#ifdef linux
    gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(videosink), info.info.x11.window);
#endif
  } else {
    fprintf(stderr, "Not x overlay\n");
  }
  
  
  g_object_set(G_OBJECT(play), "uri", argv[1], NULL);
  gst_bus_add_watch(gst_pipeline_get_bus(GST_PIPELINE(play)), bus_callback, loop);
  gst_element_set_state(play, GST_STATE_PLAYING);

  /* now run */
  //g_main_loop_run(loop);
  //SDL_CreateThread(func_loop, loop);
  
  while (0 == quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        quit = 1;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_p:
          if (1 == paused) {
            gst_element_set_state(play, GST_STATE_PLAYING);
          } else {
            gst_element_set_state(play, GST_STATE_PAUSED);
          }
          paused ^= 1;
          break;
        case SDLK_LEFT:
          g_object_get(play, "volume", &volumeValue, NULL);
          g_object_set(play, "volume", (volumeValue - 1 > 0) ? volumeValue - 1 : 0, NULL);
          break;
        case SDLK_RIGHT:
          g_object_get(play, "volume", &volumeValue, NULL);
          g_object_set(play, "volume", (volumeValue + 1 < 4) ? volumeValue + 1 : 4, NULL);
          break;
        default:
          break;
        } // End switch (event.key.keysym.sym)
        break;
      default:
        break;
      } // switch (event.type) 
    } // End (SDL_PollEvent(&event))
    SDL_Delay(5);
  } // End (1 != quit) 
  
   /* also clean up */
  gst_element_set_state(play, GST_STATE_NULL);
  gst_object_unref(GST_OBJECT(play));

  return 0;
}