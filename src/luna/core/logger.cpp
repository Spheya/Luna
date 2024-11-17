#include "Logger.hpp"

#include <memory>

namespace luna {

	namespace {
		std::function<void(std::string_view message, MessageSeverity severity)> messageCallback;
	}

	void setMessageCallback(const std::function<void(std::string_view message, MessageSeverity severity)>& callback) {
		messageCallback = callback;
	}

	void sendMessage(std::string_view message, MessageSeverity severity) {
		if (messageCallback)
			messageCallback(message, severity);
	}

}