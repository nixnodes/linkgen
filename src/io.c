#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <pthread.h>
#include <common.h>
#include <io.h>

int
write_file(char *data, FILE *fh, int rof, int f)
{
  int r;

  r = fwrite(data, 1, strlen(data), fh);

  if (!r)
    {
      if (f & 0x1)
        {
          fclose(fh);
        }
      return rof;
    }

  return 0;
}

long
get_file_size(FILE *fp)
{
  long offset, s_offset;

  s_offset = ftell(fp);
  fseek(fp, 0L, SEEK_END);
  offset = ftell(fp);
  fseek(fp, s_offset, SEEK_SET);

  return offset;
}

long
read_file(char *file, unsigned char *buffer, size_t read_max)
{
  size_t read;
  FILE *fp;

  if ((fp = fopen(file, "r")) == NULL)
    return -0xF;

  for (read = 0; !feof(fp) && read < read_max;
      read += fread(&buffer[read], 1, read_max - read - 1, fp))
    {
    }

  fclose(fp);

  return read;
}

int
open_pipe_to_executable(char *command, po_thrd o_thread)
{
  o_thread->fd_pipe = popen(command, "r");

  if (o_thread->fd_pipe == NULL)
    return errno;

  return 0;
}

int
read_from_pipe(char *buffer, FILE *pipe)
{
  size_t read = 0;

  while (!feof(pipe))
    {
      if ((read += fread(&buffer[read], 1,
      READ_MAX - read, pipe)) <= 0)
        {
          break;
        }
    }

  return read;
}

int
file_exists(char *file)
{
  int r;

  errno = 0;
  FILE *fd = fopen(file, "r");

  r = errno;

  if (fd)
    {
      fclose(fd);
    }

  return r;
}

int
dir_exists(char *dir)
{
  int r;

  errno = 0;
  DIR *dd = opendir(dir);

  r = errno;

  if (dd)
    {
      closedir(dd);
    }

  return r;
}

int
exec_and_wait_for_output(po_thrd ptr, char *command)
{
  char buf[0x2000];
  int r = 4;

  bzero(buf, 0x2000);

  if ((r = open_pipe_to_executable(command, ptr)))
    {
      return -0xE;
    }

  r = read_from_pipe(buf, ptr->fd_pipe);

  pclose(ptr->fd_pipe);

  return 0;
}

int
enum_dir(char *dir, void *cb, void *arg, int f)
{
  int
  (*callback_f)(char *data, unsigned char type, void *arg) = NULL;
  struct dirent *dirp;
  int r = 0, ir;

  callback_f = cb;

  if (!callback_f)
    {
      return 1060;
    }

  DIR *dp = opendir(dir);

  if (!dp)
    {
      return 1061;
    }

  while ((dirp = readdir(dp)))
    {
      if ((ir = callback_f(dirp->d_name, dirp->d_type, arg)))
        {
          if (f == 1)
            return ir;
          else
            r++;
        }
    }
  return r;
}
