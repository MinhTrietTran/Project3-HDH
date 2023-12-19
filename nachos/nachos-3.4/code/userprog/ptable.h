#include "bitmap.h"
#include "pcb.h"

class PTable {
private:
    BitMap *bm;                    // đánh dấu các vị trí đã được sử dụng trong pcb
    PCB* pcb[MAX_PROCESS];        // mảng chứa các đối tượng PCB
    int psize;                    // kích thước đối tượng PCB
    Semaphore* bmsem;             // Semaphore để đồng bộ hóa truy cập vào bitmap
public:
    PTable(int size = 10);
    ~PTable();
    int ExecUpdate(char* name);
    int ExitUpdate(int ec);
    int JoinUpdate(int id);
    int GetFreeSlot();
    bool IsExist(int pid);
    void Remove(int pid);
    char* GetFileName(int id);
};