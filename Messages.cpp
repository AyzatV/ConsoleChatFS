#include "Messages.h"

namespace avhw18
{
	Message& Message::operator=(const Message& m)
	{
		if (&m != this)
		{
			text = m.text;
			from_name = m.from_name;
			to_name = m.to_name;
			time = m.time;
			fresh = m.fresh;
		}
		return *this;
	}

	// вывод параметров сообщения в поток, используется для вывода списка сообщений
	std::ostream& operator<<(std::ostream& out, const Message& m)
	{
		out << std::endl << "отправлено " << m.time << " пользователем [" << m.from_name << "]";
		if (m.to_name.empty()) { out << " для всех пользователей:"; }
		else
		{
			out << " для пользователя [" << m.to_name << "]:";
			if (!m.fresh) { out << std::endl << "[сообщение прочитано получателем]"; }
		}
		out << std::endl << m.text << std::endl;
		return out;
	}

	Messages::Messages() : DATA_FILE{} {}
	
	Messages::Messages(std::string data_file) : DATA_FILE(data_file)
	{
		// загрузка массива сообщений из файла
		uploader<Message> ul;
		if (ul.data_loaded(sm_converter, m_mess, M_QUANTS, DATA_FILE)) std::cout << "Массив сообщений загружен" << std::endl;
		else std::cout << "Массив сообщений загрузить не удалось" << std::endl;
	}
	
	Messages::~Messages()
	{
		// выгрузка массива сообщений в файл
		if (!DATA_FILE.empty())
		{
			uploader<Message> ul;
			if (ul.data_saved(ms_converter, m_mess, DATA_FILE)) std::cout << "Массив сообщений сохранен в файл" << std::endl;
			else std::cout << "Массив сообщений сохранить в файл не удалось" << std::endl;
		}
	}

	// добавление сообщения в массив
	void Messages::add(const Message& m) { m_mess.push_back(m); }

	// предикат (условие) для отбора общих сообщений, передается по указателю в TArray.getSelection через функцию getCommon (см. ниже)
	bool Messages::selectCommon(const Message& m, Message& c)
	{
		return (m.to_name.empty());
	}

	// предикат (условие) для отбора парных сообщений, передается по указателю в TArray.getSelection через функцию getPaired (см. ниже)
	bool Messages::selectPaired(const Message& m, Message& c)
	{
		return ((m.to_name == c.to_name && m.from_name == c.from_name) || (m.to_name == c.from_name && m.from_name == c.to_name));
	}

	// предикат (условие) для отбора непрочитанных сообщений, передается по указателю в TArray.getSelection через функцию getFresh (см. ниже)
	bool Messages::selectFresh(const Message& m, Message& c)
	{
		bool result = false;
		if (c.from_name.empty())
		{
			if (m.to_name.empty()) { result = (m.time > c.time); }
			else { result = (m.fresh && m.to_name == c.to_name); }
		}
		else { result = (m.from_name == c.from_name && m.to_name == c.to_name && m.fresh); }
		return result;
	}

	std::vector<std::reference_wrapper<Message>> Messages::getCommon()
	{
		Message c;
		return m_mess.getSelection(selectCommon, c);
	}
	
	std::vector<std::reference_wrapper<Message>> Messages::getPaired(Message& c)
	{
		return m_mess.getSelection(selectPaired, c);
	}

	std::vector<std::reference_wrapper<Message>> Messages::getFresh(Message& c)
	{
		return m_mess.getSelection(selectFresh, c);
	}
	
	// преобразование в массив строк для хранения данных
	std::vector<std::string> Messages::ms_converter(const Message& m)
	{
		auto v = TimeData::ts_converter(m.time);
		v.push_back(m.fresh ? "true" : "false");
		v.push_back(m.from_name);
		v.push_back(m.to_name);
		v.push_back(m.text);
		return v;
	}
	
	// восстановление из массива строк
	Message Messages::sm_converter(const std::vector<std::string>& v)
	{
		Message m;
		m.time = TimeData::st_converter(v);
		m.fresh = (v.at(6) == "true");
		m.from_name = v.at(7);
		m.to_name = v.at(8);
		m.text = v.at(9);
		return m;
	}
}