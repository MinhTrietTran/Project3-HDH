#include "pcb.h"

PCB::PCB() {
    joinsem = new Semaphore("joinsem", 0);
    exitsem = new Semaphore("exitsem", 0);
    mutex = new Semaphore("mutex", 1);
    exitcode = 0;
    numwait = 0;
    parentID = -1;
    processID = -1;
}

PCB::PCB(int id) {
    joinsem = new Semaphore("joinsem", 0);
    exitsem = new Semaphore("exitsem", 0);
    mutex = new Semaphore("mutex", 1);
    exitcode = 0;
    numwait = 0;
    parentID = -1;
    processID = id;
}

PCB::~PCB() {
    delete joinsem;
    delete exitsem;
    delete mutex;
}

int PCB::Exec(char* filename, int pid) {
    // Implement the Exec function
    // Goi mutex P de tranh tinh trang hai tien trinh nap cung luc
    mutex->P();

    // Kiem tra thread da khoi tao thanh cong chua, neu chua thi bao loi khong du vung nho, goi mutex->V() va return
    this->thread = new Thread(filename); // (.threads/thread.h)

    if(this->thread == NULL) {
        printf("\nPCB::EXEC:: Not enough memory..!\n");
        mutex->V();
        return -1;
    }

    // Dat processID cua thread nay la id
    this->thread->processID = pid;
    // Dat parrentID cua thread nay la processID cua thread goi thuc thi Exec
    this->parentID = currentThread->processID;
    // Goi thuc thi Fork(StartProcess_2,pid) --> Cast thread thanh kieu int, sau do xu ly ham StartProcess cast Thread ve danh kieu cua no
    this->thread->Fork(StartProcess_2,pid);

    mutex->V();
    // Tra ve id
    return pid; // placeholder, you may need to change the return value
}

int PCB::GetID() {
    return processID;
}

int PCB::GetNumWait() {
    return numwait;
}

void PCB::JoinWait() {
    joinsem->P();
}

void PCB::ExitWait() {
    exitsem->P();
}

void PCB::JoinRelease() {
    joinsem->V();
}

void PCB::ExitRelease() {
    exitsem->V();
}

void PCB::IncNumWait() {
    mutex->P();
    numwait++;
    mutex->V();
}

void PCB::DecNumWait() {
    mutex->P();
    if(numwait > 0)
        numwait--;
    mutex->V();
}

void PCB::SetExitCode(int ec) {
    exitcode = ec;
}

int PCB::GetExitCode() {
    return exitcode;
}

void PCB::SetFileName(char* fn) {
    strncpy(filename, fn, MAX_STRING_SIZE);
}

char* PCB::GetFileName() {
    return filename;
}
