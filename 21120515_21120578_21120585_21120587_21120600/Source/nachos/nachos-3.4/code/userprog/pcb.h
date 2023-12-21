// PCB.h


#ifndef PCB_H
#define PCB_H
#include "thread.h"
#include "synch.h"

class PCB{
private:
  Semaphore *joinsem;     //semaphore cho qua trinh join
  Semaphore *exitsem;     // semaphore cho qua trinh exit
  Semaphore *multex;      // semaphore cho qua trinh truy xuat doc quyen
  int exitcode;
  int numwait;            // so tien trinh da join
  char filename[32];     // ten tien trinh
  Thread* thread;         //tien trinh cua chuong trinh

public:
  int parentID;           //id cua tien trinh cha

  // constructor
  PCB();
  PCB(int id);
  //deconstructor
  ~PCB();
  //nap chuong trinh co ten luu trong tham so filename va process ID la parentID
  int Exec(char* filename, int pid);    // tao 1 thread moi co ten la filename va process la pid
  int GetID();          //tra ve ProcessID cua tien trinh goi thuc hien
  int GetNumWait();     // tra ve so luong tien trinh cho

  void JoinWait();      // 1. tien trinh cha doi tien trinh con ket thuc
  void ExitWait();      // 4. Tien trinh con ket thuc
  void JoinRelease();   // 2. Bao cho tien trinh cha thuc thi tiep
  void ExitRelease();   // 3. Cho phep tien trinh con ket thuc

  void IncNumWait();    //tang so tien trinh cho
  void DecNumWait();    // giam so tien trinh cho

  void SetExitCode(int ec);     // dat exitcode cua tien trinh
  int GetExitCode();           // tra ve exitcode cua tien trinh

  void SetFileName(char* fn);   // dat ten cho tien trinh
  char* GetFileName();           //tra ve ten cua tien trinh
};

#endif    // PCB_H
