// zi4lab1.cpp: определяет точку входа для консольного приложения.
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
//void MyHandleError(char *s);
//HANDLE event1;
//HANDLE event2;
//HANDLE event3;
//HANDLE hThrA, hThrB;

void MyHandleError(char *s);
//void Get_And_Print_Hash(HCRYPTHASH hHash);
//
//HANDLE hEventCode, hEventDecode;
////HANDLE hWritePipe, hReadPipe;
//HANDLE hReadPipe = CreateFileA("C:\\Users\\admin\\Desktop\\Laba_4\\testread.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//HANDLE hWritePipe = CreateFileA("C:\\Users\\admin\\Desktop\\Laba_4\\testwrite.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//DWORD dw = 0;
char *szPassword = "123qwe"; // Пароль, для которого считаем хеш, а потом генерируем ключ
char *szPassword1 = new char[1024];
string s;

void sybA(mutex& m, int& r) {
	//	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HCRYPTPROV hCryptProv; // Хэндл криптопровайдера
	HCRYPTHASH hCryptHash; // Хеш-объект для алгоритма MD5
	DWORD cryptBlockSize; // Длина данных
	DWORD bytesback; // Длина новых данных
	HCRYPTKEY hCryptKey; // Ключ для шифрования

	m.lock();
	cout << "Subject A: Posilaem zapros" << "\n";
	m.unlock();

	std::this_thread::sleep_for(std::chrono::milliseconds(40));

	m.lock();
	cout << "Subject A: Polychennoe rand chislo = " << r << "\n";
	CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0); // Запросить контекст CSP
	CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hCryptHash); // Создать хеш-объект
	CryptHashData(hCryptHash, (BYTE*)szPassword1, strlen(szPassword1), 0); // Посчитать хеш от пароля
	CryptDeriveKey(hCryptProv, CALG_RC4, hCryptHash, 0, &hCryptKey); // Ключ для RC4 из хеша
	//Get_And_Print_Hash(hCryptHash);

	//cout << "Subject A: Generiruem na osnovanii P klych K(P)" << "\n";
	//cout << "Subject A: Hash :=: " << (BYTE*)hCryptHash << "\n";




	DWORD hashSize = 16;
	BYTE* hashValue = new BYTE[hashSize];
	CryptGetHashParam(hCryptHash, HP_HASHVAL, hashValue, &hashSize, 0);
	cout << "Subject A: Generiruem na osnovanii P klych K(P)" << "\n";
	cout << "Subject A: Hash :=: " << (BYTE*)hCryptHash << "\n";


	
	cout << "Subject A: Hash normalniy :=: ";
	//cout << "Subject A: Key :=: " << (char*)hCryptKey << "\n"; //
	for (int i = 0; i < hashSize; i++)
		printf("%X%X", hashValue[i] >> 4, 0xF & hashValue[i]);
	cout << endl;



	//string s;
	//cout << "Subject B: Hash normalniy :=: ";
	//for (int i = 0; i < bytesback00; i++)
	//	s = s + "%X%X", bData00[i] >> 4, 0xF & bData00[i];

	////printf("%X%X", bData00[i] >> 4, 0xF & bData00[i]);
	//cout << s << endl;
	//cout << endl;



	cout << "Subject A: Key :=: " << (BYTE*)hCryptKey << "\n"; //
	int t = r;
	int count = 0;
	while (t) {
		t /= 10;
		count++;
	}
	int a = r;   //исходное число
	int radix = 10;  //система счисления
	char buffer[10]; //результат
	char *p;  //указатель на результат
	_itoa_s(a, buffer, radix);
	char* datain = buffer;
	int lendatain = count;
	//cout << "Subject A: lendatain " << lendatain << "\n";
	cryptBlockSize = lendatain;
	bytesback = lendatain;
	//cout << "Subject A: lendatain " << lendatain<<" byteback " << bytesback<< "\n";
	CryptEncrypt(hCryptKey, NULL, TRUE, 0, (BYTE *)datain, &cryptBlockSize, 0); // Попытаться с 0-м буфером
	BYTE* bData = new BYTE[cryptBlockSize]; // Буфер реально требуемой длины
	memcpy(bData, datain, lendatain); // Скопировать данные в новый буфер
	CryptEncrypt(hCryptKey, NULL, TRUE, 0, bData, &bytesback, cryptBlockSize); // Шифруем
	bytesback00 = bytesback;
	bData00 = bData;
	cout << "Subject A: Shifruem R klychom K  i poluchaem Y=F(R,K) " << "\n";
	cout << "Subject A: Y=F(R,K):=:  " << (BYTE*)bData << "\n";
	cout << "Subject A: Posilaem  Y=F(R,K) " << "\n";
	// Всех закрываем, убиваем и освобождаем
	CryptDestroyKey(hCryptKey); CryptDestroyHash(hCryptHash); CryptReleaseContext(hCryptProv, 0);
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(40));
}


