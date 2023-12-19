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