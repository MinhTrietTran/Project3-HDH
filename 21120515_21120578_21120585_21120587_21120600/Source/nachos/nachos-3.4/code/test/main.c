#include "syscall.h"
#include "copyright.h"


int main(){
  SpaceId f_input,f_sinhvien,f_output;      // cac file luu thong tin input, thong tin sinh vien tam thoi,  va thong in output
  int check =0;    //bien voi chuc nang kiem tra
  int N =0;  //bien luu so luong thoi diem test
  char ch ='0';
  int i =0;
  //===================================

  //tao semaphore quan ly cac tien trinh
  check = CreateSemaphore("main",0);
  if(check == -1) return 1;
  check = CreateSemaphore("sinhvien",0);
  if(check == -1) return 1;
  check == CreateSemaphore("voinuoc",0);
  if(check == -1) return 1;


  //mo file output chi cho phep doc
  f_input = Open("input.txt",1);
  if(f_input == -1){
    PrintString("\nLoi mo file de doc input\n");
    return 1;
  }
  //tao file de ghi ket qua cuoi cung
  check = Create("output.txt");
  if(check == -1){
    PrintString("\nLoi tao file de ghi ket qua\n");
    return 1;
  }


  //doc so luong thoi diem test
  while(1)
  {
    Read(&ch,1,f_input);
    if(ch != '\n')
    {
      if(ch >= '0' && ch <= '9')
        N = N * 10 + (ch - 48);
    }
    else
      break;
  }
  // goi thuc thi chuong trinh sinh vien(sinhvien.c)
  check = Exec("./test/sinhvien");
  if(check == -1){
    PrintString("\nFailed to exec sinhvien");
    Close(f_input);
    return 1;
  }
  //goi thuc thi chuong trinh voi nuoc (voinuoc.c)
  check = Exec("./test/voinuoc");
  if(check == -1){
    PrintString("\nFailed to exec voinuoc");
    Close(f_input);
    return 1;
  }


  while(N--){
    //tao file sinhvien
    check = Create("sinhvien.txt");
    if(check == -1){
      PrintString("\nLoi tao file de ghi du lieu(sinh vien).");
      Close(f_input);
      return 1;
    }
    //mo file sinh vien vua tao voi che do doc-ghi
    f_sinhvien = Open("sinhvien.txt",0);
    if(f_sinhvien == -1){
      PrintString("Loi mo file de ghi du lieu(sinh vien).");
      Close(f_input);
      return 1;
    }
    //doc tung dong du lieu
    while(1){
      if(Read(&ch,1,f_input) < 1)     //truong hop da doc den cuoi file
        break;
      if(ch != '\n')                //kiem tra da doc den cuoi dong do hay chua
        Write(&ch,1,f_sinhvien);
      else
        break;
    }
    Close(f_sinhvien);
    //bat dau cho tien trinh sinh vien hoat dong
    Up("sinhvien");
    //cho tien trinh chinh cho tien trinh sinh vien hoat dong
    Down("main");
  }
  Close(f_input);
  PrintString("\nSUCCESS - OPEN FILE OUTPUT TO SEE THE FINAL OUTPUT!!!");
  Halt();
  return 1;
}
