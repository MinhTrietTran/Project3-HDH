// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "machine.h"
#include "console.h"
// #include "synchcons.h"

#include <iostream>
#define MaxFileLength 32
#define MaxBufferSize 255

// additional


void IncreatePC(){
	// update preRegister to current register
	int current = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg,current);

	// update current register to next register
	int next = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg,next);
	// update next register to next of next
	machine->WriteRegister(NextPCReg,next+4);

}

/*
Input: - User space address (int)
- Limit of buffer (int)
Output:- Buffer (char*)
Purpose: Copy buffer from User memory space to System memory space
*/
char* User2System(int virtAddr,int limit)
{
	int i;// index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit +1];//need for terminal string
	if (kernelBuf == NULL)
	return kernelBuf;
	memset(kernelBuf,0,limit+1);
	//printf("\n Filename u2s:");
	for (i = 0 ; i < limit ;i++)
	{
		machine->ReadMem(virtAddr+i,1,&oneChar);
		kernelBuf[i] = (char)oneChar;
		//printf("%c",kernelBuf[i]);
		if (oneChar == 0)
		break;
	}
	return kernelBuf;
}


/*
Input: - User space address (int)
- Limit of buffer (int)
- Buffer (char[])
Output:- Number of bytes copied (int)
Purpose: Copy buffer from System memory space to User memory space
*/
int System2User(int virtAddr,int len,char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0 ;
	do{
		oneChar= (int) buffer[i];
		machine->WriteMem(virtAddr+i,1,oneChar);
		i ++;
	}while(i < len && oneChar != 0);
	return i;
}

void Write(char* buffer, int length){
	Console *console;
	for(int i = 0; i < length; ++i){
		console->PutChar(buffer[i]);
	}
}

