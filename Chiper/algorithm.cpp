#include "algorithm.h"
using namespace std;

ifstream& algorithm::CaesarAlgoritm(ifstream& st)
{
    //int key;
    //string s;
    //cout << "Введите ключ: " << endl;
    //cin >> key;
    //cout << "Закодированное сообщение: " << endl;
    //ifstream(s).rdbuf();
    //for (int i = 0; i < s.length(); i++)
    //    if (s[i] >= 'a' && s[i] <= 'z')
    //    {
    //        s[i] += (key % 26);
    //        if (s[i] > 'z') s[i] -= 26;
    //    }
    //    else if (s[i] >= 'A' && s[i] <= 'Z')
    //    {
    //        s[i] += (key % 26);
    //        if (s[i] > 'Z') s[i] -= 26;
    //    }
    //cout << s << endl;
    return st;
}

ifstream& algorithm::DeCaesarAlgoritm(ifstream& s)
{   
    //int key;
    //cout << "Введите ключ: " << endl;
    //cin >> key;
    //cout << "Декодированное сообщение: " << endl;
    //for (int i = 0; i < s.length(); i++)
    //    if (s[i] >= 'a' && s[i] <= 'z')
    //    {
    //        s[i] -= (key % 26);
    //        if (s[i] < 'a') s[i] += 26;
    //    }
    //    else if (s[i] >= 'A' && s[i] <= 'Z')
    //    {
    //        s[i] -= (key % 26);
    //        if (s[i] < 'A') s[i] += 26;
    //    }
    //cout << s << endl;
    return s;
}

ifstream& algorithm::DESAlgoritm(ifstream& s)
{
    return s;
}

ifstream& algorithm::DeDESAlgoritm(ifstream& s)
{
    return s;
}

ifstream& algorithm::AESAlgoritm(ifstream& s)
{
    return s;
}

ifstream& algorithm::DeAESAlgoritm(ifstream& s)
{
    return s;
}

ifstream& algorithm::PlayfairAlgoritm(ifstream& s)
{
    return s;
}

ifstream& algorithm::DePlayfairAlgoritm(ifstream& s)
{
    return s;
}

ifstream& algorithm::ElgamalAlgoritm(ifstream& s)
{
    return s;
}

ifstream& algorithm::DeElgamalAlgoritm(ifstream& s)
{
    return s;
}