//DWORD WINAPI tDecoder(void *q) {
//
//	HCRYPTPROV hCryptProv;
//	CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
//
//	while (true) {

////4. Запись результата в файл 	
//WriteFile(hWritePipe, &hashCryptSize, sizeof(DWORD), &dw, NULL);
//WriteFile(hWritePipe, hashCrypt, sizeof(BYTE) * hashCryptSize, &dw, NULL);
//
//WriteFile(hWritePipe, &dataCryptSize, sizeof(DWORD), &dw, NULL);
//WriteFile(hWritePipe, dataCrypt, sizeof(BYTE) * dataCryptSize, &dw, NULL);
//
//CryptDestroyKey(hCryptKey);
//CryptDestroyHash(hCryptHash);
//CryptReleaseContext(hCryptProv, 0);
//cout << "Сообщение закодировано" << endl;
//SetEvent(hEventDecode);
//ResetEvent(hEventCode);
//	}
//}




void sybB(mutex& m, int& r) {
	//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HCRYPTPROV hCryptProv; // Хэндл криптопровайдера
	HCRYPTHASH hCryptHash; // Хеш-объект для алгоритма MD5
	DWORD cryptBlockSize; // Длина данных
	DWORD bytesbackY; // Длина новых данных
	HCRYPTKEY hCryptKey; // Ключ для шифрования
	BYTE* y;
	m.lock();
	cout << "Subject B: Zapros polychen" << "\n";
	srand((unsigned)time(NULL));
	r = rand();
	cout << "Subject B: Generacia randomnogo chisla = " << r << "\n";
	cout << "Subject B: Posilaem randomnoe chislo " << "\n";
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	m.lock();

	cout << "Subject B: Polychaem  Y=F(R,K) " << "\n";
	//cout << "Subject B: Polych  Y=F(R,K) :=: " << (char*)bData00 << "\n";
	CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0); // Запросить контекст CSP
	CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hCryptHash); // Создать хеш-объект
	CryptHashData(hCryptHash, (BYTE*)szPassword, strlen(szPassword), 0); // Посчитать хеш от пароля
	CryptDeriveKey(hCryptProv, CALG_RC4, hCryptHash, 0, &hCryptKey); // Ключ для RC4 из хеша
	cout << "Subject B: Generiruem na osnovanii italonogo Q klych K(Q) " << "\n";
	cout << "Subject B: Hash :=: " << (BYTE*)hCryptHash << "\n";

	//CryptDecrypt(hCryptKey, NULL, TRUE, 0, (BYTE*)cryptHash, &cryptHashSize);
	//cryptHash[cryptHashSize] = '\0 ';
	//string s;

	DWORD hashSize = 16;
	BYTE* hashValue = new BYTE[hashSize];
	CryptGetHashParam(hCryptHash, HP_HASHVAL, hashValue, &hashSize, 0);


	cout << "Subject B: Hash normalniy :=: ";
	for (int i = 0; i < hashSize; i++)
		//s = s + "%X%X", bData00[i] >> 4, 0xF & bData00[i];
	
		printf("%X%X", hashValue[i] >> 4, 0xF & hashValue[i]);
	//cout << s << endl;
	cout << endl;
	

	cout << "Subject B: Key :=: " << (BYTE*)hCryptKey << "\n";
	//===========Расшифровка
	CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
	CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hCryptHash);
	CryptHashData(hCryptHash, (BYTE*)szPassword, strlen(szPassword), 0);
	CryptDeriveKey(hCryptProv, CALG_RC4, hCryptHash, 0, &hCryptKey);
	CryptDecrypt(hCryptKey, NULL, TRUE, 0, (BYTE *)bData00, &bytesback00);
	bData00[bytesback00] = 0;
	// Всех закрываем, убиваем и освобождаем
	CryptDestroyKey(hCryptKey); 	CryptDestroyHash(hCryptHash); 	CryptReleaseContext(hCryptProv, 0);
	cout << "Subject B: Rashifrovivaem X=F(Y,K)" << bData00 << "\n";
	cout << "Subject B: Rashif:=: " << bData00 << "\n";

	////CryptDecrypt(hCryptKey, NULL, TRUE, 0, (BYTE*)cryptHash, &cryptHashSize);
	////cryptHash[cryptHashSize] = '\0';
	//cout << "Расшифрованный хеш: ";
	//for (int i = 0; i < bytesback00; i++)
	//	printf("%X%X", bData00[i] >> 4, 0xF & bData00[i]);
	//cout << endl;

	int a = r;   //исходное число
	int radix = 10;  //система счисления
	char buffer[10]; //результат
	char *p;  //указатель на результат
	_itoa_s(a, buffer, radix);
	char* dt = buffer;
	char* ii = (char *)bData00;
	if (strcmp(dt, (char*)bData00) == 0) {
		printf("Subject B: Parol veren\n");
	}
	else {
		printf("Subject B: Parol NE veren\n");
	}
	m.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int k = 8;
	SetConsoleTextAttribute(hConsole, 15);
	cout << "Italon parol: " << szPassword << "\n";
	cout << "Vvedite parrol: ";
	cin.getline(szPassword1, 1024);
	mutex m;
	int randomnoe = 0;
	DWORD bytesb;
	BYTE* y;
	thread sybA_thread(sybA, ref(m), ref(randomnoe));
	thread sybB_thread(sybB, ref(m), ref(randomnoe));
	if (sybA_thread.joinable()) sybA_thread.join();
	if (sybB_thread.joinable()) sybB_thread.join();
	cout << "===================================================================" << "\n";
	system("pause");
	return 0;
}

