#pragma once
#include "uploader.h"

namespace avhw18
{
	struct User
	{
	public:
		std::string log;
		std::string name;
		std::string pword;
		TimeData last_seen;	// время последнего просмотра общего чата

		void clear();
		std::string fullName() const;
		User& operator=(const User&);
		friend std::ostream& operator<< (std::ostream&, const User&);
	};

	class Users
	{
	private:
		// файл для хранения данных
		const std::string DATA_FILE;
		// количество строк для хранения User
		const size_t U_QUANTS = 9;

		// вариант отсутствия пользователей при запуске программы
		std::vectorf<User> m_users;

		// вариант наличия пользователей при запуске программы
		//std::vectorf<User> m_users
		//{
		//	{"fox", "Лена", "xof", TimeData()},
		//	{"pot", "Вова", "top", TimeData()},
		//	{"tin", "Толик", "nit", TimeData()}
		//};

	public:
		Users();
		Users(std::string);
		~Users();

		size_t add(const User&);
		void fixTime(size_t);
		static bool selectByName(const User&, User&);
		static bool selectByLog(const User&, User&);
		bool nameFound(User&, size_t&);
		bool logFound(User&, size_t&);
		bool empty() const;
		std::stringstream showNames() const;
		static std::vector<std::string> us_converter(const User&);
		static User su_converter(const std::vector<std::string>&);
	};
}