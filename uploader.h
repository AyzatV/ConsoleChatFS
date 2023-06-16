#pragma once
// for Linux comment out next three lines!
//#ifndef OS_WIN
//#define OS_WIN
//#endif
#include <fstream>
#include <filesystem>
#include "TimeData.h"
namespace fs = std::filesystem;

namespace avhw18
{
	template <typename T>
	using ts_converter = std::function<std::vector<std::string>(const T&)>;
	
	template <typename T>
	using st_converter = std::function<T(const std::vector<std::string>&)>;
	
	template <typename T>
	class uploader
	{
	public:
		uploader();
		~uploader();
		bool data_saved(ts_converter<T>, const std::vectorf<T>&, const std::string&);
		bool data_loaded(st_converter<T>, std::vectorf<T>&, const size_t, const std::string&);
	};

	template<typename T>
	uploader<T>::uploader() {}

	template<typename T>
	uploader<T>::~uploader() {}

	template<typename T>
	bool uploader<T>::data_saved(ts_converter<T> converter, const std::vectorf<T>& data, const std::string& file_name)
	{
		bool result = false;
		if (fs::exists(file_name))
		{
			// удаление старого файла если он существует
			fs::permissions(file_name, fs::perms::owner_write, fs::perm_options::add);
			fs::remove(file_name);
		}
		std::ofstream ofs(file_name);
		if (ofs.is_open())
		{
			// запись элементов массива данных
			for (const T& datum : data)
			{
				// запись параметров элемента
				for (const auto& s : converter(datum)) { ofs << s << "\n"; }
			}
			ofs << std::endl;	// очистка буфера записи
			ofs.close();
			fs::permissions(file_name, fs::perms::owner_write | fs::perms::group_write | fs::perms::others_all, fs::perm_options::remove);
			result = true;
		}
		return result;
	}
	
	template<typename T>
	bool uploader<T>::data_loaded(st_converter<T> converter, std::vectorf<T>& data, const size_t quants, const std::string& file_name)
	{
		data.clear();
		std::ifstream ifs(file_name);
		if (ifs.is_open())
		{
			size_t n = 0;
			std::string s;
			std::vector<std::string> params;
			while (ifs)
			{
				n++;
				std::getline(ifs, s);
				params.push_back(s);
				if (n == quants)
				{
					data.push_back(converter(params));
					params.clear();
					n = 0;
				}
			}
			ifs.close();
		}
		return !data.empty();
	}
}