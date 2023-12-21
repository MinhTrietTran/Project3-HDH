#include "pcb.h"
#include "utility.h"
#include "system.h"
#include "thread.h"
#include "addrspace.h"


extern void StartProcessWithID(int id);

PCB::PCB(){

  this->joinsem = new Semaphore("joinsem",0);
  this->exitsem = new Semaphore("exitsem",0);
  this->multex = new Semaphore("multex",1);

  this->thread = NULL;
  this->numwait = 0;
  this->exitcode =0;

  this->parentID = -1;

}
PCB::PCB(int id){
  if(id == 0)
    this->parentID = -1;
  else
    this->parentID = currentThread->processID;

    this->joinsem = new Semaphore("joinsem",0);
    this->exitsem = new Semaphore("exitsem",0);
    this->multex = new Semaphore("multex",1);

    this->thread = NULL;
    this->numwait = 0;
    this->exitcode =0;
}

PCB::~PCB(){
  if(joinsem != NULL)
    delete joinsem;
  if(exitsem != NULL)
    delete exitsem;
  if(multex != NULL)
    delete multex;
  if(thread != NULL){
    thread->FreeSpace();
    thread->Finish();
  }
}


void PCB::JoinWait(){
  //chuyen tien trinh sang trang thai block va dung lai , cho JoinRelease de thuc hien tiep
  joinsem->P();
}


void PCB::ExitWait(){
  //chuyen tien trinh sang trang thai block va dung lai , cho ExitRelease de thuc hien tiep
  exitsem->P();
}


void PCB::JoinRelease(){
  //giai phong tien trinh goi joinWait
  joinsem->V();
}


void PCB::ExitRelease(){
  //giai phong tien trinh dang cho
  exitsem->V();
}


void PCB::IncNumWait(){
  multex->P();
  ++numwait;
  multex->V();
}


void PCB::DecNumWait(){
  multex->P();
  if(numwait > 0)
    --numwait;
  multex->V();
}


int PCB::Exec(char* filename, int pid){
  multex->P();
  //kiem tra viec tao thread thanh cong hay chua
  thread = new Thread(filename);
  if(thread == NULL){
    printf("\n PCB_ERROR: Error in initial thread" );
    multex->V();
    return -1;
  }
  // dat process id cua thread nay la pid
  this->thread->processID = pid;
  //dat  parrentID của thread này là processID của thread gọi thực thi Exec
  this->parentID = currentThread->processID;
  // goi thuc thi fork
  this->thread->Fork(StartProcessWithID,pid);
  multex->V();
  return pid;
}



int PCB::GetNumWait(){return this->numwait;}
int PCB::GetID(){return this->thread->processID;}
int PCB::GetExitCode(){return this->exitcode;}
char* PCB::GetFileName(){  return this->filename;}

void PCB::SetExitCode(int code){  this->exitcode = code;}
void PCB::SetFileName(char* name){  strcpy(this->filename,name);}
