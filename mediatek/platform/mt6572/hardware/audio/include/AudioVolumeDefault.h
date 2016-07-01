#ifndef AUDIO_VOLUME_DEFAULT_H
#define AUDIO_VOLUME_DEFAULT_H

#define CUSTOM_VOLUME_STEP (7)
#define AUDIO_MAX_VOLUME_STEP (15)

enum VolumeType {
    NEW_VOL_TYPE_RING = 0,
    NEW_VOL_TYPE_SIP,
    NEW_VOL_TYPE_MIC,
    NEW_VOL_TYPE_FM,
    NEW_VOL_TYPE_SPH,
    NEW_VOL_TYPE_SPH2,
    NEW_VOL_TYPE_SID,
    NEW_VOL_TYPE_MEDIA,
    NEW_VOL_TYPE_MATV,
    NEW_NUM_OF_VOL_TYPE
};

enum VolumeMode {
    VOLUME_NORMAL_MODE = 0,
    VOLUME_HEADSET_MODE,
    VOLUME_SPEAKER_MODE,
    VOLUME_HEADSET_SPEAKER_MODE,
    NUM_OF_VOL_MODE
};

enum VOLUME_NORMAL_TYPE {
    NORMAL_AUDIO_BUFFER = 0,
    NORMAL_FM_RECORD_A,
    NORMAL_FM_RECORD_D,
    NORMAL_RSERVED_1,
    NORMAL_RSERVED_2,
    NORMAL_RSERVED_3,
    NORMAL_VOLUME_TYPE_MAX
};

enum VOLUME_HEADSET_TYPE {
    HEADSET_AUDIO_BUFFER = 0,
    HEADSET_FM_RECORD_A,
    HEADSET_FM_RECORD_D,
    HEADSET_RSERVED_1,
    HEADSET_RSERVED_2,
    HEADSET_RSERVED_3,
    HEADSET_VOLUME_TYPE_MAX
};

enum VOLUME_SPEAKER_TYPE {
    SPEAKER_AMP = 0,
    SPEAKER_FM_RECORD_A,
    SPEAKER_FM_RECORD_D,
    SPEAKER_RSERVED_1,
    SPEAKER_RSERVED_2,
    SPEAKER_RSERVED_3,
    SPEAKER_VOLUME_TYPE_MAX
};

enum VOLUME_HEADSET_SPEAKER_TYPE {
    HEADSET_SPEAKER_AUDIO_BUFFER = 0,
    HEADSET_SPEAKER_AMP,
    HEADSET_SPEAKER_IV_BUFFER,
    HEADSET_SPEAKER_FM_RECORD_A,
    HEADSET_SPEAKER_FM_RECORD_D,
    HEADSET_SPEAKER_RSERVED_1,
    HEADSET_SPEAKER_RSERVED_2,
    HEADSET_SPEAKER_RSERVED_3,
    HEADSET_SPEAKER_VOLUME_TYPE_MAX
};

// use for external amp
enum VOLUME_EXTAMP_TYPE {
    VOLUME_IV_BUFFER_EXTAMP = 0,
    VOLUME_AUDIO_BUFFER_EXTAMP,
    VOLUME_EXTAMP, // control exp amp gain
    VOLUME_EXTAMP_RSERVED_1,
    VOLUME_EXTAMP_RSERVED_2,
    VOLUME_EXTAMP_RSERVED_3,
    EXTAMP_VOLUME_TYPE_MAX
};

#define AUD_VOLUME_RING \
    0,32,64,96,128,160,192,0,0,0,0,0,0,0,0,\
    0,32,64,96,128,132,144,0,0,0,0,0,0,0,0,\
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0,\
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0

#define AUD_VOLUME_SIP \
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0,\
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0,\
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0,\
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0

#define AUD_VOLUME_MIC \
    64,255,255,144,255,255,160,0,0,0,0,0,0,0,0,\
    255,192,192,160,255,192,172,0,0,0,0,0,0,0,0,\
    255,208,208,164,255,208,172,0,0,0,0,0,0,0,0,\
    255,208,208,164,255,208,172,0,0,0,0,0,0,0,0

#define AUD_VOLUME_FM \
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0,\
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0,\
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0,\
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0

