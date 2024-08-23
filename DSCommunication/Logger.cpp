#include "Logger.h"


namespace DSFramework {
	namespace DSComponent {
		Log::Log(LogLv level, const std::string& content,
			const char* fileName,
			const char* functionName,
			int line,
			bool srcDetail,
            bool toConsole)
			:m_lv(level), 
            m_content(content),
            m_fileName(fileName), 
            m_functionName(functionName),
            m_line(line), 
            m_srcDetail(srcDetail),
            m_toConsole(toConsole)
		{
			m_time = boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
			m_threadID = GetCurrentThreadID();
		}

        Log::Log(LogLv level,
            const char* content,
            const char* fileName,
            const char* functionName,
            int line,
            bool srcDetail,
            bool toConsole) :
            m_lv(level),
			m_content(content),
			m_fileName(fileName),
			m_functionName(functionName),
			m_line(line),
			m_srcDetail(srcDetail),
			m_toConsole(toConsole)
		{
			m_time = boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
			m_threadID = GetCurrentThreadID();
		}

        std::string Log::GetCurrentThreadID() {
            std::ostringstream oss;
            oss << std::this_thread::get_id();
            return oss.str();
        }

        std::string Log::GetColorTextLogLv(LogLv lv)
        {
            std::string level_str;
            std::string color_code;
            switch (lv)
            {
            case LogLv::DEBUGS:
                level_str = "DEBUGS";
				color_code = "\033[36m"; // cyan
                break;
            case LogLv::INFO:
                level_str = "INFO";
				color_code = "\033[32m"; // green
                break;
            case LogLv::WARN:
                level_str = "WARN";
				color_code = "\033[33m"; // yellow
                break;
            case LogLv::ERRORS:
                level_str = "ERRORS";
				color_code = "\033[31m"; // red
                break;
            default:
                level_str = "UNKNOWN";
				color_code = "\033[37m"; // white
            }
            std::string reset_code = "\033[0m";
            return color_code + "[" + level_str + "] " + reset_code;
        }

        std::string Log::GetDefaultTextLogLv(LogLv lv)
        {
            std::string level_str;
            switch (lv)
            {
            case LogLv::DEBUGS:
                level_str = "DEBUGS";
                break;
            case LogLv::INFO:
                level_str = "INFO";
                break;
            case LogLv::WARN:
                level_str = "WARN";
                break;
            case LogLv::ERRORS:
                level_str = "ERRORS";
                break;
            default:
                level_str = "UNKNOWN";
            }
            return "[" + level_str + "] ";
        }
	}

    namespace DSComponent {
		LogDumpTask::LogDumpTask(size_t maxDumpSize, const std::string& fileName):
           m_fileName(fileName)
        {
            m_dumpLogs.reserve(maxDumpSize);
        }

		LogDumpTask::~LogDumpTask() 
        {
			if (m_file.is_open())
				m_file.close();
        }

        void LogDumpTask::Dump()
        {
			if (m_dumpLogs.empty())
				return;
			m_file.open(m_fileName, std::ios::out | std::ios::app);
            if (m_file.is_open())
            {
				for (auto& log : m_dumpLogs)
                {
					m_file << *ToString(log);
				}
            }
            m_file.close();
			std::vector<std::shared_ptr<Log>>().swap(m_dumpLogs);
        }

        std::shared_ptr<std::string> LogDumpTask::ToString(std::shared_ptr<Log> log)
        {
            std::ostringstream oss;
            if (log->m_srcDetail)
                oss << Log::GetDefaultTextLogLv(log->m_lv) << " " << log->m_time << " " << log->m_threadID << " " << log->m_content << " " <<
                log->m_fileName << " " << log->m_functionName << " " << log->m_line << "\n";
            else
                oss << Log::GetDefaultTextLogLv(log->m_lv) << " " << log->m_time << " " << log->m_threadID << " " << log->m_content << "\n";
            return std::make_shared<std::string>(oss.str());
        }
    
    
		Logger::Logger() :
			m_maxLogCount(kDefaultMaxLogCount)
        {
            m_isRunning = true;
			m_dumpTask = std::make_shared<LogDumpTask>(kDefaultMaxLogCount, kDefaultLogFileName);
            m_thread = std::thread([this] {
                while (m_isRunning)
                {
                    auto log = *(m_logs.WaitPop());
                    
                    if (log->m_toConsole)
                    {
                        std::cout << ToString(log)->c_str();
                    }
                    else
                    {
                        if (m_dumpTask->m_dumpLogs.size() < m_maxLogCount) {
							m_dumpTask->m_dumpLogs.push_back(log);
                        }
                        else
                        {
							m_dumpTask->Dump();
							m_dumpTask = std::make_shared<LogDumpTask>(m_maxLogCount, kDefaultLogFileName);
                        }
                    }
                }
            });
        }

		Logger::~Logger()
		{
            if (m_isRunning)
            {
				m_isRunning = false;
                std::shared_ptr<Log> quit_log = std::make_shared<Log>(Log::LogLv::INFO, "Logger quit.", "", "", 0, false, true);
				m_logs.Push(quit_log);
            }
            this->m_thread.join();
			if (m_dumpTask)
				m_dumpTask->Dump();
		}

        std::shared_ptr<std::string> Logger::ToString(std::shared_ptr<Log> log) 
        {
            std::ostringstream oss;
            if (log->m_srcDetail)
               oss << Log::GetColorTextLogLv(log->m_lv) << " " << log->m_time << " " << log->m_threadID << " " << log->m_content << " " <<
               log->m_fileName << " " << log->m_functionName << " " << log->m_line << "\n";
            else
               oss << Log::GetColorTextLogLv(log->m_lv) << " " << log->m_time << " " << log->m_threadID << " " << log->m_content << "\n";
            return std::make_shared<std::string>(oss.str());
        }

		void Logger::LogMessage(Log::LogLv level, std::string content,
			const char* fileName,const char* functionName,int line,
			bool srcDetail,bool toConsole)
		{
			std::shared_ptr<Log> log = std::make_shared<Log>(level, content, fileName, functionName, line, srcDetail, toConsole);
			m_logs.Push(log);
		}

        void Logger::LogMessage(Log::LogLv level, const char* content,
            const char* fileName, const char* functionName, int line, 
            bool srcDetail, bool toConsole)
        {
            std::shared_ptr<Log> log = std::make_shared<Log>(level, content, fileName, functionName, line, srcDetail, toConsole);
            m_logs.Push(log);
        }
    }
}