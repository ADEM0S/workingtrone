void *wait_for_music(void *arg)
{
  Mix_Music *music = (Mix_Music*) arg;
  while (Mix_PlayingMusic())
  {
    SDL_Delay(125);
  }
  Mix_FreeMusic(music);

  return NULL;
}


int launch_audio(Video *video)
{
	char *mp3_path = replace_extension_with_const(video->path, ".mp3");

  Mix_Music *music = Mix_LoadMUS(mp3_path);
  free(mp3_path);

  // Waits for other musics to be finished
  while (Mix_PlayingMusic())
  {
    SDL_Delay(250);
  }

  Mix_PlayMusic(music, 1);
  pthread_t wait_music;
  pthread_create(&wait_music, NULL, wait_for_music, music);

	return 0;
}
