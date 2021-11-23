#include "algorithm.h"
#include "des.h"
using namespace std;
using namespace algorithm;
string result_en1;

string& algorithm::CaesarAlgoritm(string& s)
{
    int key;
    cout << "Введите ключ: " << endl;
    cin >> key;
    for (int i = 0; i < s.length(); i++)
        if (s[i] >= 'a' && s[i] <= 'z')
        {
            s[i] += (key % 26);
            if (s[i] > 'z') s[i] -= 26;
        }
        else if (s[i] >= 'A' && s[i] <= 'Z')
        {
            s[i] += (key % 26);
            if (s[i] > 'Z') s[i] -= 26;
        }
    //cout << s << endl;
    return s;
}

string& algorithm::DeCaesarAlgoritm(string& s)
{   
    int key;
    cout << "Введите ключ: " << endl;
    cin >> key;
    for (int i = 0; i < s.length(); i++)
        if (s[i] >= 'a' && s[i] <= 'z')
        {
            s[i] -= (key % 26);
            if (s[i] < 'a') s[i] += 26;
        }
        else if (s[i] >= 'A' && s[i] <= 'Z')
        {
            s[i] -= (key % 26);
            if (s[i] < 'A') s[i] += 26;
        }
   // cout << s << endl;
    return s;
}
/*----------------------------------------------------------ЗДЕСЬ НАЧИНАЕТСЯ DES--------------------------------------------------------------------------------------*/

string byte2bit(string byte) {// строка в битовую строку
	size_t length = byte.length();
	string bit(length * 8, 0);
	for (size_t i = 0; i < length; i++) {
		for (int j = 0; j < 8; j++) {
			bit[i * 8 + j] = (byte[i] >> (7 - j)) & 1;
		}
	}
	return bit;
}

string bit2byte(string bit) {// бит из строки в строку
	int length = bit.length() / 8;
	string byte(length, 0);
	for (int i = 0; i < length; i++) {
		byte[i] = 0;
		for (int j = 0; j < 8; j++) {
			byte[i] = (byte[i] << 1) + bit[i * 8 + j];
		}
	}
	return byte;
}

string hex2bit(string hex) {// Шестнадцатеричная строка в битовую строку
	int length = hex.length();
	string bit(length * 4, 0);
	for (int i = 0; i < length; i++) {
		hex[i] -= 48;
		if (hex[i] > 9)
			hex[i] -= 7;
		for (int j = 0; j < 4; j++) {
			bit[i * 4 + j] = (hex[i] >> (3 - j)) & 1;
		}
	}
	return bit;
}

string bit2hex(string bit) {// битовая строка в шестнадцатеричную строку
	int length = bit.length() / 4;
	string hex(length, 0);
	for (int i = 0; i < length; i++) {
		hex[i] = 0;
		for (int j = 0; j < 4; j++) {
			hex[i] = (hex[i] << 1) + bit[i * 4 + j];
		}
		hex[i] += 48;
		if (hex[i] > 57)
			hex[i] += 7;
	}
	return hex;
}

void output(string s) {// вывод двоичной строки
	cout << s.length() << "\t";
	for (size_t i = 0; i < s.length(); i++) {
		if (s[i] == 1)
			cout << 1;
		else
			cout << 0;
	}
	cout << endl;
}

string transform(string bit, TABLE* table, int length) {    // Замена матрицы
	string tmp(length, 0);
	for (int i = 0; i < length; i++) {
		tmp[i] = bit[table[i] - 1];
	}
	return tmp;
}

void get_subkey(string * subkey, string key) {// Получить подключ
	string bit_key = byte2bit(key);
	string transformed_key = transform(bit_key, KEY_Table, 56);
	string C(transformed_key, 0, 28);
	string D(transformed_key, 28, 28);

	for (int i = 0; i < 16; i++) {
		C = C.substr(SHIFT_Table[i]) + C.substr(0, SHIFT_Table[i]);
		D = D.substr(SHIFT_Table[i]) + D.substr(0, SHIFT_Table[i]);
		subkey[i] = transform(C + D, PC2_Table, 48);
	}
}

string string_xor(string a, string b) {// XOR двоичной строки
	for (size_t i = 0; i < a.length(); i++) {
		a[i] ^= b[i];
	}
	return a;
}

string B2C(string B, int i) {// Использовать поле S
	int row = B[0] * 2 + B[5];
	int col = B[1] * 8 + B[2] * 4 + B[3] * 2 + B[4];
	int s = S_Box[i][row - 1][col - 1];
	string C;
	for (i = 3; i >= 0; i--) {
		C += (int(s >> i) & 1);
	}
	return C;
}

