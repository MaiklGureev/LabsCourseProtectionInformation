#include "stdafx.h"
#include <tchar.h>
#include <locale>
#include <stdlib.h>
#include <string.h> 
#include <windows.h>
#include <stdio.h>
#define _WIN32_WINNT 0x0400
#include "wincrypt.h"
#include <iostream>
using namespace std;
HANDLE hEventCode,hEventDecode;
//HANDLE hWritePipe, hReadPipe;
HANDLE hReadPipe = CreateFileA("C:\\Users\\Антон\\Desktop\\testread.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
HANDLE hWritePipe = CreateFileA("C:\\Users\\Антон\\Desktop\\testwrite.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//LPDWORD temp;
DWORD dw = 0;



DWORD WINAPI tCoder(void *q) {
	HCRYPTPROV hCryptProv;
	CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);

	while (true) {
		WaitForSingleObject(hEventCode, INFINITE);

		cout << "ID:";
		cout << endl;
		cout << GetCurrentThreadId();
		cout << endl;

		//1. Вычисление хеша данных 
		char* data = new char[1000];
		cout << "Введите сообщение: ";
		std::string s;
	    std::getline(std::cin, s); 
	    data = _strdup(s.c_str());
		int dataSize = strlen(data);

		char* password = new char[1000];
		cout << "Введите пароль: ";
		cin >> password;

		HCRYPTHASH hCryptHash;
		CryptCreateHash(hCryptProv, CALG_MD5, NULL, 0, &hCryptHash);
		CryptHashData(hCryptHash, (BYTE*)data, dataSize, CRYPT_USERDATA);
		DWORD hashSize = 16;
		BYTE* hashValue = new BYTE[hashSize];
		CryptGetHashParam(hCryptHash, HP_HASHVAL, hashValue, &hashSize, 0);
		cout << "# MD5 хеш: ";
		for (int i = 0; i < hashSize; i++)
			printf("%X%X", hashValue[i] >> 4, 0xF & hashValue[i]);
		cout << endl;

		//2. Создание ключа на основе пароля 
		CryptCreateHash(hCryptProv, CALG_MD5, NULL, 0, &hCryptHash);
		CryptHashData(hCryptHash, (BYTE*)password, strlen(password), CRYPT_USERDATA);
		HCRYPTKEY hCryptKey;
		CryptDeriveKey(hCryptProv, CALG_RC4, hCryptHash, 0, &hCryptKey);

		//3. Шифрование данных RC4 
		DWORD hashCryptSize = hashSize;
		CryptEncrypt(hCryptKey, NULL, TRUE, 0, (BYTE*)hashValue, &hashCryptSize, 0);
		BYTE* hashCrypt = new BYTE[hashCryptSize];
		memcpy(hashCrypt, hashValue, hashSize);
		DWORD bytesback = hashSize;
		CryptEncrypt(hCryptKey, NULL, true, 0, (BYTE*)hashCrypt, &bytesback, hashCryptSize);

		DWORD dataCryptSize = dataSize;
		CryptEncrypt(hCryptKey, NULL, TRUE, 0, (BYTE*)data, &dataCryptSize, 0);
		BYTE* dataCrypt = new BYTE[dataCryptSize];
		memcpy(dataCrypt, data, dataSize); //копируем данные 
		bytesback = dataSize; //количество шифруемых байтов 
		CryptEncrypt(hCryptKey, NULL, true, 0, (BYTE*)dataCrypt, &bytesback, dataCryptSize);
		
		
		//4. Запись результата в файл 	
		WriteFile(hWritePipe, &hashCryptSize, sizeof(DWORD), &dw, NULL);
		WriteFile(hWritePipe, hashCrypt, sizeof(BYTE) * hashCryptSize, &dw, NULL);

		WriteFile(hWritePipe, &dataCryptSize, sizeof(DWORD), &dw, NULL);
		WriteFile(hWritePipe, dataCrypt, sizeof(BYTE) * dataCryptSize, &dw, NULL);

		CryptDestroyKey(hCryptKey);
		CryptDestroyHash(hCryptHash);
		CryptReleaseContext(hCryptProv, 0);
		cout << "Сообщение закодировано" << endl;
		SetEvent(hEventDecode);
		ResetEvent(hEventCode);
	}
}
DWORD WINAPI tDecoder(void *q) {	
	
	HCRYPTPROV hCryptProv;
	CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);

	while (true) {
		WaitForSingleObject(hEventDecode, INFINITE);

		cout << "ID:";
		cout << endl;
		cout << GetCurrentThreadId();
		cout << endl;

		cout << "Декодирование запущено\n\r";

		//1. Чтение данных и хеша 
		DWORD cryptHashSize;
		ReadFile(hReadPipe, &cryptHashSize, sizeof(DWORD), &dw, NULL);
		BYTE* cryptHash = new BYTE[cryptHashSize];
		ReadFile(hReadPipe, cryptHash, sizeof(BYTE) * cryptHashSize, &dw, NULL);

		DWORD cryptDataSize;
		ReadFile(hReadPipe, &cryptDataSize, sizeof(DWORD), &dw, NULL);
		BYTE* cryptData = new BYTE[cryptDataSize];
		ReadFile(hReadPipe, cryptData, sizeof(BYTE) * cryptDataSize, &dw, NULL);

		//2. Создание ключа на основе пароля 
		char* password = new char[1000];
		cout << "Введите пароль: ";
		cin >> password;

		HCRYPTHASH hCryptHash;
		CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hCryptHash);
		CryptHashData(hCryptHash, (BYTE*)password, strlen(password), 0);
		HCRYPTKEY hCryptKey;
		CryptDeriveKey(hCryptProv, CALG_RC4, hCryptHash, 0, &hCryptKey);

		//3. Расшифрование данных и хеша RC4
		CryptDecrypt(hCryptKey, NULL, TRUE, 0, (BYTE*)cryptHash, &cryptHashSize);
		cryptHash[cryptHashSize] = '\0';
		cout << "Расшифрованный хеш: ";
		for (int i = 0; i < cryptHashSize; i++)
			printf("%X%X", cryptHash[i] >> 4, 0xF & cryptHash[i]);
		cout << endl;

		CryptDecrypt(hCryptKey, NULL, TRUE, 0, (BYTE*)cryptData, &cryptDataSize);
		cryptData[cryptDataSize] = '\0';
		cout << "Расшифрованное сообщение: " << cryptData << endl;

		//4. Вычисление хеша данных 
		CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hCryptHash);
		CryptHashData(hCryptHash, (BYTE*)cryptData, cryptDataSize, CRYPT_USERDATA);
		DWORD hashSize = 16;
		BYTE* hashValue = new BYTE[hashSize];
		CryptGetHashParam(hCryptHash, HP_HASHVAL, hashValue, &hashSize, 0);
		cout << "# MD5 хеш: ";
		for (int i = 0; i < hashSize; i++)
			printf("%X%X", hashValue[i] >> 4, 0xF & hashValue[i]);
		cout << endl;
		cout << "Расшифрованный MD5 хеш: ";
		for (int i = 0; i < cryptHashSize; i++)
			printf("%X%X", cryptHash[i] >> 4, 0xF & cryptHash[i]);
		cout << endl;

		bool check = true;
		if (cryptHashSize == hashSize)
		{
			for (int i = 0; i < hashSize; i++)
				if (cryptHash[i] != hashValue[i])
				{
					check = false;
					break;
				}
		}
		else check = false;

		if (check) cout << "Пароль верный";
		else cout << "Не верный пароль";
		cout << endl;

		cout << "Декодирование завершено" << endl;
		CryptDestroyKey(hCryptKey);
		CryptDestroyHash(hCryptHash);
		CryptReleaseContext(hCryptProv, 0);
		ResetEvent(hEventDecode);
	}
}

int main()
{
	setlocale (LC_ALL, "russian");

	CreatePipe(&hReadPipe, &hWritePipe, NULL, 0);

	hEventCode = CreateEvent(NULL, FALSE, FALSE, NULL);
	hEventDecode = CreateEvent(NULL, FALSE, FALSE, NULL);

	HANDLE hCoder = CreateThread(NULL, 0, &tCoder, NULL, 0, NULL);

	HANDLE hDecoder = CreateThread(NULL, 0, &tDecoder, NULL, 0, NULL);

	SetEvent(hEventCode);

	WaitForSingleObject(hCoder, INFINITE);	
	WaitForSingleObject(hDecoder, INFINITE);

	system("pause");
    return 0;
}
