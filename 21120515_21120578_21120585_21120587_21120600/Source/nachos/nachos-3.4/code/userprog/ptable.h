//Ptable.h



#ifndef PTABLE_H
#define  PTABLE_H
#define MAX_PROCESS 10

#include "bitmap.h"
#include "pcb.h"
#include "synch.h"


class PTable{
private:
  BitMap *bm;                // danh dau cac vi tri da duoc su dung trong PCB
  PCB *pcb[MAX_PROCESS];
  int psize;
  Semaphore *bmsem;         // co chuc nang ngan chan truong hop nap 2 tien trinh trong cung mot luc
public:
  // khoi tao size doi tuong PCB de luu size process. Gan gia tri ban dau la NULL
  // can phai khoi tao bm va bmsem de su dung
  //constructor
  PTable(int size);
  //destructor
  ~PTable();
  int ExecUpdate(char* name);     // xu ly cho system call SC_Exit
  int ExitUpdate(int ec);         // xu ly cho system call SC_Exit
  int JoinUpdate(int id);         // xu ly cho system call SC_Join
  int GetFreeSlot();              // tim free slot de luu thong tin cho tien trinh moi
  bool IsExist(int pid);          // kiem tra ton tai processID nay khong
  void Remove(int pid);           // khi tien trinh ket thuc, delete processID ra khoi mang quan ly no
  char* GetFileName(int id);      //tra ve ten cua tien trinh

};

#endif    //PTABLE_H
