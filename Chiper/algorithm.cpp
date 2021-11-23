#include "algorithm.h"
#include "des.h"
using namespace std;
using namespace algorithm;
string result_en1;

string& algorithm::CaesarAlgoritm(string& s)
{
    int key;
	cout << __func__ << endl;
    cout << "Enter the key: " << endl;
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
	cout << __func__ << endl;
    cout << "Enter the key: " << endl;
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
cout << __func__ << endl;
					cout << "Enter the key (8 symbols):  " << endl;
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
	cout << __func__ << endl;
					cout << "Enter the key (8 symbols):  " << endl;
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
{ =========================================================================//
//
//  Шифрование
// 
void aes::encrypt(const byte in[block_size], byte out[block_size])
{
    //  Указатель на раундовые константы
    u32* rk = encryption_round_key;

    //  Считываем блок и добавляем раундовый ключ.
    u32     a0 = get(&in[0]) ^ rk[0];
    u32     a1 = get(&in[4]) ^ rk[1];
    u32     a2 = get(&in[8]) ^ rk[2];
    u32     a3 = get(&in[12]) ^ rk[3];
    u32     b0, b1, b2, b3;

    //  Раунды криптования (цикл развёрнут 2x)
    for (int i = rounds; ; rk += 8)
    {
        b0 = fb(a0, 0) ^ fb(a1, 1) ^ fb(a2, 2) ^ fb(a3, 3) ^ rk[4];
        b1 = fb(a1, 0) ^ fb(a2, 1) ^ fb(a3, 2) ^ fb(a0, 3) ^ rk[5];
        b2 = fb(a2, 0) ^ fb(a3, 1) ^ fb(a0, 2) ^ fb(a1, 3) ^ rk[6];
        b3 = fb(a3, 0) ^ fb(a0, 1) ^ fb(a1, 2) ^ fb(a2, 3) ^ rk[7];

        //  Сверхумный MSVC разворачивает цикл, что раздувает размер
        //  и снижает (!) скорость на 10%.
        //  приходится добавлять лишний байт :-(.
#if defined(_MSC_VER) && ! defined(__cplusplus_cli)
        ;
#endif

        if (!(i -= 2)) break;

        a0 = fb(b0, 0) ^ fb(b1, 1) ^ fb(b2, 2) ^ fb(b3, 3) ^ rk[8];
        a1 = fb(b1, 0) ^ fb(b2, 1) ^ fb(b3, 2) ^ fb(b0, 3) ^ rk[9];
        a2 = fb(b2, 0) ^ fb(b3, 1) ^ fb(b0, 2) ^ fb(b1, 3) ^ rk[10];
        a3 = fb(b3, 0) ^ fb(b0, 1) ^ fb(b1, 2) ^ fb(b2, 3) ^ rk[11];
    }

    //  Последний раунд (без перемешивания столбцов)
    put(
        rk[8] ^ s_box[b3 >> 24] << 24
        ^ s_box[b2 >> 16 & 0xFF] << 16
        ^ s_box[b1 >> 8 & 0xFF] << 8
        ^ s_box[b0 >> 0 & 0xFF] << 0
        , &out[0]
    );
    put(
        rk[9] ^ s_box[b0 >> 24] << 24
        ^ s_box[b3 >> 16 & 0xFF] << 16
        ^ s_box[b2 >> 8 & 0xFF] << 8
        ^ s_box[b1 >> 0 & 0xFF] << 0
        , &out[4]
    );
    put(
        rk[10] ^ s_box[b1 >> 24] << 24
        ^ s_box[b0 >> 16 & 0xFF] << 16
        ^ s_box[b3 >> 8 & 0xFF] << 8
        ^ s_box[b2 >> 0 & 0xFF] << 0
        , &out[8]
    );
    put(
        rk[11] ^ s_box[b2 >> 24] << 24
        ^ s_box[b1 >> 16 & 0xFF] << 16
        ^ s_box[b0 >> 8 & 0xFF] << 8
        ^ s_box[b3 >> 0 & 0xFF] << 0
        , &out[12]
    );
}
    return s;
}

string& algorithm::DeAESAlgoritm(string& s)	
{ //=========================================================================//
//
//  Расшифровывание.
// 
void aes::decrypt(const byte in[block_size], byte out[block_size])
{
    //  Указатель на раундовые константы
    u32* rk = decryption_round_key;

    //  Считываем блок и добавляем раундовый ключ.
    u32     a0 = get(&in[0]) ^ rk[0];
    u32     a1 = get(&in[4]) ^ rk[1];
    u32     a2 = get(&in[8]) ^ rk[2];
    u32     a3 = get(&in[12]) ^ rk[3];
    u32     b0, b1, b2, b3;

    //  Раунды разкриптования (цикл развёрнут 2x)
    for (int i = rounds; ; rk += 8)
    {
        b0 = rb(a0, 0) ^ rb(a3, 1) ^ rb(a2, 2) ^ rb(a1, 3) ^ rk[4];
        b1 = rb(a1, 0) ^ rb(a0, 1) ^ rb(a3, 2) ^ rb(a2, 3) ^ rk[5];
        b2 = rb(a2, 0) ^ rb(a1, 1) ^ rb(a0, 2) ^ rb(a3, 3) ^ rk[6];
        b3 = rb(a3, 0) ^ rb(a2, 1) ^ rb(a1, 2) ^ rb(a0, 3) ^ rk[7];

        //  Сверхумный MSVC разворачивает цикл, что раздувает размер
        //  и снижает (!) скорость.
        //  приходится добавлять лишний байт :-(.
#if defined(_MSC_VER) && ! defined(__cplusplus_cli)
        
#endif

        if (!(i -= 2)) break;

        a0 = rb(b0, 0) ^ rb(b3, 1) ^ rb(b2, 2) ^ rb(b1, 3) ^ rk[8];
        a1 = rb(b1, 0) ^ rb(b0, 1) ^ rb(b3, 2) ^ rb(b2, 3) ^ rk[9];
        a2 = rb(b2, 0) ^ rb(b1, 1) ^ rb(b0, 2) ^ rb(b3, 3) ^ rk[10];
        a3 = rb(b3, 0) ^ rb(b2, 1) ^ rb(b1, 2) ^ rb(b0, 3) ^ rk[11];
    }

    //  Последний раунд (без перемешивания столбцов)
    put(
        rk[8] ^ r_box[b1 >> 24] << 24
        ^ r_box[b2 >> 16 & 0xFF] << 16
        ^ r_box[b3 >> 8 & 0xFF] << 8
        ^ r_box[b0 >> 0 & 0xFF] << 0
        , &out[0]
    );
    put(
        rk[9] ^ r_box[b2 >> 24] << 24
        ^ r_box[b3 >> 16 & 0xFF] << 16
        ^ r_box[b0 >> 8 & 0xFF] << 8
        ^ r_box[b1 >> 0 & 0xFF] << 0
        , &out[4]
    );
    put(
        rk[10] ^ r_box[b3 >> 24] << 24
        ^ r_box[b0 >> 16 & 0xFF] << 16
        ^ r_box[b1 >> 8 & 0xFF] << 8
        ^ r_box[b2 >> 0 & 0xFF] << 0
        , &out[8]
    );
    put(
        rk[11] ^ r_box[b0 >> 24] << 24
        ^ r_box[b1 >> 16 & 0xFF] << 16
        ^ r_box[b2 >> 8 & 0xFF] << 8
        ^ r_box[b3 >> 0 & 0xFF] << 0
        , &out[12]
    );
}
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
