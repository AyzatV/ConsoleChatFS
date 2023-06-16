#include "TimeData.h"

namespace avhw18
{
	TimeData::TimeData() : m_time(std::chrono::system_clock::now()) {}	// при создании фиксируется текущее системное время
	TimeData::TimeData(std::chrono::system_clock::time_point tp) : m_time(tp) {}
	TimeData::~TimeData() {}

	//std::string TimeData::m_format = "%d.%m.%Y %H:%M:%S";	// задание формата вывода по умолчанию

	// установка формата вывода
	void TimeData::setFormat(const std::string& format) { m_format = format; }

	// преобразование в массив строк для хранения данных
	std::vector<std::string> TimeData::ts_converter(const TimeData& time)
	{
		std::vector<std::string> v;
		TimeData td = time;
		std::string f[]{ "%d","%m","%Y","%H","%M","%S" };
		for (size_t i = 0; i < 6; i++)
		{
			td.m_format = f[i];
			v.push_back(td.toString());
		}
		return v;
	}

	// восстановление из массива строк
	TimeData TimeData::st_converter(const std::vector<std::string>& params)
	{
		std::tm td{};
		td.tm_mday = std::stoi(params.at(0));
		td.tm_mon = std::stoi(params.at(1)) - 1;
		td.tm_year = std::stoi(params.at(2)) - 1900;
		td.tm_hour = std::stoi(params.at(3));
		td.tm_min = std::stoi(params.at(4));
		td.tm_sec = std::stoi(params.at(5));
		auto t = std::mktime(&td);
		return TimeData(std::chrono::system_clock::from_time_t(t));
	}

	// преобразование в строку
	std::string TimeData::toString() const
	{
		std::stringstream ss;
		ss << *this;
		return ss.str();
	}

	// перезапись текущего системного времени
	void TimeData::renew() { m_time = std::chrono::system_clock::now(); }

	// установка начального "нулевого" значения
	void TimeData::clear() { m_time = std::chrono::time_point<std::chrono::system_clock>(); }

	// реализация оператора присваивания
	TimeData& TimeData::operator=(const TimeData& t)
	{
		if (&t != this) { m_time = t.m_time; }
		return *this;
	}

	// вывод в поток с преобразованием к заданному формату
	std::ostream& operator<<(std::ostream& out, const TimeData& t)
	{
		auto tt = std::chrono::system_clock::to_time_t(t.m_time);
		auto tm = *std::localtime(&tt);
		out << std::put_time(&tm, t.m_format.c_str());
		return out;
	}

	// далее реализация операторов сравнения

	bool operator==(const TimeData& t1, const TimeData& t2) { return (t1.m_time == t2.m_time); }

	bool operator!=(const TimeData& t1, const TimeData& t2) { return !(t1 == t2); }

	bool operator<(const TimeData& t1, const TimeData& t2) { return (t1.m_time < t2.m_time); }

	bool operator>(const TimeData& t1, const TimeData& t2) { return (t1.m_time > t2.m_time); }

	bool operator<=(const TimeData& t1, const TimeData& t2) { return !(t1 > t2); }

	bool operator>=(const TimeData& t1, const TimeData& t2) { return !(t1 < t2); }
}
