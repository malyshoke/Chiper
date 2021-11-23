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
//================================AES=========================================//
//
//  Перед использованием необходимо сгенерировать 
//  расширенный ключ (из 128-ми, 192-ч или 256-ти битного)
//
aes::status aes::expand_key(const byte* key, const unsigned size)
{
    unsigned i;

    //  инициализированный s_box содержит здесь ненулевое значение
    if (!s_box[0]) init_tables();

    //  Копируем ключ.

    u32* ek = encryption_round_key;
    for (i = 0; i < size / (8 * sizeof(u32)); i++)
        ek[i] = get(&key[i * 4]);

    //  Создаём расширенный ключ для засшифровки

    u32 round_constant = 1;
    switch (size)
    {
    default:
        ;

#ifdef AES_128
    case 128:
        for (i = 0; i < 10; i++, ek += 4)
        {
            ek[4] = ek[0]
                ^ s_box[ek[3] >> 0 & 0xFF] << 24
                ^ s_box[ek[3] >> 24] << 16
                ^ s_box[ek[3] >> 16 & 0xFF] << 8
                ^ s_box[ek[3] >> 8 & 0xFF] << 0
                ^ round_constant;
            ek[5] = ek[1] ^ ek[4];
            ek[6] = ek[2] ^ ek[5];
            ek[7] = ek[3] ^ ek[6];
            round_constant = lfsr2((byte)round_constant);
        }
        rounds = 10;
        break;
#endif

#ifdef AES_192
    case 192:
        for (i = 0; i < 8; i++, ek += 6)
        {
            ek[6] = ek[0]
                ^ s_box[ek[5] >> 0 & 0xFF] << 24
                ^ s_box[ek[5] >> 24] << 16
                ^ s_box[ek[5] >> 16 & 0xFF] << 8
                ^ s_box[ek[5] >> 8 & 0xFF] << 0
                ^ round_constant;
            ek[7] = ek[1] ^ ek[6];
            ek[8] = ek[2] ^ ek[7];
            ek[9] = ek[3] ^ ek[8];
            ek[10] = ek[4] ^ ek[9];
            ek[11] = ek[5] ^ ek[10];
            //  упростим lfsr2, т.к кол-во итераций цикла < 9
            round_constant <<= 1;// lfsr2( round_constant );
        }
        rounds = 12;
        break;
#endif

#ifdef AES_256
    case 256:
        for (i = 0; i < 7; i++, ek += 8)
        {
            ek[8] = ek[0]
                ^ s_box[ek[7] >> 0 & 0xFF] << 24
                ^ s_box[ek[7] >> 24] << 16
                ^ s_box[ek[7] >> 16 & 0xFF] << 8
                ^ s_box[ek[7] >> 8 & 0xFF] << 0
                ^ round_constant;
            ek[9] = ek[1] ^ ek[8];
            ek[10] = ek[2] ^ ek[9];
            ek[11] = ek[3] ^ ek[10];
            ek[12] = ek[4]
                ^ s_box[ek[11] >> 24] << 24
                ^ s_box[ek[11] >> 16 & 0xFF] << 16
                ^ s_box[ek[11] >> 8 & 0xFF] << 8
                ^ s_box[ek[11] >> 0 & 0xFF] << 0;
            ek[13] = ek[5] ^ ek[12];
            ek[14] = ek[6] ^ ek[13];
            ek[15] = ek[7] ^ ek[14];
            //  упростим lfsr2, т.к кол-во итераций цикла < 9
            round_constant <<= 1;// lfsr2( round_constant );
        }
        rounds = 14;
        break;
#endif
    }

    //  Создаём расширенный ключ для расшифровки

    u32* dk = decryption_round_key;
    dk[0] = ek[0];  dk[1] = ek[1];  dk[2] = ek[2];  dk[3] = ek[3];

    for (i = 4 * (rounds - 1); i; --i, dk++)
    {
        //  Исходные данные расположены в таком порядке:
        //  ..., 9, 10, 11, 12, 5, 6, 7, 8, 1, 2, 3, 4.
        //  первый раз из 4х двигаем указатель "назад" на 7
        //  остальные три - "вперёд" на 1
        ek += i & 0x3 ? 1 : -7;
        dk[4] = rt[0][s_box[ek[4 - 1] >> 0 & 0xFF]]
            ^ rt[1][s_box[ek[4 - 1] >> 8 & 0xFF]]
            ^ rt[2][s_box[ek[4 - 1] >> 16 & 0xFF]]
            ^ rt[3][s_box[ek[4 - 1] >> 24]];
    }
    dk[4] = ek[4 - 8];    dk[5] = ek[5 - 8];
    dk[6] = ek[6 - 8];    dk[7] = ek[7 - 8];

    
}


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
//================================AES=========================================//
//
//  Перед использованием необходимо сгенерировать 
//  расширенный ключ (из 128-ми, 192-ч или 256-ти битного)
//
aes::status aes::expand_key(const byte* key, const unsigned size)
{
    unsigned i;

    //  инициализированный s_box содержит здесь ненулевое значение
    if (!s_box[0]) init_tables();

    //  Копируем ключ.

    u32* ek = encryption_round_key;
    for (i = 0; i < size / (8 * sizeof(u32)); i++)
        ek[i] = get(&key[i * 4]);

    //  Создаём расширенный ключ для засшифровки

    u32 round_constant = 1;
    switch (size)
    {
    default:
        ;

#ifdef AES_128
    case 128:
        for (i = 0; i < 10; i++, ek += 4)
        {
            ek[4] = ek[0]
                ^ s_box[ek[3] >> 0 & 0xFF] << 24
                ^ s_box[ek[3] >> 24] << 16
                ^ s_box[ek[3] >> 16 & 0xFF] << 8
                ^ s_box[ek[3] >> 8 & 0xFF] << 0
                ^ round_constant;
            ek[5] = ek[1] ^ ek[4];
            ek[6] = ek[2] ^ ek[5];
            ek[7] = ek[3] ^ ek[6];
            round_constant = lfsr2((byte)round_constant);
        }
        rounds = 10;
        break;
#endif

#ifdef AES_192
    case 192:
        for (i = 0; i < 8; i++, ek += 6)
        {
            ek[6] = ek[0]
                ^ s_box[ek[5] >> 0 & 0xFF] << 24
                ^ s_box[ek[5] >> 24] << 16
                ^ s_box[ek[5] >> 16 & 0xFF] << 8
                ^ s_box[ek[5] >> 8 & 0xFF] << 0
                ^ round_constant;
            ek[7] = ek[1] ^ ek[6];
            ek[8] = ek[2] ^ ek[7];
            ek[9] = ek[3] ^ ek[8];
            ek[10] = ek[4] ^ ek[9];
            ek[11] = ek[5] ^ ek[10];
            //  упростим lfsr2, т.к кол-во итераций цикла < 9
            round_constant <<= 1;// lfsr2( round_constant );
        }
        rounds = 12;
        break;
#endif

#ifdef AES_256
    case 256:
        for (i = 0; i < 7; i++, ek += 8)
        {
            ek[8] = ek[0]
                ^ s_box[ek[7] >> 0 & 0xFF] << 24
                ^ s_box[ek[7] >> 24] << 16
                ^ s_box[ek[7] >> 16 & 0xFF] << 8
                ^ s_box[ek[7] >> 8 & 0xFF] << 0
                ^ round_constant;
            ek[9] = ek[1] ^ ek[8];
            ek[10] = ek[2] ^ ek[9];
            ek[11] = ek[3] ^ ek[10];
            ek[12] = ek[4]
                ^ s_box[ek[11] >> 24] << 24
                ^ s_box[ek[11] >> 16 & 0xFF] << 16
                ^ s_box[ek[11] >> 8 & 0xFF] << 8
                ^ s_box[ek[11] >> 0 & 0xFF] << 0;
            ek[13] = ek[5] ^ ek[12];
            ek[14] = ek[6] ^ ek[13];
            ek[15] = ek[7] ^ ek[14];
            //  упростим lfsr2, т.к кол-во итераций цикла < 9
            round_constant <<= 1;// lfsr2( round_constant );
        }
        rounds = 14;
        break;
#endif
    }

    //  Создаём расширенный ключ для расшифровки

    u32* dk = decryption_round_key;
    dk[0] = ek[0];  dk[1] = ek[1];  dk[2] = ek[2];  dk[3] = ek[3];

    for (i = 4 * (rounds - 1); i; --i, dk++)
    {
        //  Исходные данные расположены в таком порядке:
        //  ..., 9, 10, 11, 12, 5, 6, 7, 8, 1, 2, 3, 4.
        //  первый раз из 4х двигаем указатель "назад" на 7
        //  остальные три - "вперёд" на 1
        ek += i & 0x3 ? 1 : -7;
        dk[4] = rt[0][s_box[ek[4 - 1] >> 0 & 0xFF]]
            ^ rt[1][s_box[ek[4 - 1] >> 8 & 0xFF]]
            ^ rt[2][s_box[ek[4 - 1] >> 16 & 0xFF]]
            ^ rt[3][s_box[ek[4 - 1] >> 24]];
    }
    dk[4] = ek[4 - 8];    dk[5] = ek[5 - 8];
    dk[6] = ek[6 - 8];    dk[7] = ek[7 - 8];

    
}


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
	//@}    protected:


