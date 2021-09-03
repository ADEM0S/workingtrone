#define VIDEO_PATH_LENGTH 50
#define VIDEO_TYPE_LENGTH 20

typedef struct VIDEO_STRUCT
{
  char type[VIDEO_TYPE_LENGTH];
  char path[VIDEO_PATH_LENGTH];
  int has_audio;
  int is_hd;
} Video;
