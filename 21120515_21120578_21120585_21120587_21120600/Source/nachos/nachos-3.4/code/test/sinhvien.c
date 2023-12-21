#include "syscall.h"

void main(){
  SpaceId f_sinhvien,f_voinuoc,f_output;   //bien luu id cho file sinhvien, voi nuoc, va ket qua xu ly dong dong du lieu do
  int check = 0;      //bien giup kiem tra cac thao tac
  char ch = 0;
  int size = 0, fPointer = 0;       //fPointer de luu con tro file(luu vi tri file dang doc), size de luu kich thuoc cua file
  int VN_FLAG = 0;              // co VN_FLAG de kiem tra xem da doc toi cuoi file sinhvien chua
  char* temp;
  //==================================
  while(1)
  {
    size = 0;
    Down("sinhvien");       //cho tin hieu goi thuc hien tu ham main

    //mo file sinh vien de doc du lieu o che do chi cho phep doc
    f_sinhvien = Open("sinhvien.txt",1);
    if(f_sinhvien == -1)
    {
      Up("main");   //tro ve tien trinh chinh(main)
      return;
    }
    //doc kich thuoc file sinhvien
    size = Seek(-1,f_sinhvien);
    //tro ve lai dau file de chuan bi doc
    Seek(0,f_sinhvien);
    fPointer = 0;
    //tao file voinuoc de ghi thong tin voi nuoc
    check = Create("voinuoc.txt");
    if(check == -1)     //truong hop tao file that bai
    {
      Close(f_sinhvien);
      Up("main");
      return;
    }
    // mo file voi nuoc o che do doc - ghi
    f_voinuoc = Open("voinuoc.txt",0);
    if(f_voinuoc == -1)   // truong hop mo file that bai
    {
      Close(f_sinhvien);
      Up("main");
      return;
    }


    //ghi thong tin dung tich nuoc tu file sinh vien va tien hanh xu ly
    while(fPointer < size){
      VN_FLAG = 0;
      Read(&ch,1,f_sinhvien);
      if(ch != ' ')     // gia su la sau khi doc duoc ki tu trong thi chung ta da co duoc dung tich nuoc va tien hanh rot nuoc
      {
        Write(&ch,1,f_voinuoc);
        //mo file output de ghi lai ket qua
        f_output = Open("output.txt",0);
        if(f_output == -1 )
        {
          Close(f_sinhvien);
          Up("main");
          return;
        }
        //dua con tro file ve cuoi de ghi thong tin
        Seek(-1,f_output);
        Write(&ch,1,f_output);
        Close(f_output);
      }
      else
      {
        VN_FLAG = 1;
      }
      //neu da doc toi cuoi file thi them dau hieu ket thuc de thong bao
      if(fPointer == size - 1){
        VN_FLAG = 1;
        Write("*",1,f_voinuoc);     //them dau hieu ket thuc *
      }

      //xu ly thong tin voi nuoc
      if(VN_FLAG == 1){
          Close(f_voinuoc);
          Up("voinuoc");        // goi tien trinh voi nuoc de thuc hien
          Down("sinhvien");   //cho tien trinh sinhvien dung lai de cho
          //tao lai file voinuoc.txt de thuc hien ghi lai gia tri dung tich nuoc
          if(Create("voinuoc.txt") == -1){
            Close(f_sinhvien);
            Up("main");
            return;
          }
          f_voinuoc = Open("voinuoc.txt",0);
          if(f_voinuoc == -1){
            Close(f_sinhvien);
            Up("main");
            return;
          }
      }
      fPointer++;     // tang gia tri cho con tro file
    }
    Up("main");     // tro ve ham chinh cua chuong trinh
  }
  return;
}
