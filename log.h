#include <iostream>
#include <fstream>
#include <Windows.h>
#include <chrono>
#include <ctime>

enum class LogLevel
{
	DEBUG,
	RELEASE
};

class Log
{
private:
	static std::ofstream l_out;
	static LogLevel l_loglevel;

public:

	static void  SetLogLevel(LogLevel loglevel)
	{
		l_loglevel = loglevel;
	}

	static void SetLohPath(const std::string& path)
	{
		l_out.open(path);
	}


	static void Debug(const std::string& log)
	{
		if (l_loglevel == LogLevel::DEBUG)
		{
			auto start = std::chrono::system_clock::now();
			auto legacyStart = std::chrono::system_clock::to_time_t(start);
			char tmBuff[26];
			ctime_s(tmBuff, sizeof(tmBuff), &legacyStart);
			l_out << log << " - " << tmBuff << std::endl;
			l_out.flush();
		}
		else
		{
			l_out.flush();
		}
	}

	static void Release(const std::string& log)
	{
		if (l_loglevel == LogLevel::RELEASE)
		{
			auto start = std::chrono::system_clock::now();
			auto legacyStart = std::chrono::system_clock::to_time_t(start);
			char tmBuff[26];
			ctime_s(tmBuff, sizeof(tmBuff), &legacyStart);
			l_out << log << " - " << tmBuff << std::endl;
			l_out.flush();
		}
		else
		{
			l_out.flush();
		}
	}

};

std::ofstream Log::l_out;
LogLevel Log::l_loglevel;