//void Get_And_Print_Hash(HCRYPTHASH hOHash)
//{
//	//--------------------------------------------------------------------
//	// Declare and initialize local variables.
//	HCRYPTHASH   hHash;
//	BYTE         *pbHash;
//	DWORD        dwHashLen;
//	DWORD        dwHashLenSize = sizeof(DWORD);
//	DWORD        i;
//
//	//--------------------------------------------------------------------
//	// Duplicate the hash passed in.
//	// Полученный хеш: duplicated to leave the original hash intact.
//
//	if (CryptDuplicateHash(
//		hOHash,
//		NULL,
//		0,
//		&hHash))
//	{
//		// It worked. Do nothing.
//	}
//	else
//	{
//		MyHandleError("Error during CryptDuplicateHash.");
//	}
//
//	if (CryptGetHashParam(
//		hHash,
//		HP_HASHSIZE,
//		(BYTE *)&dwHashLen,
//		&dwHashLenSize,
//		0))
//	{
//		// It worked. Do nothing.
//	}
//	else
//	{
//		MyHandleError("CryptGetHashParam failed to get size.");
//	}
//
//	if (pbHash = (BYTE*)malloc(dwHashLen))
//	{
//		// It worked. Do nothing.
//	}
//	else
//	{
//		MyHandleError("Allocation failed.");
//	}
//
//	if (CryptGetHashParam(
//		hHash,
//		HP_HASHVAL,
//		pbHash,
//		&dwHashLen,
//		0))
//	{
//		// Print the hash value.
//		printf("poluchenniy hash::  ");
//		for (i = 0; i < dwHashLen; i++)
//		{
//			printf("%02x ", pbHash[i]);
//		}
//		printf("\n");
//	}
//	else
//	{
//		MyHandleError("Error during reading hash value.");
//	}
//
//	free(pbHash);
//	if (CryptDestroyHash(hHash))
//	{
//		// It worked. Do nothing.
//	}
//	else
//	{
//		MyHandleError("ERROR - CryptDestroyHash");
//	}
//} // end Get_And_Print_Hash
//
//
//
//void MyHandleError(char *s)
//{
//	fprintf(stderr, "An error occurred in running the program. \n");
//	fprintf(stderr, "%s\n", s);
//	fprintf(stderr, "Error number %x.\n", GetLastError());
//	fprintf(stderr, "Program terminating. \n");
//	exit(1);
//} // end MyHandleError



//int main()
//{
//
//	setlocale(LC_ALL, "RUSSIAN");
//
//	event1 = CreateEvent(NULL, FALSE, FALSE, NULL);
//	event2 = CreateEvent(NULL, FALSE, TRUE, NULL);
//	event3 = CreateEvent(NULL, FALSE, FALSE, NULL);
//	hThrA = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadA, NULL, 0, &uThrID);
//	hThrB = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadB, NULL, 0, &uThrID);
//	WaitForSingleObject(event3, INFINITE);
//	TerminateThread(threadA, 0);
//	TerminateThread(threadB, 0);
//	system("pause");
//	return 0;
//}

