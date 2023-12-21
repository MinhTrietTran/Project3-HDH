// Stable.h

// #ifndef SEM_H
// #define SEM_H
// //sem.h





#ifndef STABLE_H
#define STABLE_H
#include "synch.h"
#include "bitmap.h"
#define MAX_SEMAPHORE 10


class Sem
{
private:
	char name[50];		// Ten cua semaphore
	Semaphore *sem;		// Tao semaphore de quan ly
public:
	// Khoi tao doi tuong Sem. Gan gia tri ban dau la null
	// Nho khoi tao sem su dung
	Sem(char* na, int i)
	{
		strcpy(this->name, na);
		sem = new Semaphore(this->name, i);
	}

	~Sem()
	{
		if(sem)
			delete sem;
	}

	void Wait()
	{
		sem->P();	// Down(sem)
	}

	void Signal()
	{
		sem->V();	// Up(sem)
	}

	char* GetName()
	{
		return this->name;
	}
};
// Lop Sem dung de quan ly semaphore.
class STable{
private:
  BitMap *bm;                     // quan ly slot trong
  Sem *semTab[MAX_SEMAPHORE];     //quan ly toi da 10 doi tuong semaphore
public:
  // khoi tao doi tuong Sem de quan ly 10 semaphore, gan gia tri ban dau la null
  // phai khoi tao bm de su dung
  //constructor
  STable();
  //destructor
  ~STable();
  int Create(char* name, int init );      // kiem tra semaphore "name" da ton tai chua. Neu chua thi tao Semaphore moi va nguoc lai bao loi
  int Wait(char* name);                   // neu ton tai Semaphore "name" thi goi this->P() de thuc thi. Nguoc lai thi bao loi
  int Signal(char* name);                 // neu ton tai Semaphore "name" thi goi this->V() de thuc thi. Nguoc lai thi bao loi
  int FindFreeSlot();               // tim slot trong
};

#endif // STABLE_H
