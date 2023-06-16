#pragma once
#include "uploader.h"

namespace avhw18
{
	struct Message
	{
	public:
		std::string text;		// текст сообщения
		std::string from_name;	// "полное" имя отправителя
		std::string to_name;	// "полное" имя получателя
		TimeData time;			// время отправки сообщения
		bool fresh = true;		// true, если сообщение не прочитано получателем

		Message& operator=(const Message&);
		friend std::ostream& operator<< (std::ostream&, const Message&);
	};

	class Messages
	{
	private:
		// файл для хранения данных
		const std::string DATA_FILE;
		// количество строк для хранения Message
		const size_t M_QUANTS = 10;
		
		// вариант отсутствия сообщений при запуске программы
		std::vectorf<Message> m_mess;

		// вариант наличия сообщений при запуске программы - только при активации пользователей в Users!
		//std::vectorf<Message> m_mess
		//{
		//	{"Всем привет! Меня зовут Лена", "Лена--fox", "", TimeData(), true},
		//	{"Привет всем, я Вова", "Вова--pot", "", TimeData(), true},
		//	{"Всем привет от Толика!", "Толик--tin", "", TimeData(), true},
		//	{"Как тебе тут в этом чате?", "Толик--tin", "Лена--fox", TimeData(), true},
		//	{"Ну, мы тут только для демонстрации же", "Лена--fox", "Толик--tin", TimeData(), true},
		//	{"А что означает твой логин?", "Лена--fox", "Вова--pot", TimeData(), true},
		//	{"Что я пока чайник в программировании", "Вова--pot", "Лена--fox", TimeData(), true},
		//	{"Чат в консоли - это нечто конечно", "Вова--pot", "Толик--tin", TimeData(), true},
		//	{"Согласен, дешево и сердито ;)", "Толик--tin", "Вова--pot", TimeData(), true}
		//};

	public:
		Messages();
		Messages(std::string);
		~Messages();

		void add(const Message&);
		static bool selectCommon(const Message&, Message&);
		static bool selectPaired(const Message&, Message&);
		static bool selectFresh(const Message&, Message&);
		std::vector<std::reference_wrapper<Message>> getCommon();
		std::vector<std::reference_wrapper<Message>> getPaired(Message&);
		std::vector<std::reference_wrapper<Message>> getFresh(Message&);
		static std::vector<std::string> ms_converter(const Message&);
		static Message sm_converter(const std::vector<std::string>&);
	};
}