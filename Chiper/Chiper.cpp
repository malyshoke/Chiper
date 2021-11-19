// Chiper.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


    #include <iostream>
    #include <fstream>
    #include <string>
    #include <mutex>
    #include <chrono>
    #include "algorithm.h"
    using namespace std;
    int main(int argc, char* argv)
    {
        mutex m;
        string s;
        string Input[]{ "data1.txt", "data2.txt" };
        string Output[size(Input)]{ "res1.txt", "res2.txt" };
        setlocale(LC_ALL, "Rus");
        for (size_t i = 0; i < size(Input); i++)
        {
            ifstream fin(Input[i]);
            ofstream fout(Output[i]);
            if (fin.is_open() && fout.is_open())
            {
                cout << Input[i] << endl;
                getline(fin, s);
                fout << algorithm::CaesarAlgoritm(s) << endl;
                fout << algorithm::DeCaesarAlgoritm(s) << endl;
                fin.close();
                fout.close();
            }
            else
                cerr << "Files could not open" << endl;
        }
        return 0;
    }


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
