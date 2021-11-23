#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <wincrypt.h>
#include <bcrypt.h>
#include <string.h>
#include <iostream>
#include <tchar.h>
#include <io.h>
#include <vector>
#include <fstream>
#include <stdio.h>

CHAR pass[65];

std::cin >> pass;

HCRYPTPROV hProv; //дескриптор криптопровайдера
HCRYPTKEY hKey; //дескриптор ключа, созданного из хэш-кода
HCRYPTHASH hHash; //дескриптор хэш-объекта

if (!CryptAcquireContext(&hProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, 0))
{
    std::cout << "Error\n";
    return 1;
}

if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash))
{
    std::cout << "Error\n";
    CryptReleaseContext(hProv, 0);
    return 1;
}

if (!CryptHashData(hHash, (BYTE*)pass, strlen(pass), 0))
{
    std::cout << "Error\n";
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    return 1;
}

if (!CryptDeriveKey(hProv, CALG_AES_192, hHash, CRYPT_EXPORTABLE, &hKey))
{
    std::cout << "Error\n";
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    return 1;
}

std::ifstream ifs("data.txt", std::ifstream::binary);
ifs.seekg(0, ifs.end);
int len = ifs.tellg();
ifs.seekg(0);

std::vector<char> data(len + 10240);

ifs.read(data.data(), len);
ifs.close();

DWORD datalen = len;
if (!CryptEncrypt(hKey, NULL, true, 0, reinterpret_cast<BYTE*>(data.data()), &datalen, data.size()))
{
    std::cout << "Error\n";
}
else
{
    std::ofstream ofs("data_enc.txt", std::ifstream::binary);
    ofs.write(data.data(), datalen);
    ofs.close();
}

CryptDestroyHash(hHash);
CryptDestroyKey(hKey);
CryptReleaseContext(hProv, 0);

///////////////////////////////////////////////////////////////////////////////////////////////////////

if (!CryptAcquireContext(&hProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, 0))
{
    std::cout << "Error\n";
    return 1;
}

if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash))
{
    std::cout << "Error\n";
    CryptReleaseContext(hProv, 0);
    return 1;
}

if (!CryptHashData(hHash, (BYTE*)pass, strlen(pass), 0))
{
    std::cout << "Error\n";
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    return 1;
}

if (!CryptDeriveKey(hProv, CALG_AES_192, hHash, CRYPT_EXPORTABLE, &hKey))
{
    std::cout << "Error\n";
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    return 1;
}

ifs.open("data_enc.txt", std::ifstream::binary);
ifs.seekg(0, ifs.end);
len = ifs.tellg();
ifs.seekg(0);

data.resize(len + 10240);

ifs.read(data.data(), len);
ifs.close();

datalen = len;
if (!CryptDecrypt(hKey, NULL, true, 0, reinterpret_cast<BYTE*>(data.data()), &datalen))
{
    std::cout << "Error\n";
}
else
{
    std::ofstream ofs("data_dec.txt", std::ifstream::binary);
    ofs.write(data.data(), datalen);
    ofs.close();
}

CryptDestroyHash(hHash);
CryptDestroyKey(hKey);
CryptReleaseContext(hProv, 0);
}