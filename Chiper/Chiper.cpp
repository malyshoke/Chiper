// Chiper.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

    #include <sstream>
    #include <iostream>
    #include <fstream>
    #include <string>
    #include <chrono>
    #include <vector>
    #include "algorithm.h"
    using namespace std;
    using namespace algorithm;
    using Method = std::string&(*) (std::string&);

    double countTime(Method alg, string& data)
    {
        auto start = std::chrono::system_clock::now();
        alg(data);
        auto finish = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
        return duration;
    }

    int main(int argc, char* argv)
    {
        string s;
        string Input[]{ "data1.txt", "data2.txt" };
        string Output[size(Input)]{ "res1.txt", "res2.txt" };
        ostringstream buf;
        vector<pair<Method, Method>> methods =
        {
            {CaesarAlgoritm,DeCaesarAlgoritm}, {DESAlgoritm,DeDESAlgoritm}, {AESAlgoritm,DeAESAlgoritm}, {PlayfairAlgoritm,DePlayfairAlgoritm}, {ElgamalAlgoritm,DeElgamalAlgoritm},

        };

        setlocale(LC_ALL, "Rus");
            for (size_t i = 0; i < size(Input); i++)
            {
                ifstream fin(Input[i]);
                ofstream fout(Output[i]);
                if (fin.is_open() && fout.is_open())
                {
                    for (auto method : methods) {
                        auto start = std::chrono::system_clock::now();
                        cout << Input[i] << endl;
                        ostringstream buf;
                        buf << fin.rdbuf();
                        s = buf.str();
                        auto finish = std::chrono::system_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
                        double readTime = duration;
                        cout << "Время чтения:" << readTime << endl;
                        cout << "Время шифрования: " << countTime(method.first, s) << endl;
                        fout << "Зашифрованный текст: " << endl << s << endl;
                        /*cout << "Зашифрованный текст: " << endl << s << endl;*/
                        cout << "Время дешифрования: " << countTime(method.second, s) << endl;
                        fout << "Дешифрованный текст: " << endl << s << endl;
                        /*cout << "Дешифрованный текст: " << endl << s << endl;*/
                        fin.close();
                        fout.close();
                    }
                }
                else
                    cerr << "Не удается открыть файл" << endl;
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
