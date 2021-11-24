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
													//cout << "final cipher: ";
												}
												result_en1 = result_en;
												//cout << result_en << endl << endl;
												return result_en;
											}


											string ECB_de(string data, string key, MODE mode) {// режим кодовой книги
												string result_de;
												if (mode == de) {
													for (int i = 0; i<int(data.length() >> 4); i++) {
														//cout << "No." << i + 1 << "block: \ t";
														result_de += des_de(data.substr(i * 16, 16), key, mode);
													}
													//cout << "final plaintext: " << endl;
												}
												//cout << result_de << endl << endl;
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
                                        s = cipher;

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
                                        s = res;
    return s;
}
/*----------------------------------------------------------------------------------ЗДЕСЬ DES КОНЧИЛСЯ---------------------------------------------------------------------*/
string& algorithm::AESAlgoritm(string& s) {
	return s;
}

string& algorithm::DeAESAlgoritm(string& s)	
{
	return s;
   
}


int len(string s) {
	int l = 0;
	for (int i = 0; s[i] != '\0'; i++) {
		l++;
	}
	return l;
}

string ToValid(string s, int l) {
	string m;
	for (int i = 0; i < l; i++) {
		if (isupper(s[i])) {
			m += tolower(s[i]);
		}
		else { m += s[i]; }
	}
	return m;
}

string& algorithm::PlayfairAlgoritm(string& s)
{
	char alp[26] = { 0 };
	char M[5][5] = { 0 };

	cout << "Enter the key: ";
	string key;
	cin >> key;
	int k_len = len(key);

	int m_len = len(s);
	key = ToValid(key, k_len);
	k_len = len(key);
	for (int i = 0; i < k_len; i++) {
		char ch = key[i];
		alp[ch - 97] = 1;
	}

	for (int i = 0; i < 26; i++) {
		if (alp[i] == 1) {
			s += (char)i + 97;
		}
	}
	key = s;
	k_len = len(key);

	s = ToValid(s, m_len);
	m_len = len(s);
	string d;
	for (int i = 0; i < m_len; i++) {
		if (s[i] != 'j') {
			d += s[i];
		}
		else { d += 'i'; }
	}
	s = d;
	m_len = len(s);
	string ss;
	for (int i = 0; i < m_len; i++) {
		if (i > 0) {
			if (s[i - 1] == s[i]) {
				ss += 'x';
				ss += s[i];
			}
			else {
				ss += s[i];
			}
		}
		else { ss += s[i]; }
	}
	s = ss;
	m_len = len(s);
	if (m_len % 2 != 0) {
		s += 'x';
		m_len++;
	}
	int n = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (n < k_len) {
				M[i][j] = key[n];
				n++;
			}
		}
	}
	char ch = 97;
	string filtred;
	while (len(filtred) < 25 - k_len) {
		for (int i = 0; i < k_len; i++) {
			if (key[i] == ch) {
				ch++;
				i = 0;
			}
		}
		if (ch != 'j') {
			filtred += ch;
			ch++;
		}
		else { ch++; }
	}
	int c = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (i * 5 + j + 1 > k_len) {
				M[i][j] = filtred[c];
				c++;
			}
		}
	}
	string encrypted;
	for (int i = 0; i < m_len; i += 2) {
		char ch1 = s[i];
		char ch2 = s[i + 1];
		int ch1_i = 0;
		int ch1_j = 0;
		int ch2_i = 0;
		int ch2_j = 0;
		for (int o = 0; o < 5; o++) {
			for (int t = 0; t < 5; t++) {
				if (M[o][t] == ch1) {
					ch1_i = o;
					ch1_j = t;
				}
				if (M[o][t] == ch2) {
					ch2_i = o;
					ch2_j = t;
				}
			}
		}
		if (ch1_i == ch2_i) {
			ch1_j++;
			ch1_j %= 5;
			ch2_j++;
			ch2_j %= 5;
			encrypted += M[ch1_i][ch1_j];
			encrypted += M[ch1_i][ch2_j];
		}
		if (ch1_j == ch2_j) {
			ch1_i++;
			ch1_i %= 5;
			ch2_i++;
			ch2_i %= 5;
			encrypted += M[ch1_i][ch1_j];
			encrypted += M[ch2_i][ch1_j];
		}
		if (ch1_i != ch2_i && ch1_j != ch2_j) {
			int temp = ch1_i;
			ch1_i = ch2_i;
			ch2_i = temp;
			encrypted += M[ch1_i][ch1_j];
			encrypted += M[ch2_i][ch2_j];
		}

	}
	s = encrypted;
	return s;
}

