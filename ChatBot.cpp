#include "ChatBot.h"

namespace avhw18
{
	ChatBot::ChatBot() { std::cout << showHelp().str() << std::endl; }	// вывод списка команд при запуске программы
	ChatBot::~ChatBot() {}

	// исполнение введенной пользователем команды (параметр goon = false используется для выхода из цикла в main)
	std::stringstream ChatBot::ApplyCommand(bool& goon, const CB_command& command)
	{
		std::stringstream result;
		goon = true;
		switch (command.type)
		{
		case CommandType::HELP:
			result = showHelp();
			break;
		case CommandType::CHAT:
			result = showChat(command.param);
			break;
		case CommandType::IN:
			result = log_in(command.param);
			break;
		case CommandType::MES:
			result = writeMessage(command.param);
			break;
		case CommandType::OUT:
			result = log_out();
			break;
		case CommandType::QUIT:
			result = quit();
			goon = false;
			break;
		case CommandType::REG:
			result = regist();
			break;
		case CommandType::SHOW:
			result = showFresh(command.param);
			break;
		case CommandType::USERS:
			result = showUsers();
			break;
		default:
			result << std::endl << "Данная команда некорректна!" << std::endl;
			break;
		}
		return result;
	}

	// регистрация пользователя в системе
	std::stringstream ChatBot::regist()
	{
		m_loggedUser.clear();
		m_logged = false;
		std::string s;
		std::stringstream info;
		bool ok = false;
		bool goon = true;
		std::cout << std::endl << "Для регистрации введите по запросу Ваши логин, пароль и имя";
		// цикл для ввода и проверки логина
		do
		{
			ok = input_OK("логин", m_loggedUser.log);
			ok = ok && (!m_users.logFound(m_loggedUser, m_userIndex));	// проверка на совпадение введенного логина с логином другого пользователя
			if (ok) { goon = false; }
			else
			{
				m_loggedUser.clear();
				std::cout << std::endl << "Введенный Вами логин не может быть использован." << std::endl;
				std::cout << "Согласны ли Вы продолжить регистрацию с другим логином?" << std::endl;
				goon = input_OK("да или Y (yes), если согласны", s);
				goon = goon && is_yes(s);
			}
		} while (goon);
		ok = ok && input_OK("пароль", m_loggedUser.pword);
		ok = ok && input_OK("имя", m_loggedUser.name);
		if (ok)
		{
			std::cout << std::endl << "Вы ввели следующие данные:" << std::endl << m_loggedUser << std::endl;
			std::cout << "Подтверждаете ли Вы регистрацию в системе с этими данными?" << std::endl;
			ok = input_OK("да или Y (yes), если подтверждаете", s);
			ok = ok && is_yes(s);
			if (ok)
			{
				m_logged = true;
				m_userIndex = m_users.add(m_loggedUser);
				info << std::endl << m_loggedUser.name << ", Вы успешно зарегистрированы в системе." << std::endl;
				info << "Запишите или запомните Ваши регистрационные данные." << std::endl;
				info << "Сейчас Вы можете писать и читать сообщения или выйти из системы" << std::endl;
			}
			else
			{
				m_loggedUser.clear();
				info << std::endl << "Регистрация отменена, введите команду r для повторной регистрации" << std::endl;
			}
		}
		else
		{
			m_loggedUser.clear();
			info << std::endl << "Регистрация не удалась: не все данные были введены" << std::endl;
		}
		return info;
	}

	// вход пользователя в систему
	std::stringstream ChatBot::log_in(std::string log)
	{
		m_loggedUser.clear();
		std::string s;
		std::stringstream info;
		bool ok = true;
		if(log.empty()) { ok = input_OK("логин", m_loggedUser.log); }
		else { m_loggedUser.log = log; }
		ok = ok && m_users.logFound(m_loggedUser, m_userIndex);
		ok = ok && input_OK("пароль", s);
		ok = ok && (s == m_loggedUser.pword);
		if (ok)
		{
			m_logged = true;
			info << std::endl << m_loggedUser.name << ", Вы вошли в систему и можете писать и просматривать сообщения" << std::endl;
		}
		else
		{
			m_logged = false;
			m_loggedUser.clear();
			info << std::endl << "Вход в систему не удался: несовпадение учетных данных" << std::endl;
		}
		return info;
	}

	// выход пользователя из системы
	std::stringstream ChatBot::log_out()
	{
		std::stringstream info;
		if (m_logged)
		{
			info << std::endl << m_loggedUser.name << ", Вы вышли из системы" << std::endl;
			m_logged = false;
			m_loggedUser.clear();
		}
		else { info << std::endl << "Выход из системы возможен только после входа в нее" << std::endl; }
		return info;
	}

