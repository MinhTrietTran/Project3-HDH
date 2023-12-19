#include "ptable.h"

PTable::PTable(int size) {
    psize = size;
    bmsem = new Semaphore("bmsem", 1);

    // Khởi tạo Bitmap với kích thước là số đối tượng PCB có thể chứa
    bm = BitMap(psize);

    // Khởi tạo mảng PCB với các đối tượng PCB
    for (int i = 0; i < psize; i++) {
        pcb[i] = nullptr;
    }

    // Tạo đối tượng PCB cho tiến trình cha ở vị trí 0
    pcb[0] = new PCB(0);
}

PTable::~PTable() {
    delete bmsem;

    // Xóa các đối tượng PCB
    for (int i = 0; i < psize; i++) {
        if (pcb[i] != nullptr) {
            delete pcb[i];
        }
    }
}

int PTable::ExecUpdate(char* name) {
    bmsem->P();

    // Tìm một slot trống để lưu thông tin của tiến trình mới
    int slot = GetFreeSlot();

    // Kiểm tra xem có slot trống không
    if (slot == -1) {
        bmsem->V();
        return -1; // Không có slot trống
    }

    // Tạo đối tượng PCB cho tiến trình mới
    pcb[slot] = new PCB(slot);

    // Set filename cho PCB
    pcb[slot]->SetFileName(name);

    // Đánh dấu slot đã được sử dụng
    bm.Mark(slot);

    bmsem->V();
    return slot; // Trả về process ID của tiến trình mới
}

int PTable::ExitUpdate(int ec) {
    int pid = currentThread->processID;

    if (!IsExist(pid)) {
        return -1; // Tiến trình không tồn tại trong bảng quản lý
    }

    bmsem->P();

    // Xử lý cho system call SC_Exit
    // ...

    bm.Clear(pid); // Đánh dấu slot đã được giải phóng

    bmsem->V();
    return 0;
}

int PTable::JoinUpdate(int id) {
    if (!IsExist(id)) {
        return -1; // Tiến trình không tồn tại trong bảng quản lý
    }

    bmsem->P();

    // Xử lý cho system call SC_Join
    // ...

    bmsem->V();
    return 0;
}

int PTable::GetFreeSlot() {
    return bm.Find(); // Sử dụng hàm Find của Bitmap để tìm slot trống
}

bool PTable::IsExist(int pid) {
    return bm.Test(pid); // Sử dụng hàm Test của Bitmap để kiểm tra sự tồn tại
}

void PTable::Remove(int pid) {
    bm.Clear(pid); // Sử dụng hàm Clear của Bitmap để giải phóng slot
}

char* PTable::GetFileName(int id) {
    if (!IsExist(id)) {
        return nullptr; // Tiến trình không tồn tại trong bảng quản lý
    }

    return pcb[id]->GetFileName();
}