string& algorithm::DePlayfairAlgoritm(string& str)
{
	char alp[26] = { 0 };
	char M[5][5] = { 0 };
	string encrypted;
	cout << "Enter the key : ";
	string key;
	cin >> key;
	int k_len = len(key);
	encrypted = str;
	string decrypted;
	for (int i = 0; i < len(encrypted); i += 2) {
		char ch1 = encrypted[i];
		char ch2 = encrypted[i + 1];
		int ch1_i = 0;
		int ch1_j = 0;
		int ch2_i = 0;
		int ch2_j = 0;
		for (int o = 0; o < 5; o++) {
			for (int t = 0; t < 5; t++) {
				if (M[o][t] == ch1) {
					ch1_i = o;
					ch1_j = t;
				}
				if (M[o][t] == ch2) {
					ch2_i = o;
					ch2_j = t;
				}
			}
		}
		if (ch1_i == ch2_i) {
			ch1_j -= 1;
			if (ch1_j < 0) {
				ch1_j = 5 - ch1_j;
			}
			ch2_j -= 1;
			if (ch2_j < 0) {
				ch2_j = 5 - ch2_j;
			}
			decrypted += M[ch1_i][ch1_j];
			decrypted += M[ch1_i][ch2_j];
		}
		if (ch1_j == ch2_j) {
			ch1_i -= 1;
			if (ch1_i < 0) {
				ch1_i = 5 - ch1_i;
			}
			ch2_i -= 1;
			if (ch2_i < 0) {
				ch2_i = 5 - ch2_i;
			}
			decrypted += M[ch1_i][ch1_j];
			decrypted += M[ch2_i][ch1_j];
		}
		if (ch1_i != ch2_i && ch1_j != ch2_j) {
			int temp = ch1_i;
			ch1_i = ch2_i;
			ch2_i = temp;
			decrypted += M[ch1_i][ch1_j];
			decrypted += M[ch2_i][ch2_j];
		}

	}
	str = decrypted;
	return str;
}

string& algorithm::VigenereAlgoritm(string& s)
{	
	cout << __func__ << endl;
	string B; string C; string D;
	B = s;
	cout << "Enter the key: " << endl;
	cin >> C;
	int c = C.size(); //делаем замену переменных для удобства
	int b = B.size();
	int k = 0;

	if (b >= c)
	{
		for (int i = 0; i < (b / c); i++)
		{
			D = D + C; //Записываем целое количество ключа. Растягивая ключ по длине слова.
		}
		for (int j = 0; j < (b % c); j++)
		{
			D = D + C[j];
		}
	}
	else  //Иначе если ключ длиннее слова, укорачиваем ключ до длины слова.
	{
		for (int s = 0; s < b; s++)
		{
			D = D + C[s];
		}
	}
	cout << "New key: " << D << endl;

	for (int i = 0; i < b; i++) {
		k = (int)(D[i]);
		B[i] += k;
	}
    return s=B;
}

string& algorithm::DeVigenereAlgoritm(string& s)
{	
	string B; string C; string D;
	cout << __func__ << endl;
	B = s;
	cout << "Enter the key: " << endl;
	cin >> C;
	int c = C.size(); //делаем замену переменных для удобства
	int b = B.size();
	int k = 0;
	if (b >= c)
	{
		for (int i = 0; i < (b / c); i++)
		{
			D = D + C; //Записываем целое количество ключа. Растягивая ключ по длине слова.
		}
		for (int j = 0; j < (b % c); j++)
		{
			D = D + C[j];
		}
	}
	else  //Иначе если ключ длиннее слова, укорачиваем ключ до длины слова.
	{
		for (int s = 0; s < b; s++)
		{
			D = D + C[s];
		}
	}
	cout << "New key: " << D << endl;
	for (int i = 0; i < b; i++) {
		k = (int)(D[i]);
		B[i] -= k;
	}

    return s=B;
}
