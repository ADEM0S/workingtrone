#define MAIN_WIN_NAME "Main Window"

#define VIDEO_FORMAT_IN "(%[^,], %[^,], %d, %d)\n"
#define LEN_FORMAT_IN "length : %d\n"
#define VIDEO_STRUCTS_PATH "assets/video_structs.dat"

#define VIDEO_MS_HD_PROC_OFFSET 18 // average of 18 ms of processing


int get_structs_length(FILE *fptr)
{
	int length = 0;
	fscanf(fptr, LEN_FORMAT_IN, &length);
	return length;
}

void fill_dynarray_from_file(FILE *fptr, DynArray *videos_list, int length)
{
	Video temp;
	for (int i = 0; i < length; i++)
	{
		fscanf(fptr, VIDEO_FORMAT_IN, temp.type, temp.path, &temp.has_audio, &temp.is_hd);
		insert_array(videos_list, temp);
	}
}

int read_structs(DynArray *videos_list)
{
	FILE *fptr = fopen(VIDEO_STRUCTS_PATH, "r");

	if (fptr == NULL) return 0;

	int length = get_structs_length(fptr);

	fill_dynarray_from_file(fptr, videos_list, length);

	fclose(fptr);
	return length;
}

void fill_videos_list_by_type(DynArray *good_type_list, DynArray *videos_list, char *type)
{
	if (videos_list->array == NULL) return;
	for (int i = 0; i < videos_list->used; i++)
	{
		if (strcmp(videos_list->array[i].type, type) == 0)
		{
			insert_array(good_type_list, videos_list->array[i]);
		}
	}
}

Video get_random_video(DynArray *videos_list, char *type)
{
	time_t t;
	srand((unsigned) time(&t));

	DynArray g_type;
	init_array(&g_type, 1);

	fill_videos_list_by_type(&g_type, videos_list, type);
	Video rand_vid = g_type.array[rand() % g_type.used];
	free_array(&g_type);
	return rand_vid;
}


float get_mtts(Video *video, VideoCapture cap, int playtime)
{
	if (playtime)
		return (float)(playtime * 1000.0) / (float)cap.get(CAP_PROP_FRAME_COUNT);

	float mtts = (1000.0) / (float)cap.get(CAP_PROP_FPS); // T = 1 / f

	if (video->is_hd)
		mtts -= VIDEO_MS_HD_PROC_OFFSET; // Processing time for hd videos is very high so we need an offset

	return mtts;
}


int play_video(Video *video, int playtime)
{
	if (video->has_audio)
	{
		if (launch_audio(video) != 0)
		{
			printf("Couldn't launch audio.\n");
			return -1;
		}
	}

	VideoCapture cap(video->path);
	if(!cap.isOpened())
	{
		cap.release();
		return -1;
	}

	float mtts = get_mtts(video, cap, playtime);

	Mat frame;
	char c;
	while (1)
	{
		cap >> frame;

		if (frame.empty()) break;

		imshow(MAIN_WIN_NAME, frame);

		c = (char) waitKey(mtts);
		if (c == 27) break; // CHANGE 27 (esc) TO SOME CONSTANT IN ANOTHER HEADER FILE
	}

	frame.release();
	cap.release();

	return 1;
}

int play_random_video(int playtime, DynArray *videos_list, char *type)
{
	Video vid_to_play = get_random_video(videos_list, type);
	return play_video(&vid_to_play, playtime);
}

Video *get_main_loop_video(DynArray *vids_list_ptr)
{
	for (int i = 0; i < vids_list_ptr->used; i++)
	{
		if ( strcmp(vids_list_ptr->array[i].type, "mainloop") == 0)
			return &vids_list_ptr->array[i];
	}
	return NULL;
}

Video* get_video_by_path(DynArray *videos, const char * path)
{

	for (int i = 0; i < videos->used; i++)
	{
		if ( strcmp(videos->array[i].path, path) == 0)
			return &videos->array[i];
	}

	return NULL;
}