	// вывод списка команд
	std::stringstream ChatBot::showHelp() const
	{
		std::stringstream help;
		help << std::endl << " Список команд (вводится символ команды и через пробел параметр при необходимости):" << std::endl;
		help << " q - прекращение работы с программой" << std::endl;
		help << " r - регистрация нового пользователя" << std::endl;
		help << " i - вход пользователя в систему чата, параметр - логин" << std::endl;
		help << " o - выход из системы чата" << std::endl;
		help << " c - вывод чата с другим пользователем, параметр - логин или имя другого" << std::endl;
		help << "     пользователя (при отсутствии параметра - вывод общего чата), находясь в котором" << std::endl;
		help << "     можно написать одно сообщение другому пользователю (или всем, если общий чат)," << std::endl;
		help << "     после чего происходит переход в режим ввода команд" << std::endl;
		help << " m - написать сообщение пользователю, параметр - логин или имя получателя" << std::endl;
		help << "     (при отсутствии параметра сообщение адресуется всем)" << std::endl;
		help << " s - вывод непрочитанных сообщений, параметр - логин или имя отправителя" << std::endl;
		help << "     (при отсутствии параметра выводятся все новые сообщения)" << std::endl;
		help << " u - вывод списка пользователей" << std::endl;
		help << " ? - вывод данного списка команд" << std::endl;
		return help;
	}

	// вывод зарегистрированных пользователей
	std::stringstream ChatBot::showUsers() const
	{
		std::stringstream info;
		if (m_users.empty()) { info << "Пока нет зарегистрированных пользователей" << std::endl; }
		else { info << m_users.showNames().str(); }
		return info;
	}

	// выход из программы
	std::stringstream ChatBot::quit() const
	{
		std::stringstream info;
		info << std::endl << "Поступил запрос на прекращение работы программы" << std::endl;
		return info;
	}

	// вывод сообщений чата и отправка сообщения в этот чат
	std::stringstream ChatBot::showChat(std::string from_name)
	{
		std::stringstream info;
		if (m_logged)
		{
			std::string to_name;
			bool name_ok = from_name.empty();
			Message comparer;	// параметр для поиска сообщений на основе сравнения с заданными критериями
			if (name_ok)
			{
				// выборка и показ сообщений, адресованных всем (общий чат)
				std::vector<std::reference_wrapper<Message>> selection(m_messages.getCommon());
				for (Message& m : selection) { info << m << std::endl; }
				m_loggedUser.last_seen.renew();	// фиксация времени просмотра сообщений общего чата
				m_users.fixTime(m_userIndex);
			}
			else
			{
				User sender{ from_name, from_name };
				// поиск пользователя по совпадению с его логином или именем и, если он найден, внесение его данных в sender (отправитель)
				name_ok = name_OK(sender);
				if (name_ok)
				{
					comparer.to_name = m_loggedUser.fullName();
					to_name = sender.fullName();
					comparer.from_name = to_name;
					// выборка и показ сообщений чата между текущим (loggedUser) и другим пользователем
					std::vector<std::reference_wrapper<Message>> selection(m_messages.getPaired(comparer));
					for (Message& m : selection)
					{
						info << m << std::endl;
						if (m.to_name == comparer.to_name) { m.fresh = false; }	// пометка сообщения как прочитанного
					}
				}
			}
			if (name_ok)
			{
				std::cout << info.str();
				info.str(std::string());	// очистка info
				info << makeMessage(m_loggedUser.fullName(), to_name).str();
			}
			else { info << std::endl << "Введенный Вами пользователь не найден (команда u - вывод списка)" << std::endl; }
		}
		else { info << std::endl << "Для работы с чатами необходимо войти в систему (команда i)" << std::endl; }
		return info;
	}

	// вывод непрочитанных сообщений
	std::stringstream ChatBot::showFresh(std::string from_name)
	{
		std::stringstream info;
		if (m_logged)
		{
			bool name_ok = from_name.empty();
			bool no_fresh;
			Message comparer;	// параметр для поиска сообщений на основе сравнения с заданными критериями
			comparer.to_name = m_loggedUser.fullName();
			comparer.time = m_loggedUser.last_seen;
			if (name_ok)
			{
				m_loggedUser.last_seen.renew();	// фиксация времени просмотра сообщений общего чата
				m_users.fixTime(m_userIndex);
				comparer.from_name = from_name;
				std::stringstream m_c;	// параметр для сборки непрочитанных сообщений, адресованных всем
				std::stringstream m_p;	// параметр для сборки непрочитанных сообщений, адресованных текущему пользователю
				// выборка сообщений, непрочитанных текущим пользователем
				std::vector<std::reference_wrapper<Message>> selection(m_messages.getFresh(comparer));
				no_fresh = (selection.size() == 0);
				if (!no_fresh)
				{
					for (Message& m : selection)
					{
						if (m.to_name.empty()) { m_c << m << std::endl; }
						else
						{
							m_p << m << std::endl;
							m.fresh = false;	// пометка сообщения как прочитанного
						}
					}
					if (!m_c.str().empty()) { info << std::endl << "Новые сообщения, адресованные всем:" << std::endl << m_c.str(); }
					if (!m_p.str().empty()) { info << std::endl << "Новые сообщения, адресованные Вам:" << std::endl << m_p.str(); }
				}
			}
			else
			{
				User sender{ from_name, from_name };
				// поиск пользователя по совпадению с его логином или именем и, если он найден, внесение его данных в sender (отправитель)
				name_ok = name_OK(sender);
				if (name_ok)
				{
					comparer.to_name = m_loggedUser.fullName();
					comparer.from_name = sender.fullName();
					// выборка непрочитанных сообщений от конкретного пользователя
					std::vector<std::reference_wrapper<Message>> selection(m_messages.getFresh(comparer));
					no_fresh = (selection.size() == 0);
					if (!no_fresh)
					{
						for (Message& m : selection)
						{
							info << m << std::endl;
							m.fresh = false;
						}
					}
				}
			}
			if (name_ok) { if (no_fresh) { info << std::endl << "Непрочитанных Вами сообщений нет" << std::endl; } }
			else { info << std::endl << "Введенный Вами отправитель не найден (команда u - вывод списка)" << std::endl; }
		}
		else { info << std::endl << "Для работы с чатами необходимо войти в систему (команда i)" << std::endl; }
		return info;
	}

