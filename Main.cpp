#include "ChatBot.h"
#include <exception>
#ifdef OS_WIN
#include <Windows.h>
#endif // OS_WIN

int main()
{
#ifdef OS_WIN
	// подключение кодовых страниц для ввода-вывода кириллицей
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
#endif // OS_WIN
	{
		bool goon = true;
		avhw18::ChatBot cb;
		avhw18::CB_command command;

		while (goon)
		{
			try
			{
				std::cout << std::endl << "Введите команду: ";
				std::cin >> command;
				std::cout << std::endl << "Вы ввели команду: " << command << std::endl;
				std::cout << cb.ApplyCommand(goon, command).str();
			}
			catch (const std::exception& e)
			{
				goon = false;
				std::cerr << std::endl << "Обработано стандартное исключение:" << std::endl << e.what() << std::endl;
			}
			catch (...)
			{
				goon = false;
				std::cerr << std::endl << "Обработано неизвестное исключение" << std::endl;
			}
		}
	}

	std::cout << std::endl << "Работа программы прекращена" << std::endl;
	std::cin.get();
	return 0;
}