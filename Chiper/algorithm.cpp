#include "algorithm.h"
using namespace std;
string algorithm::CaesarAlgoritm(string& s)
{
    int key;
    cout << "Введите ключ: " << endl;
    cin >> key;
    cout << "Закодированное сообщение: " << endl;
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
    cout << s << endl;
    return s;
}

string algorithm::DeCaesarAlgoritm(string& s)
{   
    int key;
    cout << "Введите ключ: " << endl;
    cin >> key;
    cout << "Декодированное сообщение: " << endl;
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
    cout << s << endl;
    return s;
}

string algorithm::DESAlgoritm(string& s)
{
    return s;
}

string algorithm::DeDESAlgoritm(string& s)
{
    return s;
}

string algorithm::AESAlgoritm(string& s)
{
    return s;
}

string algorithm::DeAESAlgoritm(string& s)
{
    return s;
}

string algorithm::PlayfairAlgoritm(string& s)
{
    return s;
}

string algorithm::DePlayfairAlgoritm(string& s)
{
    return s;
}

string algorithm::ElgamalAlgoritm(string& s)
{
    return s;
}

string algorithm::DeElgamalAlgoritm(string& s)
{
    return s;
}