// additional

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
  int type = machine->ReadRegister(2);

	switch(which){
		case NoException:
			return;
		case PageFaultException:
				DEBUG('a',"\n[Page Fault Exception] No valid translation found\n");
				printf("\n[Page Fault Exception] No valid translation found\n");
				// ASSERT(false);
				interrupt->Halt();
				return;
		case ReadOnlyException:
				DEBUG('a',"\n[Read Only Exception] Write attemped to page marked\n");
				printf("\n[Read Only Exception] Write attemped to page marked\n");
				// ASSERT(false);
				interrupt->Halt();
				return;
		case BusErrorException:
				DEBUG('a',"\n[Bus Errror Exception] Translation resulted in an valid physical address\n");
				printf("\n[Bus Error Exepption] Translation resulted in an valid physical address\n");
				// ASSERT(false);
				interrupt->Halt();
				return;
		case AddressErrorException:
				DEBUG('a',"\n[Address Error Exception] Unaligned reference or one that was beyond the end of the address space\n");
				printf("\n [Address Error Exception] Unaligned reference or one that was beyond the end of the address space \n");
				// ASSERT(false);
				interrupt->Halt();
				return;
		case OverflowException:
				DEBUG('a',"/n[OverFlow Exception] Integer overflow in add or sub\n");
				printf("\n[Overflow Exception] Integer overflow in add or sub\n");
				// ASSERT(false);
				interrupt->Halt();
				return;
		case IllegalInstrException:
				DEBUG('a',"\n[Illegal Instruction Exception] Unimplemented or reserved instr\n");
				printf("\n[Illegal Instruction Exception] Unimplemented or reserved instr\n");
				// ASSERT(false);
				interrupt->Halt();
				return;
		case NumExceptionTypes:
				DEBUG('a',"\n[Number Exception]\n");
				printf("\n[Number Exception]\n");
				// ASSERT(false);
				interrupt->Halt();
				return;
		case SyscallException:{
			switch(type){
				case SC_Create:{
					int virtAddr;
					char* filename;
					DEBUG('a',"\n SC_Create call ...");
					DEBUG('a',"\n Reading virtual address of filename");
					// Lấy tham số tên tập tin từ thanh ghi r4
					virtAddr = machine->ReadRegister(4);
					DEBUG ('a',"\n Reading filename.");
					// MaxFileLength là = 32
					filename = User2System(virtAddr,MaxFileLength+1);
					if (filename == NULL)
					{
						printf("\n Not enough memory in system");
						DEBUG('a',"\n Not enough memory in system");
						machine->WriteRegister(2,-1); // trả về lỗi cho chương
						// trình người dùng
						delete[] filename;
						break;
					}
					//truong hop ten file khong hop le
					if(strlen(filename) == 0){
						printf("\n Filename is Invalid.");
						DEBUG('a',"\nFilename is Invalid.");
						machine->WriteRegister(2,-1);
						delete[] filename;
						break;
					}
					DEBUG('a',"\n Finish reading filename.");
					//DEBUG(‘a’,"\n File name : '"<<filename<<"'");
					// Create file with size = 0
					// Dùng đối tượng fileSystem của lớp OpenFile để tạo file,
					// việc tạo file này là sử dụng các thủ tục tạo file của hệ điều
					// hành Linux, chúng ta không quản ly trực tiếp các block trên
					// đĩa cứng cấp phát cho file, việc quản ly các block của file
					// trên ổ đĩa là một đồ án khác
					if (!fileSystem->Create(filename,0))
					{
						printf("\n Error create file '%s'",filename);
						machine->WriteRegister(2,-1);
						delete[] filename;
						break;
					}
					//tao file thanh cong
					machine->WriteRegister(2,0); // trả về cho chương trình
					// người dùng thành công
					delete[] filename;
					break;
				}
				case SC_Open:{
					//lay ten file tu thanh ghi so 4 va kiem tra tinh hop le
					int addr = machine->ReadRegister(4);
					char* filename = User2System(addr,MaxFileLength + 1);
					if(filename == NULL){
						DEBUG('f',"\nSC_OPEN_ERROR : Not enough memory in system.");
						printf("\nSC_OPEN_ERROR : Not enough memory in system.");
						machine->WriteRegister(2,-1);
						delete[] filename;

						return;
					}
					if(strlen(filename) == 0 || strlen(filename) > MaxFileLength){
						DEBUG('f',"\nSC_OPEN_ERROR : filename is Invalid.");
						printf("\nSC_OPEN_ERROR : Filename is Invalid.");
						machine->WriteRegister(2,-1);
						delete[] filename;

						return;
					}
					// lay type tu thanh ghi so 5 va kiem tra tinh hop le
					int type = machine->ReadRegister(5);
					if(type != 0 && type != 1){
						DEBUG('f',"\nSC_OPEN_ERROR : Type is Invalid.");
						printf("\nSC_OPEN_ERROR : Type is Invalid.");
						machine->WriteRegister(2,-1);
						delete[] filename;
						return;
					}
					int freeSlot = fileSystem->FindFreeSlot();
					if(freeSlot != -1){
						if ((fileSystem->openState[freeSlot] = fileSystem->Open(filename, type)) != NULL) //Mo file thanh cong
						{
							machine->WriteRegister(2, freeSlot); //tra ve OpenFileI
						}
						else
							machine->WriteRegister(2,-1);;
					}
					delete[] filename;
					break;
				}
				case SC_Close:{
					//doc ten file tu thanh ghi thu 4
					int id = machine->ReadRegister(4);
					// kiem tra tinh hop le
					if(id >= 0 && id <= 9){
						//kiem tra da mo file thanh cong chua
						if(fileSystem->openState[id]){
							delete fileSystem->openState[id];
							fileSystem->openState[id] = NULL;
							machine->WriteRegister(2,0);
							break;
						}
					}
					machine->WriteRegister(2,-1);		// truong hop dong file that bai
					break;
				}
				case SC_Read:{
					//lay Input
					int virtAddr = machine->ReadRegister(4);		//lay dia chi cua buffer tu thanh ghi so 4
					int charcount = machine->ReadRegister(5);	//lay gia tri charcount tu thanh ghi so 5
					int id = machine->ReadRegister(6);		//lay id file tu thanh ghi so 6
					char* buffer;
					// tuong hop size khong hop le
					if(charcount < 0){
						DEBUG('f',"\nSC_READ_ERROR: Unexpected buffer size.");
						printf("\nSC_READ_ERROR: Unexpected buffer size.");
						machine->WriteRegister(2,-1);
						return ;
					}
					// kiem tra tinh hop le cua file id
					if(id < 0 || id > 9){
						DEBUG('f',"\nSC_READ_ERROR : Unexpected file id %d",id);
						printf("\nSC_READ_ERROR: Unexpected file id %d",id);
						machine->WriteRegister(2,-1);
						return;
					}
					//kiem tra file co ton tai khong
					if(fileSystem->openState[id] == NULL){
						DEBUG('f',"\nSC_READ_ERROR: Reading file id %d is not open.",id);
						printf("\n SC_READ_ERROR: Reading file id %d is not open.",id);
						machine->WriteRegister(2,-1);
						return;
					}
					//kiem tra tuong hop file muon doc la file stdout
					if(fileSystem->openState[id]->type == 3){
						DEBUG('f',"\nSC_READ_ERROR: You cannot read file stdout. This is only write file");
						printf("\nSC_READ_ERROR: You cannot read file stdout. This is only write file");
						machine->WriteRegister(2,-1);
						return;
					}
					//lay ra vi tri hien tai de doc file
					int curPos = fileSystem->openState[id]->GetCurrentPos();
					//chuyen tu userspce sang systemspace
					buffer = User2System(virtAddr,charcount);
					//truong hop nguoi dung muon doc file quy uoc stdin(type = 2)
					if(fileSystem->openState[id]->type == 2){
							//doc size
							int size = gSynchConsole->Read(buffer,charcount);
							//chuyen tu vung nho systemSpace sang user space
							System2User(virtAddr,size,buffer);
							//tra ve so luong byte thuc su doc duoc
							machine->WriteRegister(2,size);
							delete buffer;
							return;
					}
					//truong hop doc file binh thuong (type = 0)
					if(fileSystem->openState[id]->Read(buffer,charcount) > 0){
						int newPos = fileSystem->openState[id]->GetCurrentPos();
						System2User(virtAddr,newPos - curPos,buffer);
						machine->WriteRegister(2,newPos-curPos);
					}
					//truong hop file rong tra ve -2
					else{
						machine->WriteRegister(2,-2);
					}
					delete buffer;
					break;
				}
				case SC_Write:{
					//doc input
					int virtAddr = machine->ReadRegister(4);		//doc dia chi chua buffer trong thanh ghi so 4
					int charcount = machine->ReadRegister(5);		//doc dia gia tri charcount trong thanh ghi so 5
					int id = machine->ReadRegister(6);			//doc file id trong thanh ghi so 6
					char *buffer;
					if(charcount < 0){
						DEBUG('f',"\nSC_WRITE_ERROR: Unexpected buffer size.");
						printf("\nSC_WRITE_ERROR: Unexpected buffer size.");
						machine->WriteRegister(2,-1);
						return ;
					}
					// kiem tra tinh hop le cua file id
					if(id < 0 || id > 9){
						DEBUG('f',"\nSC_WRITE_ERROR : Unexpected file id %d",id);
						printf("\nSC_WRITE_ERROR: Unexpected file id %d",id);
						machine->WriteRegister(2,-1);
						return;
					}
					//kiem tra file co ton tai khong
					if(fileSystem->openState[id] == NULL){
						DEBUG('f',"\nSC_WRITED_ERROR: Reading file id %d is not open.",id);
						printf("\n SC_WRITE_ERROR: Reading file id %d is not open.",id);
						machine->WriteRegister(2,-1);
						return;
					}
					//kiem tra tuong hop file muon ghi la file stdin
					if(fileSystem->openState[id]->type == 2){
						DEBUG('f',"\nSC_WRITE_ERROR: You cannot write file stdin. This is only read file");
						printf("\nSC_WRITE_ERROR: You cannot write file stdin. This is only read file");
						machine->WriteRegister(2,-1);
						return;
					}
					//lay vi tri bat dau ghi file
					int curPos = fileSystem->openState[id]->GetCurrentPos();

					//chuyen tu userspace ve thanh systemspace
					buffer = User2System(virtAddr,charcount);
					//xet truong hop ghi file stdout (type = 3)
					if(fileSystem->openState[id]->type == 3){
							//ghi cho den khi gap ky tu \0 thi dung lai
						int i = 0;
						while(buffer[i] = '\n'){
							gSynchConsole->Write(buffer+i,1);		//ghi tung byte mot
							++i;
						}
						//ghi ky tu \0
						buffer[i] = '\n';
						gSynchConsole->Write(buffer+i,1);
						//ghi ket qua so byte da ghi duoc
						machine->WriteRegister(2,i-1);

						delete buffer;
						return;
					}
					//truong hop ghi vao file binh thuong(type == 0)
					//goi ham Write de ghi
					if(fileSystem->openState[id]->Write(buffer,charcount) > 0){
							// tinh so byte thuc su ghi duoc
							int newPos = fileSystem->openState[id]->GetCurrentPos();
							machine->WriteRegister(2,newPos-curPos);
							delete buffer;

					}
					break;
				}
				case SC_Seek:{
					//doc input
					//do vi tri muon di chuyen tai thanh ghi so 4
					int pos = machine->ReadRegister(4);
					//doc file id tu thanh ghi so 5
					int id = machine->ReadRegister(5);
					//kiem tra id
					if(id < 0 || id > 9){
						printf("\nSC_SEEK_ERROR : Unexpected file id.");
						DEBUG('f',"\nSC_SEEK_ERROR: Unexpected file id. ");
						machine->WriteRegister(2,-1);
						return;
					}
					//kiem tra file da ton tai chua
					if(fileSystem->openState[id] == NULL){
						DEBUG('f',"\nSC_SEEK_ERROR: File %d is not exist",id);
						printf("\nSC_SEEK_ERROR: File %d is not exist",id);
						machine->WriteRegister(2,-1);
						return;
					}
					//kiem ta viec goi Seek tren console
					if(id == 0 || id == 1){
						DEBUG('f',"\nSC_SEEK_ERROR: can not call Seek from console. ");
						printf("\nSC_SEEK_ERROR: can not call Seek from console.");
						machine->WriteRegister(2,-1);
						return;
					}
					//neu pos = -1 thi di chuyen toi cuoi file
					if(pos == -1)
						pos = fileSystem->openState[id]->Length();
					//di chuyen toi vi tri chi dinh
					fileSystem->openState[id]->Seek(pos);
					machine->WriteRegister(2,pos);
					break;
				}
				case SC_Halt:{
					DEBUG('a',"\nShut down, initialize by user program\n");
					printf("\nShut down, initialize by user program\n");
					interrupt->Halt();
					break;
				}
				// Đoạn mã này xử lý lệnh gọi hệ thống SC_ReadInt, đọc một số nguyên từ đầu vào của bàn điều khiển.
				// Nó khởi tạo bộ đệm để lưu trữ dữ liệu đầu vào, đọc các ký tự từ bảng điều khiển và chuyển đổi chúng thành số nguyên.
				// Mã đảm bảo đầu vào là số nguyên hợp lệ và xử lý số âm một cách thích hợp.
				// Nếu đầu vào hợp lệ, số nguyên được ghi vào thanh ghi 2; nếu không, một thông báo lỗi sẽ được in.
				case SC_ReadInt:{
					// Cấp phát bộ nhớ cho bộ đệm đầu vào
					char* buffer = new char[MaxBufferSize + 1];

					// Đọc ký tự từ console vào bộ đệm
					int bytesRead = gSynchConsole->Read(buffer,MaxBufferSize);

					// Null-terminate buffer
					buffer[bytesRead] = '\0';

					// Khởi tạo các biến dấu, chỉ mục và kết quả
					int sign = 1;
					int i = 0;
					int result = 0;

					// Gắn cờ để theo dõi tính hợp lệ của đầu vào
					bool isValid = true;

					// Xử lý bộ đệm đầu vào
					if(bytesRead > 0){
						if(buffer[0] == '-'){
							sign = -1;
							i++;
						}

						// Chuyển ký tự thành số nguyên
						for(; i < bytesRead; i++){
							if(buffer[i] >= '0' && buffer[i] <= '9'){
								result = result * 10 + (buffer[i] - '0');
							}
							else{
								isValid = false;
								break;
							}
						}

						// Kiểm tra đầu vào một ký tự có dấu âm
						if(bytesRead == 1){
							if(sign == -1){
								isValid = false;
							}
						}

						// Xử lý kết quả dựa trên tính hợp lệ
						if(isValid == true){
							result = result * sign;
							// Viết kết quả vào thanh ghi 2
							machine->WriteRegister(2,result);
						}
						else{
							DEBUG('a',"\nSC_READINT: Invalid - Input is non Integer\n");

							// In thông báo lỗi do đầu vào không hợp lệ
							printf("\nSC_READINT : Invalid - Invalid - Input non Integer\n");

							// Viết 0 để thanh ghi 2 trong trường hợp đầu vào không hợp lệ
							machine->WriteRegister(2,0);
						}
					}
					else{
						// In thông báo lỗi khi đầu vào trống
						DEBUG('a',"\nSC_READINT : Invalid - Input is Empty\n");
						printf("\nSC_READINT : Invalid - Invalid - Input is Empty\n");
					}
					delete[] buffer;
					break;
				}
				// Đoạn mã này xử lý lệnh gọi hệ thống SC_PrintInt, in một số nguyên ra đầu ra của bàn điều khiển.
				// Nó đọc số nguyên từ thanh ghi 4, chuyển nó thành bộ đệm ký tự và sau đó ghi nó vào bàn điều khiển.
				// Số âm được xử lý bằng cách in dấu '-' và chuyển giá trị tuyệt đối thành ký tự.
				case SC_PrintInt:{

					// Đọc số nguyên từ thanh ghi 4
					int number = machine->ReadRegister(4);
					// Cấp phát bộ nhớ cho vùng đệm ký tự
					char *buffer = new char[MaxBufferSize + 1];
					// Xử lý số và in ra console
					if(number == 0){
						// Print '0' cho số 0
						gSynchConsole->Write("0", 1);
					}
					else{
						if(number < 0){
							number = number * -1;
							// Xử lý số âm bằng cách in dấu '-'
							gSynchConsole->Write("-", 1);
						}
						// Tính độ dài của số
						int length = 0;
						int temp = number;
						while(temp > 0){
							length++;
							temp /= 10;
						}
						// Chuyển số thành ký tự và lưu vào bộ đệm
						for(int i = length - 1; i >= 0; i--){
							buffer[i] = ((number % 10) + '0');
							number /= 10;
						}
						// Null-terminate buffer
						buffer[length] = '\0';
						// Ghi bộ đệm ra console
						gSynchConsole->Write(buffer, length + 1);

					}
					// Giải phóng bộ nhớ đệm
					delete[] buffer;

					break;
				}
				case SC_ReadChar:{
					// Syscall này tương ứng với hàm ReadChar được sử dụng "char ReadChar();"
					// Khởi tạo 1 bảng các ký tự với số lượng MaxBufferSize + 1. Thêm 1 phần tử phần kết thúc chuỗi
					char *input = new char[MaxBufferSize + 1];

					 // Lấy dữ liệu từ màn hình vào mảng input và số lượng byte đọc được sẽ được lưu vào biến bytes
					int bytes = gSynchConsole -> Read(input, MaxBufferSize);

					// Kiểm tra nếu số lượng bytes mà đọc được lớn hơn 1 tức là đã nhập 1 chuỗi
					// Chỉ chấp nhận 1 byte. 1 byte tương ứng với 1 ký thực char (char = 1 byte)
					if(bytes > 1){
						DEBUG('a',"\nSC_READCHAR : Invalid - Number of characters is more than 1\n");
						printf("\nSC_READCHAR : Invalid - Number of characters is more than 1\n");
						machine->WriteRegister(2,0);
					}
					else if( bytes == 0){ // Loại bõ trường hợp nhập vào empty (Trường hợp người dùng enter)
						DEBUG('a',"\nSC_READCHAR : Invalid - Input character is empty\n");
						printf("\nSC_READCHAR : Invalid - Input character is empty\n");
						machine->WriteRegister(2,0);
					}
					else if(bytes == 1){ // Nếu đúng 1 byte thì tiến hành
						// Tạo 1 biến char và gán nó chính là giá trị của phần tử đầu tiên của mảng input
						char onechar = *input;
						// Dùng machine( biến con trỏ dạng máy được khai báo toàn cục) để ghi onchar vào thanh ghi số 2
						// Thanh ghi số 2 là thành ghi trả về
						machine->WriteRegister(2, onechar);
					}
					delete input; // Sau khi thực hiện xong tiến hành lấy lại vùng nhớ cấp phát cho input
					break;
				}
				case SC_PrintChar:{
					// Syscall này dùng để in ra màn hình cho syscall ReadChar với định nghĩa hàm "void PrintChar(char value);"
					// Hàm nhận vào 1 "char value" làm tham số, tiến hành lấy tham số này từ thanh ghi số 4 bằng ReadRegister nhờ vào machine
					char ch = (char)machine->ReadRegister(4); // Hàm ReadRegister trả về kiểu int -> ép kiểu thành char
					// Hàm gSynchConsole để tiến hành ghi giá trị ký tự char đọc được ra màn hình Console
					gSynchConsole->Write(&ch, 1);
					break;
				}
				case SC_ReadString:{
					// Syscall này tương ứng cho hàm "void ReadString (char* buffer, int length);"
					// Hàm nhập vào 2 tham số: 1 là 1 con trỏ buffer, và 1 length cho số lượng phần tử

					// Tiền hành đọc dữ liệu mảng tham số buffer từ thanh ghi số 4
					int bufferAddress = machine->ReadRegister(4);
					// Đọc tham số thứ 2 là length từ thanh ghi số 5. Tham số này chính là số lượng phần tử người dùng định nghĩa lớn nhất
					int maxlength = machine->ReadRegister(5);

					// Tạo 1 mảng buffer để lưu trữ dữ liệu được đọc từ màng hình
					char *buffer = new char[maxlength + 1]; // +1 for '/0'
					// bytesRead là số phần tử đọc được thực tế từ việc đọc trong màn hình Console và lưu vào mảng buffer
					int bytesRead = gSynchConsole->Read(buffer,maxlength);

					// Gán ký hiệu kết thúc cho chuỗi vừa đọc được từ màn hình '\0'
					buffer[bytesRead] = '\0';

					// Tiến hành kiểm tra: nếu không có byte nào được đọc, thì thông báo cho người dùng chuỗi rỗng
					// Ngược lại thì tiến hành chuyển đổi dữ liệu đọc được từ system space sang user space
					// và lưu vào mảng bufferAddress chính là tham số được truyền vào hàm.
					if(bytesRead == 0){
						DEBUG('a',"\nSC_READSTRING : Invalid - The string is empty\n");
						printf("\nSC_READSTRING : Invalid - The string is empty - \n");
					}
					else {
						int byteCopied = System2User(bufferAddress, bytesRead + 1, buffer); // byteCopied chính là số lượng byte
															// thực sự được chuyển đổi
						if(byteCopied <= 0){
							DEBUG('a',"\nSC_READSTRING : Failed - Some errors have been occured when tranfers from system to user\n");
							printf("\nSC_READSTRING : Failed - Some errors have been occured when tranfers from system to user\n");
						}
					}
					// Thu hồi vùng nhớ cho buffer
					delete buffer;
					break;
				}
				case SC_PrintString:{
					// Syscall này tương ứng với "void PrintString(char* buffer);"
					// Nhập vào 1 tham số buffer dạng con trỏ char chính là mảng ký tự
					// Tiến hành lấy dữ liệu tham số buffer từ thanh ghi số 4 ( thanh ghi tham số)
					int bufferAddress = machine->ReadRegister(4);
					// Tiến hành chuyển đổi dữ liệu mảng ký tự từ user space to system space vào mảng buffer
					char *buffer = User2System(bufferAddress,MaxBufferSize);

					// Lấy ra độ dài của mảng buffer và dùng gSynchConsole để ghi vào màn hình console cho người dùng
					int length = strlen(buffer);
					gSynchConsole->Write(buffer, length + 1);

					break;
				}
        case SC_Exec:{
					//doc ten chuong trinh tu thanh ghi r4
					int virtAddr = machine->ReadRegister(4);
					//chuyen vung nho userspace toi systemspace + lay ten chuong trinh nap vao kernel
					char* name = User2System(virtAddr,MaxFileLength+1);
					//truong hop bi loi va khong mo duoc file
					if(name == NULL){
						DEBUG('a',"\nSC_EXEC: Not enough memory in system");
						printf("\nSC_EXEC : Not enough memory in system");
						machine->WriteRegister(2,-1);
						delete[] name;
						return;
					}
					//kiem tra tinh hop le cua ten file
					if(strlen(name) ==0 || strlen(name) > MaxFileLength){
						DEBUG('a',"\nSC_EXEC : File name is too short or too long.");
						printf("\nSC_EXEC : File name is too short or too long.");
						delete[] name;
						machine->WriteRegister(2,-1);
						return;
					}
					//mo file
					OpenFile* file = fileSystem->Open(name);
					//kiem tra co mo duoc file khong
					if(file == NULL){
						printf("\nSC_EXEC : Can not open this file with name %s.",name);
						machine->WriteRegister(2,-1);
						delete[] name;
						delete file;
						return;
					}
					//truong hop khong co loi
					int id = pTab->ExecUpdate(name);
					machine->WriteRegister(2,id);
					delete[] name;
					delete file;
					break;
        }
        case SC_Join:{
					//doc id tien trinh can join o thanh ghi thu 4
					int id = machine->ReadRegister(4);

					//goi thuc hien JoinUpdate() va luu ket qua vao thanh ghi thu 2
					int res = pTab->JoinUpdate(id);
					if(res < 0){
						printf("\nSC_JOIN : Error in JoinUpdate.");
						machine->WriteRegister(2,-1);
						break;
					}
					machine->WriteRegister(2,res);
					break;
        }
        case SC_Exit:{
					//doc exitStatus tu thanh ghi r4
					int exitState = machine->ReadRegister(4);
					if(exitState != 0){
						break;
					}
					//thuc hien exitUpdate va luu vao thanh ghi r2
					int res = pTab->ExitUpdate(exitState);
					machine->WriteRegister(2,res);
					currentThread->FreeSpace();
					currentThread->Finish();
					break;
        }
        case SC_CreateSemaphore:{
					//doc dia chi name tu thanh ghi r4
					int nameAddr = machine->ReadRegister(4);
					//doc dia chi semval tu thanh ghi r5
					int semval = machine->ReadRegister(5);
					//chuyen vung nho userspace toi systemspace
					char* name = User2System(nameAddr,MaxFileLength + 1);
					if(name == NULL){
						DEBUG('a',"\nSC_CREATESEMAPHORE_ERROR : Not enough memory in system.");
						printf("\nSC_CREATESEMAPHORE_ERROR : Not enough memory in system.");
						machine->WriteRegister(2,-1);
						delete[] name;
						break;
					}
					//tao semaphore
					int res = semTab->Create(name,semval);
					//xu ly loi neu co cho qua trinh tao semaphore
					if(res == -1){
						DEBUG('a',"\nSC_CREATESEMAPHORE_ERROR: Loi khoi tao semaphore.\n");
						printf("\nSC_CREATESEMAPHORE_ERROR : Loi khoi tao semaphore.");
						machine->WriteRegister(2,-1);
						delete[] name;
						break;
					}
					//luu ket qua vao thanh ghi r2
					delete[] name;
					machine->WriteRegister(2,res);

					break;
        }
        case SC_Up:{
					//doc dia chi name tu thanh ghi r4
					int nameAddr = machine->ReadRegister(4);
					//chuyen vung nho userspace toi systemspace
					char* name = User2System(nameAddr,MaxFileLength + 1);
					if(name == NULL){
						DEBUG('a',"\nSC_UP_ERROR : Not enough memory in system.");
						printf("\nSC_UP_ERROR : enough memory in system.");
						machine->WriteRegister(2,-1);
						delete[] name;
						return;
					}
					//goi phuong thuc signal cua lop Stable
					// kiem tra semaphore name co trong bang sTab chua
					int res = semTab->Signal(name);
					if(res == -1){
						DEBUG('a',"\nSC_UP_ERROR : Semaphore name khong tong tai.");
						printf("\nSC_UP_ERROR :Semaphore name khong ton tai.");
						delete[] name;
						machine->WriteRegister(2,-1);
						return;
					}
					//luu ket qua vao thanh ghi r2
					machine->WriteRegister(2,res);
					delete[] name;
					break;
        }
        case SC_Down:{
					//doc dia chi name tu thanh ghi r4
					int nameAddr = machine->ReadRegister(4);
					//chuyen vung nho userspace toi systemspace
					char* name = User2System(nameAddr,MaxFileLength+1);
					if(name == NULL){
						DEBUG('a',"\nSC_DOWN_ERROR : Not enough memory in system.");
						printf("\nSC_DOWN_ERROR : Not enough memory in system.");
						machine->WriteRegister(2,-1);
						delete[] name;
						break;
					}
					//goi phuong thuc cua wait cua lop Stable
					// kiem tra semaphore name co trong bang sTab chua
					int res = semTab->Wait(name);
					if(res == -1){
						DEBUG('a',"\nSC_DOWN_ERROR : Semaphore name khong ton tai.");
						printf("\nSC_DOWN_ERROR : Semaphore name khong ton tai.");
						delete[] name;
						machine->WriteRegister(2,-1);
						break;
					}
					//luu ket qua vao thanh ghi r2
					machine->WriteRegister(2,res);
					delete[] name;
					break;
        }
				default:{
					interrupt->Halt();
					break;
				}
			}
			IncreatePC(); // tang gia tri thanh ghi PC sau moi syscall de tranh truong hop lap vo tan
		}
		default:
			break;
	}
}
