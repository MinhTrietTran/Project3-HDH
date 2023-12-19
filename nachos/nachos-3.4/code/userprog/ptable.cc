#include "ptable.h"
#include <string.h>

PTable::PTable(int size) {
    if (size < 0)
        return;
    psize = size;
     // Khởi tạo Bitmap với kích thước là số đối tượng PCB có thể chứa
    bm = new BitMap(psize);
    bmsem = new Semaphore("bmsem", 1);

   

    // Khởi tạo mảng PCB với các đối tượng PCB
    for (int i = 0; i < psize; i++) {
        pcb[i] = nullptr;
    }
    bm -> Mark(0);
    // Tạo đối tượng PCB cho tiến trình cha ở vị trí 0
    pcb[0] = new PCB(0);
    pcb[0] = SetFileName("./test/scheduler");
    pcb[0]->parentID = -1;
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
    // Goi P de tranh tinh trang hai tien trinh duoc nap cung luc
    bmsem->P();

  

    // Kiem tra su ton tai cua chuong trinh "name" bang cach goi phuong thuc Open cua fileSystem
    if(name == NULL) {
        printf("\nPTable::Exec : Can not execute name is NULL.\n");
        bmsem->V();
        return -1;
    }

    // So sanh ten chuong trinh va ten cua currentThread de chan chac chuong trinh khong goi thuc thi chinh no
    if(strcmp(name,"./test/scheduler") == 0 || strcmp(name, currentThread->getName()) == 0) {
        printf("\nPTable::Exec : Can not execute itself. \n");
        bmsem->V();
        return -1;
    }

    // Tìm một slot trống để lưu thông tin của tiến trình mới
    int slot = GetFreeSlot();

    // Kiểm tra xem có slot trống không
    if (slot < 0) {
        bmsem->V();
        return -1; // Không có slot trống
    }

    // Tạo đối tượng PCB cho tiến trình mới
    pcb[slot] = new PCB(slot);

    // Set filename cho PCB
    pcb[slot]->SetFileName(name);
    // ParentID la processID cua currentThread
    pcb[slot]->parentID = currentThread->processID;

    // Goi thuc thi phuong thuc Exec cua PCB
    int pid = pcb[slot]->Exec(name,slot);
    // Goi V()
    bmsem->V();
    return pid; // Trả về thuc thi cua PCB->Exec
}

int PTable::ExitUpdate(int ec) {
    // Nếu tiến trình gọi là main process thì gọi Halt().
	int id = currentThread->processID;
	if(id == 0)
	{
		
		currentThread->FreeSpace();		
		interrupt->Halt();
		return 0;
	}
    
        if(IsExist(id) == false)
	{
		printf("\nPTable::ExitUpdate: This %d is not exist. Try again?", id);
		return -1;
	}


	// Ngược lại gọi SetExitCode để đặt exitcode cho tiến trình gọi.
	pcb[id]->SetExitCode(ec);
	pcb[pcb[id]->parentID]->DecNumWait();
    
    // Gọi JoinRelease để giải phóng tiến trình cha đang đợi nó(nếu có) và ExitWait() để xin tiến trình cha
    // cho phép thoát.
	pcb[id]->JoinRelease();
    // 
	pcb[id]->ExitWait();
	
	Remove(id);
	return ec;
}

int PTable::JoinUpdate(int id) {
    // Ta kiểm tra tính hợp lệ của processID id và kiểm tra tiến trình gọi Join có phải là cha của tiến trình
	// có processID là id hay không. Nếu không thỏa, ta báo lỗi hợp lý và trả về -1.
	if(id < 0)
	{
		printf("\nPTable::JoinUpdate : id = %d", id);
		return -1;
	}
	// Check if process running is parent process of process which joins
	if(currentThread->processID != pcb[id]->parentID)
	{
		printf("\nPTable::JoinUpdate Can't join in process which is not it's parent process.\n");
		return -1;
	}


    	// Tăng numwait và gọi JoinWait() để chờ tiến trình con thực hiện.
	// Sau khi tiến trình con thực hiện xong, tiến trình đã được giải phóng
	pcb[pcb[id]->parentID]->IncNumWait();
	

	//pcb[id]->boolBG = 1;
	
	pcb[id]->JoinWait();

	// Xử lý exitcode.	
	int ec = pcb[id]->GetExitCode();
        // ExitRelease() để cho phép tiến trình con thoát.
	pcb[id]->ExitRelease();

    // Successfully
	return ec;
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