#define AUD_VOLUME_SPH \
    52,64,76,88,100,112,124,0,0,0,0,0,0,0,0,\
    76,88,100,112,124,136,148,0,0,0,0,0,0,0,0,\
    40,52,64,76,88,100,112,0,0,0,0,0,0,0,0,\
    40,52,64,76,88,100,112,0,0,0,0,0,0,0,0

#define AUD_VOLUME_SID \
    0,0,16,192,0,0,0,0,0,0,0,0,0,0,0,\
    0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,\
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

#define AUD_VOLUME_MEDIA \
    100,148,148,148,128,148,148,0,0,0,0,0,0,0,0,\
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0,\
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0,\
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0

#define AUD_VOLUME_MATV \
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0,\
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0,\
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0,\
    0,43,85,128,171,213,255,0,0,0,0,0,0,0,0

#define AUD_NORMAL_VOLUME_DEFAULT \
    132,0,0,0,0,0

#define AUD_HEADSER_VOLUME_DEFAULT \
    132,0,0,0,0,0

#define AUD_SPEAKER_VOLUME_DEFAULT \
    132,0,0,0,0,0

#define AUD_HEADSETSPEAKER_VOLUME_DEFAULT \
    132,120,0,0,0,0,0,0

#define AUD_EXTAMP_VOLUME_DEFAULT \
    132,0,0,0,0,0

#define AUD_VOLUME_LEVEL_DEFAULT \
    7,7,7,7,7,7,7,7,7

typedef struct _ADAUDIO_CUSTOM_VOLUME_PARAM_STRUCT {
    unsigned char audiovolume_ring[NUM_OF_VOL_MODE][AUDIO_MAX_VOLUME_STEP];
    unsigned char audiovolume_sip[NUM_OF_VOL_MODE][AUDIO_MAX_VOLUME_STEP];
    unsigned char audiovolume_mic[NUM_OF_VOL_MODE][AUDIO_MAX_VOLUME_STEP];
    unsigned char audiovolume_fm[NUM_OF_VOL_MODE][AUDIO_MAX_VOLUME_STEP];
    unsigned char audiovolume_sph[NUM_OF_VOL_MODE][AUDIO_MAX_VOLUME_STEP];
    unsigned char audiovolume_sph2[NUM_OF_VOL_MODE][AUDIO_MAX_VOLUME_STEP];
    unsigned char audiovolume_sid[NUM_OF_VOL_MODE][AUDIO_MAX_VOLUME_STEP];
    unsigned char audiovolume_media[NUM_OF_VOL_MODE][AUDIO_MAX_VOLUME_STEP];
    unsigned char audiovolume_matv[NUM_OF_VOL_MODE][AUDIO_MAX_VOLUME_STEP];

    unsigned char normalaudiovolume[NORMAL_VOLUME_TYPE_MAX];
    unsigned char headsetaudiovolume[HEADSET_VOLUME_TYPE_MAX];
    unsigned char speakeraudiovolume[SPEAKER_VOLUME_TYPE_MAX];
    unsigned char headsetspeakeraudiovolume[HEADSET_SPEAKER_VOLUME_TYPE_MAX];
    unsigned char extampaudiovolume[EXTAMP_VOLUME_TYPE_MAX];

    unsigned char audiovolume_level[NEW_NUM_OF_VOL_TYPE];
} ADAUDIO_CUSTOM_VOLUME_PARAM_STRUCT;

#define CFG_FILE_ADAUDIO_CUSTOM_REC_SIZE        sizeof(ADAUDIO_CUSTOM_VOLUME_PARAM_STRUCT)
#define CFG_FILE_ADAUDIO_CUSTOM_REC_TOTAL   1

static ADAUDIO_CUSTOM_VOLUME_PARAM_STRUCT ad_audio_custom_default = {
    AUD_VOLUME_RING,
    AUD_VOLUME_SIP,
    AUD_VOLUME_MIC,
    AUD_VOLUME_FM,
    AUD_VOLUME_SPH,
    AUD_VOLUME_SPH,
    AUD_VOLUME_SID,
    AUD_VOLUME_MEDIA,
    AUD_VOLUME_MATV,
    AUD_NORMAL_VOLUME_DEFAULT,
    AUD_HEADSER_VOLUME_DEFAULT,
    AUD_SPEAKER_VOLUME_DEFAULT,
    AUD_HEADSETSPEAKER_VOLUME_DEFAULT,
    AUD_EXTAMP_VOLUME_DEFAULT,
    AUD_VOLUME_LEVEL_DEFAULT
};

#endif

