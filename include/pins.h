int pin_open(int pin_n)
{
  // open the pin by writing the pin_n number into the export file
  FILE* fptr_exp = NULL;
  char path[] = "/sys/class/gpio/export";

  if ( (fptr_exp = fopen(path, "w")) == NULL ) return -1;
  fprintf(fptr_exp, "%d", pin_n);
  printf("exported %d\n", pin_n);
  fclose(fptr_exp);

  return 0;
}

int pinMode(int pin_n, int mode)
{
  // choose if it is in or out
  char dir_path[40];
  sprintf(dir_path, "/sys/class/gpio/gpio%d/direction", pin_n);

  char pin_mode[5] = "in";
  if (mode) strcpy(pin_mode, "out");

  FILE* fptr_dir;
  if ( (fptr_dir = fopen(dir_path, "w")) == NULL ) return -1;
  fprintf(fptr_dir, "%s", pin_mode);
  printf("made %d to %d\n", pin_n, mode);
  fclose(fptr_dir);
  return 0;
}

int pin_close(int pin_n)
{
  FILE* fptr_unexp = NULL;
  char path[] = "/sys/class/gpio/unexport";

  if ( (fptr_unexp = fopen(path, "w")) == NULL ) return -1;
  fprintf(fptr_unexp, "%d", pin_n);
  printf("unexported %d\n", pin_n);
  fclose(fptr_unexp);

  return 0;
}

void handle_pins()
{

}