	// подготовка и отправка сообщения
	std::stringstream ChatBot::writeMessage(std::string name)
	{
		std::stringstream info;
		if (m_logged)
		{
			std::string to_name = name;
			bool name_ok = name.empty();
			if (!name_ok)
			{
				User recipient{ name, name };
				// поиск пользователя по совпадению с его логином или именем и, если он найден, внесение его данных в recipient (получатель)
				name_ok = name_OK(recipient);
				to_name = recipient.fullName();
			}
			if(name_ok) { info << makeMessage(m_loggedUser.fullName(), to_name).str(); }
			else { info << std::endl << "Введенный Вами получатель не найден (команда u - вывод списка)" << std::endl; }
		}
		else { info << std::endl << "Чтобы написать сообщение необходимо войти в систему (команда i)" << std::endl; }
		return info;
	}

	// ввод и отправка сообщения
	std::stringstream ChatBot::makeMessage(std::string from_name, std::string to_name)
	{
		std::stringstream info;
		std::string text;
		std::string name{ to_name.empty() ? "всех" : "пользователя [" + to_name + "]" };
		std::cout << std::endl << "Введите текст сообщения для " << name << ":" << std::endl;
		std::getline(std::cin, text);
		if (!text.empty())
		{
			TimeData time;	// фиксация времени отправки сообщения
			Message m{ text, from_name, to_name, time, true };
			m_messages.add(m);
			info << std::endl << std::endl << "Вы отправили сообщение:" << std::endl << m << std::endl;
		}
		else { info << std::endl << "Сообщение не отправлялось" << std::endl; }
		return info;
	}

	// поиск пользователя по совпадению с его логином или именем и, если он найден, внесение его данных в u
	bool ChatBot::name_OK(User& u)
	{
		size_t i;
		return m_users.nameFound(u, i);
	}

	// обработка ввода пользователя и внесение результата в param
	bool ChatBot::input_OK(std::string param_name, std::string& param)
	{
		bool result = false;
		std::string input;
		std::string word;
		std::cout << std::endl << "Введите " << param_name << ": ";
		std::getline(std::cin, input);
		input += " ";
		for (const char c : input)
		{
			if (c == ' ') { if (!word.empty()) break; }
			else { word += c; }
		}
		if (!word.empty())
		{
			param = word;
			result = true;
		}
		return result;
	}

	inline bool ChatBot::is_yes(std::string s)
	{
		return (s == "y" || s == "Y" || s == "yes" || s == "Yes" || s == "YES" || s == "да" || s == "Да" || s == "ДА");
	}

	// обработка ввода пользователя и формирование из него команды в установленном формате
	std::istream& operator>>(std::istream& in, CB_command& command)
	{
		std::string input;
		std::string p;
		std::getline(in, input);
		input += " ";
		command.type = CommandType::HELP;
		command.param.clear();
		bool gettype = true;
		for (const char c : input)
		{
			if (gettype)
			{
				if (c == '?') break;
				if (c == 'C' || c == 'c')
				{
					command.type = CommandType::CHAT;
					gettype = false;
				}
				if (c == 'I' || c == 'i')
				{
					command.type = CommandType::IN;
					gettype = false;
				}
				if (c == 'M' || c == 'm')
				{
					command.type = CommandType::MES;
					gettype = false;
				}
				if (c == 'O' || c == 'o')
				{
					command.type = CommandType::OUT;
					break;
				}
				if (c == 'Q' || c == 'q')
				{
					command.type = CommandType::QUIT;
					break;
				}
				if (c == 'R' || c == 'r')
				{
					command.type = CommandType::REG;
					break;
				}
				if (c == 'S' || c == 's')
				{
					command.type = CommandType::SHOW;
					gettype = false;
				}
				if (c == 'U' || c == 'u')
				{
					command.type = CommandType::USERS;
					break;
				}
			}
			else
			{
				if (c == ' ') { if (!p.empty()) break; }
				else { p += c; }
			}
		}
		if (!p.empty()) { command.param = p; }
		in.clear();
		return in;
	}

	// вывод команды в установленном формате
	std::ostream& operator<<(std::ostream& out, const CB_command& command)
	{
		out << static_cast<char>(command.type);
		if (!command.param.empty()) { out << " " << command.param; }
		return out;
	}
}