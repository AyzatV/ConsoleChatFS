#pragma once
#include "Users.h"
#include "Messages.h"

namespace avhw18
{
	enum class CommandType
	{
		HELP = '?',	// вывод списка команд
		CHAT = 'c',	// показать чат с пользователем, параметр - логин или имя пользователя (при отсутствии - общий чат)
		IN = 'i',	// войти в систему, параметр - логин
		MES = 'm',	// написать сообщение пользователю, параметр - логин или имя пользователя (при отсутствии - в общий чат)
		OUT = 'o',	// выход из системы
		QUIT = 'q',	// выход из программы
		REG = 'r',	// регистрация в системе
		SHOW = 's', // показать непрочитанные сообщения, параметр - логин или имя отправителя (при отсутствии - все новые сообщения)
		USERS = 'u'	// вывод списка зарегистрированных пользователей
	};

	// структура для команд пользователя при работе с классом ChatBot
	struct CB_command
	{
	private:
		CommandType type = CommandType::HELP;
		std::string param;
	public:
		friend std::istream& operator>> (std::istream&, CB_command&);
		friend std::ostream& operator<< (std::ostream&, const CB_command&);
		friend class ChatBot;
	};

	// основной класс для работы с чатами
	class ChatBot
	{
	private:
		const std::string USERS_FILE = "chat_users.txt";	// файл для хранения массива пользователей
		const std::string MESS_FILE = "chat_mess.txt";		// файл для хранения массива сообщений
		Users m_users{ USERS_FILE };			// массив зарегистрированных пользователей
		Messages m_messages{ MESS_FILE };		// массив сообщений
		bool m_logged = false;	// true, если пользователь вошел в систему
		User m_loggedUser;		// пользователь, который вошел в систему
		size_t m_userIndex;		// индекс текущего пользователя в массиве зарегистрированных пользователей

		std::stringstream log_in(std::string);
		std::stringstream log_out();
		std::stringstream quit() const;
		std::stringstream regist();
		std::stringstream showChat(std::string);
		std::stringstream showFresh(std::string);
		std::stringstream showHelp() const;
		std::stringstream showUsers() const;
		std::stringstream writeMessage(std::string);
		std::stringstream makeMessage(std::string, std::string);
		bool is_yes(std::string);
		bool name_OK(User&);
		bool input_OK(std::string, std::string&);

	public:
		ChatBot();
		~ChatBot();
		std::stringstream ApplyCommand(bool&, const CB_command&);
	};
}