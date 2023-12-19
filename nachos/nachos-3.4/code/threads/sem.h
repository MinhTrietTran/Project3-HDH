#include "semaphore.h"
#include <cstring>

class Sem {
private:
    char name[50];
    Semaphore *sem; // Tạo Semaphore để quản lý
public:
    Sem(char* na, int i) {
        strcpy(this->name, na);
        sem = new Semaphore(name, i);
    }

    ~Sem() {
        delete sem;
    }

    void wait() {
        sem->P();
    }

    void signal() {
        sem->V();
    }

    char* GetName() {
        // Trả về tên của Semaphore
        return name;
    }
};
