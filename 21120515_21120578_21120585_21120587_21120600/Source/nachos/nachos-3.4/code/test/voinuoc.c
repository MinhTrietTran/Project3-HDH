#include "syscall.h"
void main(){
  int check = 0;      //bien de kiem tra cac thao tac trong chuong trinh
  SpaceId f_voinuoc,f_output;    //bien de luu id file voi nuoc
  char ch = '0';
  int v1 = 0,v2 = 0 ;   // luu trang thai voi nuoc so 1 va so 2
  int l = 0;    //luu dung tich nuoc
  int done = 0;   //dung de danh dau khi ma toan bo sinh vien da lay duoc nuoc

  //=====================================================

  while(1){     //vong lap nay de dam bao cho tien trinh voi nuoc khong bi exit sau khi xu ly xong o tung thoi diem xet
    while(1){
      Down("voinuoc");        // doi tin hieu goi thuc hien tu tien trinh sinh vien
      ch=0;
      //no file voi nuoc o che do chi doc de doc dung tich nuoc
      f_voinuoc = Open("voinuoc.txt",1);
      if(f_voinuoc == -1){
        Up("sinhvien");
        return;
      }
      //doc dung tich nuoc tu file voi nuoc
      done = 0;
      l = 0;
      while(1){
        if(Read(&ch,1,f_voinuoc) == -2){      //truong hop doc toi cuoi fle
          Close(f_voinuoc);
          break;
        }

        if(ch != '*')             //truong hop gap ky tu la so
          l = l * 10 + (ch - 48);
        else                    //truong hop gap ky tu ket thuc thoi diem xet
        {
          Close(f_voinuoc);
          done = 1;
          break;
        }

      }
      // mo file output de ghi ket qua
      f_output = Open("output.txt",0);
      if(f_output == -1){
        Close(f_voinuoc);
        Up("sinhvien");
        return;
      }
      //dua con tro file output ve cuoi file de ghi
      Seek(-1,f_output);
      //kiem tra neu luong nuoc la 1 so khac 0 moi cho phep thuc hien
      if(l != 0){
        if(v1 <= v2){ //voi nuoc 1 dang trong
          v1 += l;
          Write("_1",2,f_output);
        }
        else{     // voi nuoc 2 dang trong
          v2 += l;
          Write("_2",2,f_output);
        }
      }
      if(done == 1)  // truong hop ket thuc file thi tra ve trang thai ban dau
      {
        v1 = 0;
        v2 = 0;
        Write("\r\n",2,f_output);   // ghi them dau hieu ket thuc file output
        Close(f_output);
        Up("sinhvien");
        break;
      }
      else
      {
        Write("   ",3,f_output);    //cho thong tin tiep theo tu tien trinh sinhvien
      }
      Close(f_output);
      Up("sinhvien");
    }
  }
  return;
}
