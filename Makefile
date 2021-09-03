compile:
	g++ -fsanitize=address main.c -o giffer  `pkg-config --cflags --libs opencv4` -I/usr/local/include/opencv4 `sdl2-config --cflags --libs` -lSDL2_mixer
	g++ -fsanitize=address init.c -o init  `pkg-config --cflags --libs opencv4` -I/usr/local/include/opencv4 `sdl2-config --cflags --libs` -lSDL2_mixer
