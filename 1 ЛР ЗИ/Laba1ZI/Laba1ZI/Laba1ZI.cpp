// Laba1ZI.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

int i=0;
int flag=0;
unsigned char etalon[]={"xyz"};

int check_pass()
{
unsigned char password [4];
printf ("Enter password > ");
scanf("%s",password);
while (etalon[i]!=0) if (password[i]!=etalon[i++]) flag=1;
return flag;
}

int _tmain(int argc, _TCHAR* argv[])
{
	_asm{cpuid};
	if (check_pass()==0)
		printf("OK");
	else
		printf("Bad password");
	return 0;
}

