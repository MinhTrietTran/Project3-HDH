#include "syscall.h"
void main(){
  int file;
  char* info = "nice to meet you ";
  file = Open("newfile.txt",0);
  Seek(-1,file);
  Write(info, 17, file);
  return;
}
