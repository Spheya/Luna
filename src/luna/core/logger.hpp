#pragma once

#include <iostream>
#include <functional>
#include <format>
#include <utility>

#ifdef LUNA_DISABLE_LOGGER
#deifne LUNA_LOG_LEVEL 0
#endif

#ifndef LUNA_LOG_LEVEL
#define LUNA_LOG_LEVEL 4 // 0 = no logs, 1 = errors only, 2 = warnings, 3 = info, 4 = trace
#endif

namespace luna {

	enum class MessageSeverity : uint8_t {
		Trace   = 0,
		Info    = 1,
		Warning = 2,
		Error   = 3
	};

	void setMessageCallback(const std::function<void(std::string_view message, MessageSeverity severity)>& callback);
	void sendMessage(std::string_view message, MessageSeverity severity);

	template<MessageSeverity Severity = MessageSeverity::Info>
	inline void log(std::string_view message) {
		if constexpr (uint8_t(Severity) > 3 - LUNA_LOG_LEVEL)
			sendMessage(message, Severity);
	}

	template<MessageSeverity Severity = MessageSeverity::Info>
	inline void log(const std::string& message) {
		if constexpr (uint8_t(Severity) > 3 - LUNA_LOG_LEVEL)
			sendMessage(message, Severity);
	}

	template<MessageSeverity Severity = MessageSeverity::Info>
	inline void log(const char* message) {
		if constexpr (uint8_t(Severity) > 3 - LUNA_LOG_LEVEL)
			sendMessage(message, Severity);
	}

	template<MessageSeverity Severity = MessageSeverity::Info, typename ...T>
	inline void log(std::format_string<T...> message, T&&... args) {
		if constexpr (uint8_t(Severity) > 3 - LUNA_LOG_LEVEL)
			sendMessage(std::format(message, std::forward<T>(args)...), Severity);
	}

	inline void trace(std::string_view message) { log<MessageSeverity::Trace>(message); }
	inline void trace(const std::string& message) { log<MessageSeverity::Trace>(message); }
	inline void trace(const char* message) { log<MessageSeverity::Trace>(message); }
	template<typename ...T> inline void trace(const std::format_string<T...> message, T&&... args) { log<MessageSeverity::Trace>(message, std::forward<T>(args)...); }

	inline void warn(std::string_view message) { log<MessageSeverity::Warning>(message); }
	inline void warn(const std::string& message) { log<MessageSeverity::Warning>(message); }
	inline void warn(const char* message) { log<MessageSeverity::Warning>(message); }
	template<typename ...T> inline void warn(const std::format_string<T...> message, T&&... args) { log<MessageSeverity::Warning>(message, std::forward<T>(args)...); }

	inline void error(std::string_view message) { log<MessageSeverity::Error>(message); }
	inline void error(const std::string& message) { log<MessageSeverity::Error>(message); }
	inline void error(const char* message) { log<MessageSeverity::Error>(message); }
	template<typename ...T> inline void error(const std::format_string<T...> message, T&&... args) { log<MessageSeverity::Error>(message, std::forward<T>(args)...); }

}