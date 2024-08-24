#pragma once
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <thread>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Singleton.h"
#include "ConcurrentQueue.h"
#include "ThreadPool.h"

namespace DSFramework {
	namespace DSComponent {
		const char* const kDefaultLogFileName = "log.txt";
		const size_t kDefaultMaxLogCount = 10;

		struct Log {
			enum class LogLv {
				DEBUGS = 0,
				INFO = 1,
				WARN = 2,
				ERRORS = 3,
				UNKNOWN = 4
			};

			LogLv m_lv = LogLv::UNKNOWN;
			std::string m_time;
			std::string m_threadID;
			std::string m_content;

			std::string m_fileName;
			std::string m_functionName;
			int m_line = -1;
			bool m_srcDetail = true;
			bool m_toConsole = true;

			Log(LogLv level,
				const std::string& content,
				const char* fileName,
				const char* functionName,
				int line,
				bool srcDetail = true,
				bool toConsole = true);

			Log(LogLv level,
				const char* content,
				const char* fileName,
				const char* functionName,
				int line,
				bool srcDetail = true,
				bool toConsole = true);

			static std::string GetCurrentThreadID();

			static std::string GetColorTextLogLv(LogLv lv);

			static std::string GetDefaultTextLogLv(LogLv lv);
		};

		class LogDumpTask : public std::enable_shared_from_this<LogDumpTask>
		{
			friend class Logger;
		private:
			std::vector<std::shared_ptr<Log>> m_dumpLogs;
			std::ofstream m_file;
			std::string m_fileName;
		public:
			LogDumpTask(size_t maxDumpSize, const std::string& fileName);
			virtual ~LogDumpTask();
			void Dump();
		private:
			std::shared_ptr<std::string> ToString(std::shared_ptr<Log> log);
		};

		class Logger : public Singleton<Logger>
		{
			friend Singleton<Logger>;
		private:
			ConcurrentQueue<std::shared_ptr<Log>> m_logs;
			std::shared_ptr<LogDumpTask> m_dumpTask;
			size_t m_maxLogCount;

			std::thread m_thread;
			bool m_isRunning;
		private:
			Logger();
			Logger(const Logger&) = delete;
			Logger& operator=(const Logger&) = delete;
			Logger(Logger&&) = delete;

			std::shared_ptr<std::string> ToString(std::shared_ptr<Log> log);
		public:
			virtual ~Logger();
		public:
			void LogMessage(Log::LogLv level, std::string content,
				const char* fileName, const char* functionName, int line, bool srcDetail = true, bool toConsole = true);

			void LogMessage(Log::LogLv level, const char* content,
				const char* fileName, const char* functionName, int line, bool srcDetail = true, bool toConsole = true);
		};

#define LOG_DEBUG_CONSOLE_DETAIL(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::DEBUGS, CONTENT, __FILE__, __FUNCTION__, __LINE__, true, true)
#define LOG_DEBUG_CONSOLE(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::DEBUGS, CONTENT,"","", 0, false, true)
#define LOG_DEBUG_FILE_DETAIL(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::DEBUGS, CONTENT, __FILE__, __FUNCTION__, __LINE__, true, false)
#define LOG_DEBUG_FILE(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::DEBUGS, CONTENT,"","", 0, false, false)

#define LOG_INFO_CONSOLE_DETAIL(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::INFO, CONTENT, __FILE__, __FUNCTION__, __LINE__, true, true)
#define LOG_INFO_CONSOLE(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::INFO, CONTENT,"","", 0, false, true)
#define LOG_INFO_FILE_DETAIL(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::INFO, CONTENT, __FILE__, __FUNCTION__, __LINE__, true, false)
#define LOG_INFO_FILE(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::INFO, CONTENT,"","", 0, false, false)

#define LOG_WARN_CONSOLE_DETAIL(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::WARN, CONTENT, __FILE__, __FUNCTION__, __LINE__, true, true)
#define LOG_WARN_CONSOLE(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::WARN, CONTENT,"", "", 0, false, true)
#define LOG_WARN_FILE_DETAIL(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::WARN, CONTENT, __FILE__, __FUNCTION__, __LINE__, true, false)
#define LOG_WARN_FILE(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::WARN, CONTENT, "", "", 0, false, false)

#define LOG_ERROR_CONSOLE_DETAIL(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::ERRORS, CONTENT, __FILE__, __FUNCTION__, __LINE__, true, true)
#define LOG_ERROR_CONSOLE(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::ERRORS, CONTENT, "", "", 0, false, true)
#define LOG_ERROR_FILE_DETAIL(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::ERRORS, CONTENT, __FILE__, __FUNCTION__, __LINE__, true, false)
#define LOG_ERROR_FILE(CONTENT) Logger::GetInstance()->LogMessage(Log::LogLv::ERRORS, CONTENT, "", "", 0, false, false)
	}
}