byte inline aes::lfsr2(byte a)
{
    return a & 0x80 ? a << 1 ^ (byte)basis    // вычитание по модулю 2
        : a << 1;                 // нет обратноё связи
}


//=========================================================================//
/// !.
/// Создаёт прямую и обратную таблицы замен (S-box)
/// и расширенные таблицы замен/микширования -
/// используются для оптимизации криптораундов по скорости.
///
void aes::init_tables()
{
    //  Таблицы для быстрого умножения в поле GF(2**8)  
    byte    pow[1 << 8], log[1 << 8];

    //  Заполняем их.
    unsigned    i = 0;      //  Начальная степень omega;
    byte        a = 1;      //  omega**i в полиномиальной форме;
    log[0] = 0;             //  Ой элемент нужен для генерации S-box
    do
    {
        pow[i] = a;         //  omega**i = x7**7 + .. + x0**0;
        log[a] = (byte)i;
        //  следующий элемент таблицы выбирается 
        //  на основе полинома phi(x+1)
        a ^= lfsr2(a);      //  a *= (x + 1)
    } while ((byte)++i);     //  2**8 итераций

    //  Что бы не инициализировать отдельно элементы [0x63, 0]
    //  используем "лишнюю" итерацию цикла;
    //   для этого модифицируем неиспользуемый элемент таблицы.
    pow[(1 << 8) - 1] = 0;

    //  Генерируем прямой и обратный S-box'ы
    i = 0;
    do
    {
        byte    a = pow[(1 << 8) - 1 - log[i]];

        //  Умножаем на     x**4 + x**3 + x**2 + x + 1
        a ^= a << 1 ^ a << 2 ^ a << 3 ^ a << 4  // a<<1|a>>7 == a<<1^a>>7 !
            ^ a >> 4 ^ a >> 5 ^ a >> 6 ^ a >> 7  // переставим слагаемые 
                                                  // что бы помочь компилятору
          //  и прибавляем    x**6 + x**5 + x**1 + x**0
            ^ (1 << 6 ^ 1 << 5 ^ 1 << 1 ^ 1 << 0);

        s_box[i] = a;
        r_box[a] = i;
    } while ((byte)++i);     //  2**8 итераций

    //  Генерируем расширенные таблицы замен
    for (i = 0; i < 256; i++)
    {
        byte f = (byte)s_box[i];
        byte f2 = lfsr2(f);
        ft[0][i] = (f ^ f2) << 24 | f << 16 | f << 8 | f2;
        ft[1][i] = rot_left_8(ft[0][i]);
        ft[2][i] = rot_left_8(ft[1][i]);
        ft[3][i] = rot_left_8(ft[2][i]);

        byte r = (byte)r_box[i];
        //  Умножение выполняется по формуле:
        //  pow[(log[x] + log[y]) % 255]    ( при x и y != 0 )
        rt[0][i] = !r ? r :
            (u32)pow[(0x68 + log[r]) % 255] << 24 // log[0x0B] == 0x68
            ^ (u32)pow[(0xEE + log[r]) % 255] << 16 // log[0x0D] == 0xEE
            ^ (u32)pow[(0xC7 + log[r]) % 255] << 8 // log[0x09] == 0xC7
            ^ (u32)pow[(0xDF + log[r]) % 255] << 0;// log[0x0E] == 0xDF
        rt[1][i] = rot_left_8(rt[0][i]);
        rt[2][i] = rot_left_8(rt[1][i]);
        rt[3][i] = rot_left_8(rt[2][i]);
    }
}


aes::u32 inline aes::get(const byte array[4])
{
#if (('1234' >> 24) == '1')     // little endian ?
    return  *(u32*)(array);
#else
    return  array[0] << 0
        | array[1] << 8
        | array[2] << 16
        | array[3] << 24;
#endif
}


void inline aes::put(u32 value, byte array[4])
{
#if (('1234' >> 24) == '1')     // little endian ?
    * (u32*)(array) = value;
#else
    array[0] = value >> 0 & 0xFF;
    array[1] = value >> 8 & 0xFF;
    array[2] = value >> 16 & 0xFF;
    array[3] = value >> 24;
#endif
}


aes::u32 inline aes::fb(u32 byte, unsigned pos)
{
    return ft[pos][byte >> pos * 8 & 0xFF];
}


aes::u32 inline aes::rb(u32 byte, unsigned pos)
{
    return rt[pos][byte >> pos * 8 & 0xFF];
}


aes::u32    aes::s_box[256], aes::r_box[256];
aes::u32    aes::ft[4][256], aes::rt[4][256];

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
