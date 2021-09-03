char *str_trim_extention(char *path)
{
  char *ret = (char*) malloc( ( strlen(path) - 3) * sizeof(char) );

  int length = strlen(path) - 4;

  for (int i = 0; i < length; i++)
  {
    ret[i] = path[i];
  }
  ret[length] = '\0';
  return ret;
}

char *replace_extension(char *path, char *new_ext)
{
  char *trimmed = str_trim_extention(path);
  char *new_path = (char*) malloc( (strlen(path)+1) * sizeof(char) );
  strcpy(new_path, trimmed);
  free(trimmed);
  strcat(new_path, new_ext);
  return new_path;
}

char *replace_extension_with_const(char *path, const char *new_ext)
{
  char *trimmed = str_trim_extention(path);
  char *new_path = (char*) malloc( (strlen(path)+1) * sizeof(char) );
  strcpy(new_path, trimmed);
  free(trimmed);
  strcat(new_path, new_ext);
  return new_path;
}