string function(string R, string K) {   // функция f
	string ER = transform(R, EXTENSION_Table, 48);
	string BS = string_xor(ER, K);
	string f;
	for (int i = 0; i < 8; i++) {
		string B(BS.substr(i * 6, 6));
		string C = B2C(B, i);
		f += C;
	}
	return f;
}

			string iterative_en(string L, string R, string* K, MODE mode) {   // 16 итераций
				if (mode == en) {
					for (int i = 0; i < 16; i++) {
						string tmp(L);
						L = R;
						R = string_xor(tmp, function(R, K[i]));
					}
				}
				return transform(L + R, IP1_Table, 64);
				cout << endl;
			}

			string iterative_de(string L, string R, string* K, MODE mode) {

				if (mode == de) {
				for (int i = 15; i >= 0; i--) {
					string tmp(R);
					R = L;
					L = string_xor(tmp, function(L, K[i]));
				}
				}
				return transform(L + R, IP1_Table, 64);
				cout << endl;
			}
string des_en(string data, string key, MODE mode) {
									string bit_data;
									if (mode == en)
										bit_data = byte2bit(data);

									bit_data = transform(bit_data, IP_Table, 64);
									string L(bit_data, 0, 32);
									string R(bit_data, 32, 32);

									string subkey[16];
									get_subkey(subkey, key);

									string result_en = iterative_en(L, R, subkey, mode);
									if (mode == en) {
										string hex = bit2hex(result_en);
										//cout << "cipher:\t";
										//cout << hex << endl << endl;
										return hex;
									}
								}


								string des_de(string data, string key, MODE mode) {
									string bit_data;
									if (mode == de)
										bit_data = hex2bit(data);

									bit_data = transform(bit_data, IP_Table, 64);
									string L(bit_data, 0, 32);
									string R(bit_data, 32, 32);

									string subkey[16];
									get_subkey(subkey, key);

									string result_de = iterative_de(L, R, subkey, mode);
									if (mode == de) {
										string byte = bit2byte(result_de);
										//cout << "plaintext: ";
										//cout << byte << endl << endl;
										return byte;
									}
								}

string ECB_en(string data, string key, MODE mode) {// режим кодовой книги
												string result_en;
												if (mode == en) {
													for (int i = 0; i<int(data.length() >> 3); i++) {
														//cout << "No." << i + 1 << "block: \ t";
														result_en += des_en(data.substr(i * 8, 8), key, mode);
													}
													cout << "final cipher: ";
												}
												result_en1 = result_en;
												cout << result_en << endl << endl;
												return result_en;
											}


											string ECB_de(string data, string key, MODE mode) {// режим кодовой книги
												string result_de;
												if (mode == de) {
													for (int i = 0; i<int(data.length() >> 4); i++) {
														//cout << "No." << i + 1 << "block: \ t";
														result_de += des_de(data.substr(i * 16, 16), key, mode);
													}
													cout << "final plaintext: " << endl;
												}
												cout << result_de << endl << endl;
												return result_de;
											}

string& algorithm::DESAlgoritm(string& s)
{ string key;
					cout << "Введите ключ(8 символов): " << endl;
					cin >> key; // Устанавливаем ключ

					string plaintext = s;
					//cout << "plaintext: ";
					//cout << plaintext << endl << endl;

					char c = 0;
					while (plaintext.length() % 8 != 0) {// Открытый текст меньше 8 цифр и автоматически заполняется 0
						plaintext += c;
					}

					string cipher;
					// Использовать режим кодовой книги
					cipher = ECB_en(plaintext, key, en);
					//plaintext = ECB_de(cipher, key, de);


    return s;
}

string& algorithm::DeDESAlgoritm(string& s)
{
    
    string key;
					cout << "Введите ключ(8 символов): " << endl;
					cin >> key; // Устанавливаем ключ

					string res = result_en1;
					//cout << "cipher: ";
					//cout << res << endl << endl;

					/*char c1 = 0;
					while (res.length() % 8 != 0) {// Открытый текст меньше 8 цифр и автоматически заполняется 0
						res += c1;
					}*/

					string cipher;
					// Использовать режим кодовой книги
					cipher = result_en1;//ECB_en(plaintext, key, en);
					res = ECB_de(cipher, key, de);

    return s;
}
/*----------------------------------------------------------------------------------ЗДЕСЬ DES КОНЧИЛСЯ---------------------------------------------------------------------*/
string& algorithm::AESAlgoritm(string& s)
{
    return s;
}

string& algorithm::DeAESAlgoritm(string& s)
{
    return s;
}

string& algorithm::PlayfairAlgoritm(string& s)
{
    return s;
}

string& algorithm::DePlayfairAlgoritm(string& s)
{
    return s;
}

string& algorithm::ElgamalAlgoritm(string& s)
{
    return s;
}

string& algorithm::DeElgamalAlgoritm(string& s)
{
    return s;
}
