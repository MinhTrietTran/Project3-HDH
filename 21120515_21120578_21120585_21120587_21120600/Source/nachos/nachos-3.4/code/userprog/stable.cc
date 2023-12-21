//STable.cc

#include "stable.h"
#include "system.h"

STable::STable(){
  this->bm = new BitMap(MAX_SEMAPHORE);
  for(int i = 0; i < MAX_SEMAPHORE; ++i){
    semTab[i] = NULL;
  }
}


//destructor
STable::~STable(){
  delete bm;
  bm = NULL;
  for(int i = 0; i < MAX_SEMAPHORE; ++i){
    delete semTab[i];
    semTab[i] = NULL;
  }
}


int STable::Create(char* name, int init ){

  for(int i=0; i<MAX_SEMAPHORE; i++)
	{
    //kiem tra vi tri thu i da duoc nap semaphore hay chua
		if(bm->Test(i))
		{
      //kiem tra ten semaphore cos trung voi ten trong semTab vi tri thu i khong.
			if(strcmp(name, semTab[i]->GetName()) == 0)
			{
        printf("\nSTABLE_ERROR :  Name is extist");
				return -1;
			}
		}

	}

  // tim vi tri trong
  int freeSpace = FindFreeSlot();
  //truong hop khong tim thay
  if(freeSpace < 0){
    printf("\nSTABLE_ERROR: Khong tim thay vi tri trong");
    return -1;
  }
  else{     // truong hop tim duoc vi tri trong
    this->semTab[freeSpace] = new Sem(name,init);
  }
}


int STable::Wait(char* name){
  for(int i = 0; i < MAX_SEMAPHORE; ++i){
    // kiem tra tinh trang o thi i da nap semaphore chua
    if(bm->Test(i)){
      //so sanh ten trong tham so dau vao
      if(strcmp(name,semTab[i]->GetName()) == 0){
        semTab[i]->Wait();
        return 0;
      }
    }
  }
  printf("\nERROR: semaphore khong ton tai");
  return -1;

}


int STable::Signal(char* name){
  for(int i = 0; i < MAX_SEMAPHORE; ++i){
    // kiem tra tinh trang o thi i da nap semaphore chua
    if(bm->Test(i)){
      //so sanh ten trong tham so dau vao
      if(strcmp(name,semTab[i]->GetName()) == 0){
        semTab[i]->Signal();
        return 0;
      }
    }
  }
  printf("\nERROR: semaphore khong ton tai");
  return -1;
}


int STable::FindFreeSlot(){
  this->bm->Find();
}
