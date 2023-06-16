#include "Users.h"

namespace avhw18
{
	// приведение параметров к начальным "нулевым" значениям
	void User::clear()
	{
		this->log.clear();
		this->pword.clear();
		this->name.clear();
		this->last_seen.clear();
	}

	// "полное" имя пользователя в формате (имя--логин) для возможности идентификации, т.к. имена пользователей могут совпадать
	std::string User::fullName() const { return (name + "--" + log); }

	User& User::operator=(const User& u)
	{
		if (&u != this)
		{
			log = u.log;
			pword = u.pword;
			name = u.name;
			last_seen = u.last_seen;
		}
		return *this;
	}

	// вывод в поток данных пользователя, используется для проверки при регистрации пользователя
	std::ostream& operator<<(std::ostream& out, const User& u)
	{
		out << " логин: " << u.log << std::endl;
		out << "пароль: " << u.pword << std::endl;
		out << "   имя: " << u.name << std::endl;
		return out;
	}

	Users::Users() : DATA_FILE{} {}

	Users::Users(std::string data_file) : DATA_FILE(data_file)
	{
		// загрузка массива пользователей из файла
		uploader<User> ul;
		if (ul.data_loaded(su_converter, m_users, U_QUANTS, DATA_FILE)) std::cout << "Массив пользователей загружен" << std::endl;
		else std::cout << "Массив пользователей загрузить не удалось" << std::endl;
	}
	
	Users::~Users()
	{
		// выгрузка массива пользователей в файл
		if (!DATA_FILE.empty())
		{
			uploader<User> ul;
			if (ul.data_saved(us_converter, m_users, DATA_FILE)) std::cout << "Массив пользователей сохранен в файл" << std::endl;
			else std::cout << "Массив пользователей сохранить в файл не удалось" << std::endl;
		}
	}

	// добавление пользователя в массив
	size_t Users::add(const User& u)
	{
		m_users.push_back(u);
		return m_users.size() - 1;
	}

	// фиксация времени просмотра пользователем общего чата
	void Users::fixTime(size_t index) { m_users[index].last_seen.renew(); }

	// предикат для поиска пользователя по имени (через User& c), передается по указателю в vectorf.tryGetOne через функцию nameFound (см. ниже)
	bool Users::selectByName(const User& u, User& c) { return (u.log == c.log || u.name == c.name); }

	// предикат для поиска пользователя по логину (через User& c), передается по указателю в vectorf.tryGetOne через функцию logFound (см. ниже)
	bool Users::selectByLog(const User& u, User& c) { return (u.log == c.log); }

	bool Users::nameFound(User& c, size_t& i) { return m_users.tryGetOne(selectByName, c, i); }

	bool Users::logFound(User& c, size_t& i) { return m_users.tryGetOne(selectByLog, c, i); }

	// проверка наличия зарегистрированных пользователей
	bool Users::empty() const { return (m_users.size() == 0); }

	// вывод списка "полных" имен пользователей
	std::stringstream Users::showNames() const
	{
		std::stringstream info;
		info << std::endl << "Имена пользователей:" << std::endl;
		for (const auto& user : m_users) { info << user.fullName() << std::endl; }
		return info;
	}
	
	// преобразование в массив строк для хранения данных
	std::vector<std::string> Users::us_converter(const User& u)
	{
		auto v = TimeData::ts_converter(u.last_seen);
		v.push_back(u.log);
		v.push_back(u.name);
		v.push_back(u.pword);
		return v;
	}
	
	// восстановление из массива строк
	User Users::su_converter(const std::vector<std::string>& v)
	{
		User u;
		u.last_seen = TimeData::st_converter(v);
		u.log = v.at(6);
		u.name = v.at(7);
		u.pword = v.at(8);
		return u;
	}
}