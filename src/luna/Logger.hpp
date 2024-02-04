#pragma once

#include <functional>
#include <string>

namespace luna {

	enum class MessageSeverity : unsigned char {
		Info = 0, Warning, Error
	};

	void setMessageCallback(const std::function<void(const char* message, const char* prefix, MessageSeverity severity)>& callback);
	void log(const std::string& message, MessageSeverity severity);

}