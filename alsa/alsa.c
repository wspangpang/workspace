#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sched.h>
#include <errno.h>
#include <getopt.h>
#include <sys/time.h>
#include <alsa/asoundlib.h>

static snd_pcm_t* open_alsa_device(const char* dev_name,
                                    unsigned rate,
                                    unsigned channels,
                                    bool playback)
{
    printf("open_alsa_device %s,%d,%d,%d\n", dev_name, rate, channels, playback);
    snd_pcm_hw_params_t *hwparams = 0;
    snd_pcm_t* handle = 0;
    int err = 0;
    snd_pcm_stream_t stream = playback ? SND_PCM_STREAM_PLAYBACK : SND_PCM_STREAM_CAPTURE;
    if((err = snd_pcm_open(&handle, dev_name, stream, 0)) < 0)
    {
        printf("%s snd_pcm_open record err: %s\n", dev_name, snd_strerror(err));
        return 0;
    }

    /*snd_pcm_nonblock(handle, (playback ? 1 : 0));*/
    snd_pcm_nonblock(handle, 0);

    snd_pcm_hw_params_alloca(&hwparams);
    snd_pcm_hw_params_any(handle, hwparams);
    snd_pcm_hw_params_set_access(handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, hwparams, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(handle, hwparams, channels);

    int dir = 0;

    if((err = snd_pcm_hw_params_set_rate_near(handle, hwparams,  &rate, &dir)) < 0)
    {
        printf("%s set rate %d failed = %s\n", dev_name, rate, snd_strerror(err));
    }

    {
        snd_pcm_uframes_t frames = rate / 50;
        snd_pcm_uframes_t val;
        snd_pcm_hw_params_set_period_size_near(handle, hwparams, &frames, &dir);
        frames = rate * 2;
        err = snd_pcm_hw_params_get_buffer_size_max(hwparams, &val);
        if (err == 0) {
            if (val < (rate * 2)) {
               frames = val;
            }
        }
        if ((err = snd_pcm_hw_params_set_buffer_size_near(handle, hwparams, &frames)) < 0) {
            printf("%s set buffer size near %d failed = %s\n", dev_name, (int)frames, snd_strerror(err));
        }
    }

    if((err = snd_pcm_hw_params(handle, hwparams)) < 0)
    {
        printf("%s snd_pcm_hw_params err: %s\n", dev_name, snd_strerror(err));
        return 0;
    }

    if (playback) {
        do {
            snd_pcm_sw_params_t *swparams;
            snd_pcm_uframes_t frames = rate / 50;
            snd_pcm_sframes_t bufferSize;

            snd_pcm_sw_params_alloca(&swparams);
            #if 0
            err = snd_pcm_hw_params_get_buffer_size(hwparams, &frames);
            if (err != 0) {
                printf("%s [for playbac] get buffer size failed = %s\n", dev_name, snd_strerror(err));
                break;
            }
            #endif
            bufferSize = frames;
            printf("%s [for playbac] buffer size = %d\n", dev_name, (int)frames);

            err = snd_pcm_sw_params_current(handle, swparams);
            if (err < 0) {
                printf("%s [for playbac] determine current swparams failed = %s\n", dev_name, snd_strerror(err));
                break;
            }

            err = snd_pcm_sw_params_set_start_threshold(handle, swparams, bufferSize * 120 / 100);
//          err = snd_pcm_sw_params_set_start_threshold(handle, swparams, (bufferSize) * 1 / 10);
            if (err < 0) {
                printf("%s [for playbac] set start threshold %d failed = %s\n", dev_name, (int)bufferSize / 4, snd_strerror(err));
                break;
            }

            err = snd_pcm_sw_params(handle, swparams);
            if (err < 0) {
                printf("%s [for playbac] set sw params failed = %s\n", dev_name, snd_strerror(err));
                break;
            }
        } while (false);
    }

    if ((err = snd_pcm_prepare (handle)) < 0) {
        printf("%s snd_pcm_prepare err: %s\n", dev_name, snd_strerror(err));
        return 0;
    }
    return handle;
}


int main()
{
    FILE* fp = fopen("./pcm_tst.pcm", "w+");
    if (fp == NULL)
    {
        perror("open file failed.");
        return 0;
    }

    int err = 0;
    snd_pcm_t* mic_handle = NULL;
    snd_pcm_t* spk_handle = NULL;
    snd_pcm_t* pcmin_handle = NULL;
    snd_pcm_t* pcmout_handle = NULL;
    printf("start test alsa mic.\n");

    mic_handle = open_alsa_device("hw:0,0", 8000, 1, false);
    if (mic_handle != 0) 
    {
        printf("open mic alsa success.\n");
    }
    else 
    {
        printf("open mic alsa failed.\n");
        return 0;
    }

    spk_handle = open_alsa_device("hw:0,0", 8000, 1, true);
    if (spk_handle != 0) 
    {
        printf("open spk alsa success.\n");
    }
    else 
    {
        printf("open spk alsa failed.\n");
        return 0;
    }


    char buf[256];

    snd_pcm_state_t state = snd_pcm_state(mic_handle);
    printf("mic start currnet state %d \n", state);
    snd_pcm_start(mic_handle);
    state = snd_pcm_state(mic_handle);
    printf("mic after call start, current state is %d\n", state);

    state = snd_pcm_state(spk_handle);
    printf("spk start currnet state %d \n", state);
    snd_pcm_start(spk_handle);
    state = snd_pcm_state(spk_handle);
    printf("spk after call start, current state is %d\n", state);

    int count = snd_pcm_poll_descriptors_count(mic_handle);
    if (count <= 0)
    {
        printf("invalid poll descriptors count.\n");
        return 0;
    }

    printf("poll count is %d\n", count);

    struct pollfd* ufds = malloc(count * sizeof(struct pollfd));

    err = snd_pcm_poll_descriptors(mic_handle, ufds, count);
    if (err < 0)
    {
        printf("unable to obtain poll descriptor for capture: %s\n", snd_strerror(err));
        return 0;
    }

    count = snd_pcm_poll_descriptors_count(spk_handle);
    if (count <= 0)
    {
        printf("invalid spk poll descriptors count.\n");
        return 0;
    }

    printf("poll spk count is %d\n", count);

    struct pollfd* ufds_spk = malloc(count * sizeof(struct pollfd));

    err = snd_pcm_poll_descriptors(spk_handle, ufds_spk, count);
    if (err < 0)
    {
        printf("unable to obtain spk poll descriptor for capture: %s\n", snd_strerror(err));
        return 0;
    }

    printf("start read and write.... \n");

    int i = 0;
    for (; i < 10000; i++)
    {
        unsigned short revents;
        {
            poll(ufds, count, -1);
            snd_pcm_poll_descriptors_revents(mic_handle, ufds, count, &revents);
            if (revents & POLLERR)
            {
                snd_pcm_state_t state = snd_pcm_state(mic_handle);
                printf("poll catch error.... with error %d\n", state);
                return 0;
            }

            if (revents & POLLIN)
            {
                snd_pcm_sframes_t f_count = snd_pcm_avail_update(mic_handle);
                snd_pcm_sframes_t r_count = snd_pcm_readi(mic_handle, buf, 128);
                /*printf("read frames f count %d, r count %d\n", f_count, r_count);*/
                fwrite(buf, 1, r_count * 2, fp);
            }
        }

        unsigned short revents_spk;
        {
            poll(ufds_spk, count, -1);
            snd_pcm_poll_descriptors_revents(spk_handle, ufds_spk, count, &revents_spk);
            if (revents_spk & POLLERR)
            {
                snd_pcm_state_t state = snd_pcm_state(spk_handle);
                printf("poll spk catch error.... with error %d\n", state);
                return 0;
            }

            if (revents_spk & POLLOUT)
            {
                snd_pcm_sframes_t f_count = snd_pcm_avail_update(spk_handle);
                snd_pcm_sframes_t r_count = snd_pcm_writei(spk_handle, buf, 128);
                printf("write frames f count %d, r count %d\n", f_count, r_count);
            }
        }
    }

    snd_pcm_drain(mic_handle);
    snd_pcm_drain(spk_handle);

    snd_pcm_close(mic_handle);
    snd_pcm_close(spk_handle);
    return 0;
}
