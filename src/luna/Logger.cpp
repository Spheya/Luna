#include "Logger.hpp"

#include <memory>

namespace luna {

	namespace {
		std::function<void(const char* message, const char* prefix, MessageSeverity severity)> messageCallback;
	}

	void setMessageCallback(const std::function<void(const char* message, const char* prefix, MessageSeverity severity)>& callback) {
		messageCallback = callback;
	}

	void log(const std::string& message, MessageSeverity severity) {
		if (messageCallback) {
			constexpr const char* prefixes[3] = {
				"Info", "Warning", "Error"
			};

			const char* prefix = prefixes[unsigned char(severity)];
			messageCallback(message.c_str(), prefix, severity);
		}
	}

}