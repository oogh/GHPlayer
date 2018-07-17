#include <libavformat/avformat.h>
#include "video_state.h"
#include "user_config.h"

/**
 * 打开流，获取流索引
 * @param filename 输入文件
 * @return 0: 成功, <0 失败
 */
int stream_open(const char *filename) {
    int ret = 0;
    AVInputFormat *iformat = NULL; // 解复用器
    int v_index, a_index;

    av_register_all();
    avformat_network_init();

    VideoState *is = av_mallocz(sizeof(VideoState));
    if (!is) {
        ret = AVERROR(ENOMEM);
        goto fail;
    }

    is->filename = av_strdup(filename);
    if (!is->filename) {
        av_log(NULL, AV_LOG_FATAL, "Filename is NULL");
        ret = -1;
        goto fail;
    }

    AVFormatContext *ic = avformat_alloc_context();
    if (!ic) {
        ret =  AVERROR(ENOMEM);
        goto fail;
    }

    int err;
    err = avformat_open_input(&ic, is->filename, iformat, 0);
    if (err < 0) {
        char buf[1024];
        av_strerror(err, buf, sizeof(buf));
        av_log(NULL, AV_LOG_FATAL, "FAILED avformat_open_input(filename): %s", buf);
        goto fail;
    }

    err = avformat_find_stream_info(ic, 0);
    if (err < 0) {
        char buf[1024];
        av_strerror(err, buf, sizeof(buf));
        av_log(NULL, AV_LOG_FATAL, "FAILED avformat_find_stream_info(ic): %s", buf);
        ret = -1;
        goto fail;
    }

    if (SHOW_STATUS) {
        av_dump_format(ic, 0, is->filename, 0);
    }

    if(!DISABLE_VIDEO)
        v_index = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (!DISABLE_AUDIO)
        a_index = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

    if (v_index >= 0) {
        is->a_index = a_index;
        is->video_st = ic->streams[v_index];
    }

    if (v_index >= 0) {
        is->a_index = a_index;
        is->audio_st = ic->streams[a_index];
    }

    if (v_index < 0 || a_index < 0) {
        av_log(NULL, AV_LOG_FATAL, "FAILED av_find_best_stream(AVMEDIA_TYPE VIDEO/AUDIO)");
        ret = -1;
        goto fail;
    }

    av_log(NULL, AV_LOG_WARNING, "打开流成功， indexV=%d, indexA=%d", is->v_index, is->a_index);

    fail:
    if (ic) {
        avformat_close_input(&ic);
    }

    return ret;
}