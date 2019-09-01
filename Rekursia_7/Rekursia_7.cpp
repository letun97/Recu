//реверсия строк запись в файл происходит в потоке
#include <iostream>
#include <fstream>
#include <string>
#include <thread> 
#include <chrono>
#include <mutex>
#include <list>
#include <limits>
using namespace std;
bool b = true;
void func(ofstream& FunOut, ifstream& FunIn, mutex& m_out, mutex& m_in)
{
	setlocale(LC_ALL, "ru");
	string end; //буфер
	m_in.lock(); //мутекс останавливает поток, когда другой поток читает данные из файла
	getline(FunIn, end);//считываем строчку из файла
	m_in.unlock();
	if (end != "")//проверка не закончился ли текст
	{
		size_t length = end.length(); //определяем длинну строки

		m_out.lock(); // мутекс останаливает поток, кога другой мутекс записывает данные
		for (int i = length - 1; i >= 0; i--)
		{
			//cout << end[i]; //выводим строку задом на перед
			FunOut << end[i]; //вывод строки на оборот в файл
		}
		//cout << endl;
		FunOut << endl;
		m_out.unlock();
	}
	else
	{
		b = false;
	}

}

int main() {
	//Колличество потоков
	const int length = 10;
	mutex m_out, m_in;
	int kol = 0; 

	std::ofstream out;
	std::ifstream in("text.txt"); // окрываем файл для чтения
	out.open("print.txt"); // окрываем файл для записи
	if ((in.is_open()) && (out.is_open()))
	{
		while (b)
		{
			
			thread Rethread[length];
			//потоки
			for (int i = 0; i < length; i++) {
				Rethread[i] = thread(func, std::ref(out), std::ref(in), ref(m_out), ref(m_in));
			}
			//закрытие потоков
			for (int i = 0; i < length; i++) {
				if (Rethread[i].joinable()) {
					Rethread[i].join();
				}
			}
			kol += length;
			cout << "обработано " << kol << " строк" << endl;
		}

	}
	//закрытие файлов
	out.close();
	in.close();
	return 0;
}