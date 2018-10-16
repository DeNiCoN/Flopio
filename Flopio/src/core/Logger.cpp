#include "Logger.h"
namespace engine {
	Logger::~Logger() {
		outputVector.clear();
	}
	Logger::Logger(std::ostream* stream) {
		outputVector.push_back(stream);
	}
	Logger::Logger(std::vector<std::ostream*> out) : outputVector(out) {};

	void Logger::addStream(std::ostream* stream) {
		outputVector.push_back(stream);
	}

	void Logger::removeStream(std::ostream* stream) {
		for (auto it = outputVector.begin(); it != outputVector.end(); it++) {
			if (stream == *it) {
				outputVector.erase(it);
				break;
			}
		}
	}
}

