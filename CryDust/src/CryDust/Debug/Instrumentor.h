#pragma once
#include <string>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <thread>


#include <mutex>
#include <sstream>
#include "CryDust/Core/Log.h"
namespace CryDust {


	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	//存储性能分析的结果，包括名称、开始和结束时间戳以及线程ID
	struct ProfileResult
	{
		std::string Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime; //运行时间
		std::thread::id ThreadID;
	};

	//存储当前的性能分析会话名称。
	struct InstrumentationSession
	{
		std::string Name;
	};

	/// <summary>
	/// 性能分析的核心类
	/// </summary>
	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				{
					CORE_DEBUG_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
				}
				InternalEndSession();
			}
			m_OutputStream.open(filepath);

			if (m_OutputStream.is_open())
			{
				m_CurrentSession = new InstrumentationSession({ name });
				WriteHeader();
			}
			else
			{
				if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				{
					CORE_DEBUG_ERROR("Instrumentor could not open results file '{0}'.", filepath);
				}
			}
		}

		void EndSession()
		{
			std::lock_guard lock(m_Mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}
		

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	private:
		Instrumentor()
			: m_CurrentSession(nullptr)
		{
		}
		~Instrumentor()
		{
			EndSession();
		}

		/// <summary>
		/// 写头部
		/// </summary>
		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_OutputStream.flush();
		}

		/// <summary>
		/// 写尾部
		/// </summary>
		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		///中间结束会话（）,之前必须拥有含mutex的锁
		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void InternalEndSession()
		{
			if (m_CurrentSession)
			{
				WriteFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}
	private:
		std::mutex m_Mutex;
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
	};

	/// <summary>
	/// 性能分析计时器
	/// </summary>
	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}
		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}
		void Stop()
		{
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;
	};

namespace InstrumentorUtils {

	template <size_t N>
	struct ChangeResult
	{
		char Data[N];
	};

	template <size_t N, size_t K>
	constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
	{
		ChangeResult<N> result = {};

		size_t srcIndex = 0;
		size_t dstIndex = 0;
		while (srcIndex < N)
		{
			size_t matchIndex = 0;
			while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
				matchIndex++;
			if (matchIndex == K - 1)
				srcIndex += matchIndex;
			result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
			srcIndex++;
		}
		return result;
	}
}
}


#define CD_PROFILE 0
#if CD_PROFILE
// Resolve which function signature macro will be used. Note that this only
	// is resolved when the (pre)compiler starts, so the syntax highlighting
	// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define CD_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define CD_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define CD_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define CD_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define CD_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define CD_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define CD_FUNC_SIG __func__
#else
#define CD_FUNC_SIG "CD_FUNC_SIG unknown!"
#endif



#define CD_PROFILE_BEGIN_SESSION(name, filepath) ::CryDust::Instrumentor::Get().BeginSession(name, filepath)
#define CD_PROFILE_END_SESSION() ::CryDust::Instrumentor::Get().EndSession()
#define CD_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::CryDust::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
											   ::CryDust::InstrumentationTimer timer##line(fixedName##line.Data)
#define CD_PROFILE_SCOPE_LINE(name, line) CD_PROFILE_SCOPE_LINE2(name, line)
#define CD_PROFILE_SCOPE(name) CD_PROFILE_SCOPE_LINE(name, __LINE__)
#define CD_PROFILE_FUNCTION() CD_PROFILE_SCOPE(__FUNCSIG__)    //方法名
#else
#define CD_PROFILE_BEGIN_SESSION(name, filepath)
#define CD_PROFILE_END_SESSION()
#define CD_PROFILE_SCOPE(name)
#define CD_PROFILE_FUNCTION()
#endif
