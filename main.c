#include <opencv4/opencv2/highgui/highgui.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <string>
#include <unistd.h>
#include <cerrno>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

using namespace std;
using namespace cv;

#include "include/constants.h"
#include "include/utils.h"
#include "include/video.h"
#include "include/pins.h"
#include "include/dynamic_array.h"
#include "include/audio_manipulation.h"
#include "include/video_manipulation.h"


// PINS
#define LED 1
#define BUZZER 2
#define WHITE_BUTTON 3
#define YELLOW_BUTTON 4
#define RED_BUTTON 5
#define BLUE_BUTTON 6
#define GREEN_BUTTON 7

#define INPUT 0
#define OUTPUT 1


uint8_t GIF_LENGTH = 5;


int main(int argc, char **argv)
{
  printf("\n===============================\nBEGINNING OF THE GIFFER PROGRAM\n===============================\n\n");


  int res = pin_open(LED);
  if (res != 0)
  {
    printf("Error pin_open the LED.\n");
    return EXIT_FAILURE;
  }
  pin_open(BUZZER);
  pin_open(WHITE_BUTTON);
  pin_open(YELLOW_BUTTON);
  pin_open(RED_BUTTON);
  pin_open(BLUE_BUTTON);
  pin_open(GREEN_BUTTON);

  pinMode(LED, OUTPUT);
  pinMode(BUZZER, INPUT);
  pinMode(WHITE_BUTTON, INPUT);
  pinMode(YELLOW_BUTTON, INPUT);
  pinMode(RED_BUTTON, INPUT);
  pinMode(BLUE_BUTTON, INPUT);
  pinMode(GREEN_BUTTON, INPUT);

  int result = 0;
  int flags = MIX_INIT_MP3;

  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    printf("Failed to init SDL\n");
    exit(1);
  }

  if (flags != (result = Mix_Init(flags))) {
    printf("Could not initialize mixer (result: %d).\n", result);
    printf("Mix_Init: %s\n", Mix_GetError());
    exit(1);
  }

  Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S16SYS, 2, 1024);



  DynArray vids_list;
  DynArray *vids_list_ptr = &vids_list;
  init_array(vids_list_ptr, 1);
  read_structs(vids_list_ptr);

  int led_state, buss_state, white_state, yellow_state, red_state, blue_state, green_state;

  int f_count = 0;

  int loop = 1;

  Video *loop_video = get_main_loop_video(vids_list_ptr);

  while (loop)
  {
    VideoCapture cap(loop_video->path);
    if (!cap.isOpened()) break;

    namedWindow(MAIN_WIN_NAME, WINDOW_NORMAL);
    setWindowProperty(MAIN_WIN_NAME, WND_PROP_FULLSCREEN, CAP_PROP_FORMAT);
    setWindowProperty(MAIN_WIN_NAME, WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN); // WINDOW_FULLSCREEN

    Mat frame;

    while (1)
    {
      cap >> frame;
      f_count++;
      imshow(MAIN_WIN_NAME, frame);

      if (f_count == (int)cap.get(CAP_PROP_FRAME_COUNT))
      {
        f_count = 0;
        cap.set(CAP_PROP_POS_FRAMES, f_count);
      }

      handle_pins();

      char c = (char) waitKey(50);
      if (c == 27)
      {
        loop = 0;
        break; // goes to the mainloop initiation
      }

      if (c == 32) // TAB // EXAMPLE for the moment, must be locked on a button pin
      {
        char type[10];
        strcpy(type, "drink");

        if (play_random_video(GIF_LENGTH, vids_list_ptr, type) == -1)
        {
          printf("Error playing random video.\n");
          return EXIT_FAILURE;
        }
      }
      if (c == 97) // 1
      {
        if (play_video( get_video_by_path(vids_list_ptr, "videos/point/point_pour_le_mamouth.mp4"), 0) == -1)
        {
          printf("Error playing random video.\n");
          return EXIT_FAILURE;
        }
      }
      if (c == 98) // 0
      {
        char type[15];
        strcpy(type, "miscelleanous");
        if (play_random_video(0, vids_list_ptr, type) == -1)
        {
          printf("Error playing random video.\n");
          return EXIT_FAILURE;
        }
      }
    }
    frame.release();
    cap.release();
  }

  destroyAllWindows();
  free_array(vids_list_ptr);

  SDL_Quit();

  pin_close(LED);
  pin_close(BUZZER);
  pin_close(WHITE_BUTTON);
  pin_close(YELLOW_BUTTON);
  pin_close(RED_BUTTON);
  pin_close(BLUE_BUTTON);
  pin_close(GREEN_BUTTON);

  printf("\n\n=========================\nEND OF THE GIFFER PROGRAM\n=========================\n\n");

  return EXIT_SUCCESS;
}
