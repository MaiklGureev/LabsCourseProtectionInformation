// ZI4.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#define _WIN32_WINNT 0x0400
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

#define D_SCL_SECURE_NO_WARNING
#include "wincrypt.h"
#include <iostream>
#include <string>
#include<cstdlib> 
#include<vector> 
#include<mutex> 
#include<thread> 
#include<chrono>
#include <clocale>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
using namespace std;

const int elementsCount = 10;
DWORD bytesback00;
BYTE* bData00;


char *szPassword = "PASSWORD"; // Пароль, для которого считаем хеш, а потом генерируем ключ
char *szPassword1= new char[1024];




void sybA(mutex& m,int& r) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HCRYPTPROV hCryptProv; // Хэндл криптопровайдера
	HCRYPTHASH hCryptHash; // Хеш-объект для алгоритма MD4
	DWORD cryptBlockSize; // Длина данных
	DWORD bytesback; // Длина новых данных
	HCRYPTKEY hCryptKey; // Ключ для шифрования

	
	m.lock();
	SetConsoleTextAttribute(hConsole, 13);
	cout << "Subject A: Posilaem zapros" << "\n";
	m.unlock();

	this_thread::sleep_for(std::chrono::milliseconds(40));

	m.lock();
	SetConsoleTextAttribute(hConsole, 13);
	cout << "Subject A: Poly4ennoe rand 4islo = "<< r << "\n";

	CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0); // Запросить контекст CSP
	CryptCreateHash(hCryptProv, CALG_MD4, 0, 0, &hCryptHash); // Создать хеш-объект
	CryptHashData(hCryptHash, (BYTE*)szPassword1, strlen(szPassword1), 0); // Посчитать хеш от пароля
	CryptDeriveKey(hCryptProv, CALG_DES, hCryptHash, 0, &hCryptKey); // Ключ для DES из хеша
	cout << "Subject A: Generiryem na osnovanii P klych K(P)" << "\n";

	cout <<"Subject A: Hash :=: " << (char*)hCryptHash << "\n";
	cout << "Subject A: Key :=: " << (char*)hCryptKey << "\n";

	
	int t = r;
	int count=0;
	while (t) {
		t /= 10;
		count++;
	}


	int a = r;   //исходное число
	int radix = 10;  //система счисления
	char buffer[10]; //результат
	char *p;  //указатель на результат
	_itoa_s(a, buffer, radix);


	char* datain=buffer;
	

	int lendatain = count;
	cryptBlockSize = lendatain;
	bytesback = lendatain;
	CryptEncrypt(hCryptKey, NULL, TRUE, 0, (BYTE *)datain, &cryptBlockSize, 0); // Попытаться с 0-м буфером
	BYTE* bData = new BYTE[cryptBlockSize]; // Буфер реально требуемой длины
	memcpy(bData, datain, lendatain); // Скопировать данные в новый буфер
	CryptEncrypt(hCryptKey, NULL, TRUE, 0, bData, &bytesback, cryptBlockSize); // Шифруем
	

	bytesback00 = bytesback;
	bData00 = bData;
	
	
	cout << "Subject A: IIIifryem R kly4om K  i poly4aem Y=F(R,K) "  << "\n";	
	cout << "Subject A: Y=F(R,K):=:  " << (char*)bData << "\n";
	cout << "Subject A: Posilaem  Y=F(R,K) " << "\n";

// Всех закрываем, убиваем и освобождаем
	CryptDestroyKey(hCryptKey); CryptDestroyHash(hCryptHash); CryptReleaseContext(hCryptProv, 0); 

	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(40));



}

void sybB(mutex& m,int& r) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HCRYPTPROV hCryptProv; // Хэндл криптопровайдера
	HCRYPTHASH hCryptHash; // Хеш-объект для алгоритма MD4
	DWORD cryptBlockSize; // Длина данных
	DWORD bytesbackY; // Длина новых данных
	HCRYPTKEY hCryptKey; // Ключ для шифрования
	BYTE* y;

	m.lock();
	SetConsoleTextAttribute(hConsole, 14);
	cout << "Subject B: Zapros poly4en" << "\n";
	srand((unsigned)time(NULL));
	r = rand();
	cout << "Subject B: Generatia randomnogo 4isla = " << r << "\n";
	cout << "Subject B: Posilaem randomnogo 4islo " << "\n";


	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	m.lock();
	SetConsoleTextAttribute(hConsole, 14);
	cout << "Subject B: Poly4aem  Y=F(R,K) " << "\n";
	cout << "Subject B: Poly4  Y=F(R,K) :=: " << (char*)bData00 << "\n";


	CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0); // Запросить контекст CSP
	CryptCreateHash(hCryptProv, CALG_MD4, 0, 0, &hCryptHash); // Создать хеш-объект
	CryptHashData(hCryptHash, (BYTE*)szPassword, strlen(szPassword), 0); // Посчитать хеш от пароля
	CryptDeriveKey(hCryptProv, CALG_DES, hCryptHash, 0, &hCryptKey); // Ключ для DES из хеша
	cout << "Subject B: Generiryem na osnovanii italonogo Q klych K(Q)" << "\n";

	cout << "Subject B: Hash :=: " << (char*)hCryptHash << "\n";
	cout << "Subject B: Key :=: " << (char*)hCryptKey << "\n";
		

	//===========Расшифровка
	CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
	CryptCreateHash(hCryptProv, CALG_MD4, 0, 0, &hCryptHash);
	CryptHashData(hCryptHash, (BYTE*)szPassword, strlen(szPassword), 0);
	CryptDeriveKey(hCryptProv, CALG_DES, hCryptHash, 0, &hCryptKey);

	CryptDecrypt(hCryptKey, NULL, TRUE, 0, (BYTE *)bData00, &bytesback00);
	bData00[bytesback00] = 0;

	// Всех закрываем, убиваем и освобождаем
	CryptDestroyKey(hCryptKey); 	CryptDestroyHash(hCryptHash); 	CryptReleaseContext(hCryptProv, 0);
	cout << "Subject B: Rashifrovivaem X=F(Y,K)" << bData00 <<"\n";
	cout << "Subject B: Rashif:=: " << bData00 << "\n";

	int a = r;   //исходное число
	int radix = 10;  //система счисления
	char buffer[10]; //результат
	char *p;  //указатель на результат
	_itoa_s(a, buffer, radix);


	char* dt = buffer;
	char* ii = (char *)bData00;



	if (strcmp(dt, (char*)bData00) == 0) {
		SetConsoleTextAttribute(hConsole, 10);
		printf("Subject B: Parol veren\n");
	}
	else {
		SetConsoleTextAttribute(hConsole, 12);
		printf("Subject B: Parol NE veren\n");
	}
	SetConsoleTextAttribute(hConsole, 15);
		
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));



}

int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	
	int k = 8;
	while (k < 10)
	{
		
		SetConsoleTextAttribute(hConsole, 15);
		cout << "Italon parol: " << szPassword << "\n";
		cout << "Vvedite parrol: ";
		cin.getline(szPassword1, 1024);

	mutex m;

	int randomnoe=0;

	DWORD bytesb;
	BYTE* y;
	thread sybA_thread(sybA, ref(m),ref(randomnoe));
	thread sybB_thread(sybB, ref(m),ref(randomnoe));
	if (sybA_thread.joinable()) sybA_thread.join();
	if (sybB_thread.joinable()) sybB_thread.join();
	cout << "===================================================================" << "\n";

	k++;
	}

	system("pause");
	return 0;
}