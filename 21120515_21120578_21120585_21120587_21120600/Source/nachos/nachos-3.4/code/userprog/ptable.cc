//PTable.cc

#include "ptable.h"
#include "system.h"
#include "openfile.h"

PTable::PTable(int size){
  if(size < 0)
    return;
  this->psize = size;
  this->bm = new BitMap(size);
  bmsem = new Semaphore("bmsem",1);
  for(int i =0 ;i < MAX_PROCESS; ++i){
    pcb[i] = 0;
  }

  bm->Mark(0);
  pcb[0] = new PCB(0);
  pcb[0]->SetFileName("./test/testab");             
  pcb[0]->parentID = -1;

}
//destructor
PTable::~PTable(){
  delete bm;
  for(int i = 0; i< MAX_PROCESS; ++i)
    delete pcb[i];
  delete bmsem;
}


int PTable::ExecUpdate(char* name){
  bmsem->P();    // tranh tinh trang nap 2 tien trinh cung mot luc
  // kiem tra tinh hop le cua name
  if(name == NULL){
    printf("\nPTable -- ExecUpdate: name can't be a null value.\n");
    bmsem->V();
    return -1;
  }
  //kiem tra su ton tai cua chuong trinh Name
  if(strcmp(name,pcb[0]->GetFileName()) == 0 || strcmp(name,currentThread->getName()) == 0){
    printf("\nPTable -- ExecUpdate: can't execute itself -- %s -- %s -- %s\n",name,pcb[0]->GetFileName(),currentThread->getName());
    bmsem->V();
    return -1;
  }

  //neu tat ca deu hop le thi tim slot trong
  int freespace = this->GetFreeSlot();
  //kiem tra co tim duoc slot trong khong
  if(freespace < 0){
    printf("\nPTable -- ExecUpdate: there is not free slot.\n");
    bmsem->V();
    return -1;
  }
  //truong hop tim duoc slot trong
  pcb[freespace] = new PCB(freespace);                            // khoi tao PCB moi voi processID la index cua slot nay,
  pcb[freespace]->SetFileName(name);                          //set Name
  pcb[freespace]->parentID = currentThread->processID;      //parentID la processID cua currentThread

  //goi thi thi phuong thuc exec cua lop PCB
    int pid = pcb[freespace]->Exec(name,freespace);

    //goi bmsem->V()
    bmsem->V();
    //tra ve ket qua thuc thi cua phuong thuc Exec
    return pid;
}


int PTable::ExitUpdate(int ec){
  //truong hop tien trinh goi la main Process
  int id = currentThread->processID;
  if(id == 0){
    currentThread->FreeSpace();
    interrupt->Halt();
    return 0;
  }


  if(IsExist(id) == false){
    printf("\nPTable --- ExitUpdate: this %d is not exist.",id);
    return -1;
  }
  //truong hop tien trinh goi khong la main Process thi goi SetExitCode de dat exitcode cho tien trinh duoc goi
  pcb[id]->SetExitCode(ec);
  pcb[pcb[id]->parentID]->DecNumWait();
  //goi JoinRelease de giai phong tien trinh cha dang doi(neu co) va ExitWait de xin tien trinh cha duoc thoat
  pcb[id]->JoinRelease();
  pcb[id]->ExitWait();
  Remove(id);
  return ec;
}


int PTable::JoinUpdate(int id){
  //kiem tra tinh hop le cua id
  if(id < 0 || id > MAX_PROCESS){
    printf("\n PTable -- JoinUpate: Id %d is invalid.",id);
    return -1;
  }
  // kiem tra tien trinh dang goi co la tien trinh cha cua tien trinh cho processID la id khong
  if(currentThread->processID != pcb[id]->parentID){
    printf("\nPTable -- JoinUpdate: Can't join in process which is not it's parent process.");
    return -1;
  }
  //tang numwait va goi joinwait de cho tien trinh con thuc hien
  pcb[pcb[id]->parentID]->IncNumWait();

  pcb[id]->JoinWait();

  //xu ly exitcode sau khi tien trinh con thuc hien xong
  int ec = pcb[id]->GetExitCode();
  //cho phep tien trinh con thoat
  pcb[id]->ExitRelease();
  return ec;
}




int PTable::GetFreeSlot(){
  return bm->Find();
}


bool PTable::IsExist(int pid){
  return bm->Test(pid);
}


void PTable::Remove(int pid){
  bm->Clear(pid);
  if(pcb[pid] != NULL)
      delete pcb[pid];
}


char* PTable::GetFileName(int id){
  return pcb[id]->GetFileName();
}
