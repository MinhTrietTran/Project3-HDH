#include "synch.h"

class PCB {
private:
    Semaphore* joinsem;    // semaphore cho quá trình join
    Semaphore* exitsem;    // semaphore cho quá trình exit
    Semaphore* mutex;      // semaphore cho quá trình truy xuất đọc quyền
    int exitcode;          // exit code của tiến trình
    int numwait;           // số tiến trình đã join
public:
    int parentID;          // ID của tiến trình cha
    char filename[MAX_STRING_SIZE]; // Tên của tiến trình
    int processID;         // Process ID của tiến trình

    PCB();
    PCB(int id);
    ~PCB();

    int Exec(char* filename, int pid); // Tạo 1 thread mới có tên là filename và process là pid
    int GetID();
    int GetNumWait();
    void JoinWait();
    void ExitWait();
    void JoinRelease();
    void ExitRelease();
    void IncNumWait();
    void DecNumWait();
    void SetExitCode(int ec);
    int GetExitCode();
    void SetFileName(char* fn);
    char* GetFileName();
};

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
    // ...

    return 0; // placeholder, you may need to change the return value
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
