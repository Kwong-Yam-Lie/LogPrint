//
// Created by gyli on 23-11-7.
//
#ifndef LOGPRINT__MINILOG_H_
#define LOGPRINT__MINILOG_H_

#include <format.h>
#include <source_location>
#include <iostream>
#include<utility>

namespace minilog
{

// X-MACRO technique. A special MACRO method parameter named x.
// Begin*****X-MACRO.
#define MINILOG_FOREACH_log_level(f) \
    f(trace) \
    f(debug) \
    f(info) \
    f(critical) \
    f(warning) \
    f(error) \
    f(fatal)

	enum class log_level : std::uint8_t
	{ // The scope of enum is 'log_level'.
#define _FUNCTION(name) name,
		MINILOG_FOREACH_log_level(_FUNCTION)
#undef _FUNCTION
	};

	namespace details
	{
#if defined(__linux__) || defined(__APPLE__)
		inline char level_ansi_colors[(std::uint8_t)log_level::fatal + 1][6] = { // Colors configuration.
			"37m",
			"35m",
			"32m",
			"34m",
			"33m",
			"31m",
			"31;1m",
		};
#define MINILOG_IF_HAS_ANSI_COLORS(x) x
#else
#define MINILOG_IF_HAS_ANSI_COLORS(x)
#endif

		static std::string log_level_name(log_level lev) { // Because of the existence of ODR, static or inline must be added here. Template function is inline by default.
			switch (lev)
			{
#define _FUNCTION(name) case log_level::name:return #name;
			MINILOG_FOREACH_log_level(_FUNCTION)
#undef _FUNCTION
			}
			return "unknown";
		}
// End*****X-MACRO.
// Result preview:<gcc -E main.cpp -I "path" -L "path" -lname | less> && <End> or <Shift + G>.

		template<class T>
		class with_source_location
		{
		 private:
			using SL = std::source_location;
			T inner;
			SL loc;

		 public:
			template<class U>
			requires std::constructible_from<T, U> // constraint : T can definitely be constructed from U.
			// _inner is the literal format string, T is the 'fmt::format_string<Args...>'. Eg:const char literal -> string.
			consteval with_source_location(U&& _inner,
				SL _loc = SL::current()) // This function is only used at compile time.
				:inner(std::forward<U>(_inner)), loc(std::move(_loc)) {
			}
			[[nodiscard]] constexpr T const& fmt() const {
				return inner;
			}
			[[nodiscard]] constexpr SL const& location() const {
				return loc;
			}
		};

		inline log_level g_max_level =
			log_level::debug; // `inline` is used for global scope needs. And `g` prefix based google code style.


		template<typename ...Args>
		void generic_log(log_level lev, with_source_location<fmt::format_string<Args...>> fmt, Args&& ...args) {
			// The consteval constructor is called during parameter passing.
			if (lev >= g_max_level)
			{
				auto const& loc = fmt.location();
				std::cout MINILOG_IF_HAS_ANSI_COLORS(<< "\E[" <<details::level_ansi_colors[(std::uint8_t)lev])
					<< loc.file_name() << ":" << loc.line() << "[" << log_level_name(lev) << "]"
					<< vformat(fmt.fmt(), fmt::make_format_args(args...))
					MINILOG_IF_HAS_ANSI_COLORS(<< "\E[m") << "\n";
			}
		}
	}

	inline log_level set_log_level(log_level lev) {
		std::exchange(details::g_max_level, lev);
	}

// How to design an upgrade of generic_log? Here is.
#define _FUNCTION(name) \
template<typename... Args> \
void log_##name(details::with_source_location<fmt::format_string<Args...>> fmt, Args&& ...args) { \
    return generic_log(log_level::name, std::move(fmt), std::forward<Args>(args)...); \
}
	MINILOG_FOREACH_log_level(_FUNCTION)
#undef _FUNCTION

#define MINILOG_P(x) ::minilog::log_debug(#x "={}", x)
// C++ macro definitions are not affected by namespaces.
// Macro definitions are expanded during the preprocessing phase, while namespaces are active during the compilation phase.
// Therefore, macro definitions are expanded during preprocessing without being affected by the namespace.
// the `::` of `::minilog` must be added.

}

#endif // LOGPRINT__MINILOG_H_

