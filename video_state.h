typedef struct VideoState {
    const char *filename;
    int v_index, a_index; // 音视频的流索引
    AVStream *video_st, *audio_st; // 音视频流

}VideoState;