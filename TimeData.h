#pragma once
#ifdef OS_WIN
#pragma warning(disable : 4996)
#endif // OS_WIN
#include <ctime>
#include <chrono>
#include <sstream>
#include "vectorf.h"

namespace avhw18
{
	// класс используется для фиксации времени отправки сообщений
	class  TimeData
	{
	private:
		std::chrono::system_clock::time_point m_time;
		std::string m_format = "%d.%m.%Y %H:%M:%S";	// задание формата вывода по умолчанию

	public:
		TimeData();
		TimeData(std::chrono::system_clock::time_point);
		~TimeData();
		void setFormat(const std::string&);
		static std::vector<std::string> ts_converter(const TimeData&);
		static TimeData st_converter(const std::vector<std::string>&);
		std::string toString() const;
		void renew();
		void clear();
		TimeData& operator=(const TimeData&);
		friend std::ostream& operator<< (std::ostream&, const TimeData&);
		friend bool operator== (const TimeData&, const TimeData&);
		friend bool operator!= (const TimeData&, const TimeData&);
		friend bool operator< (const TimeData&, const TimeData&);
		friend bool operator> (const TimeData&, const TimeData&);
		friend bool operator<= (const TimeData&, const TimeData&);
		friend bool operator>= (const TimeData&, const TimeData&);
	};
}