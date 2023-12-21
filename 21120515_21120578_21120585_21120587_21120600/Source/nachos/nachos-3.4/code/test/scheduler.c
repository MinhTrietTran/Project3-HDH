#include "syscall.h"


void main()
{
	int exe1,execsv,execvn;
	exe1 = Exec("./test/main");
	Join(exe1);
	return;
}
