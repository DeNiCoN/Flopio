#pragma once
#include <iostream>
#include <vector>
namespace engine {

	class Logger {
	private:
		std::vector<std::ostream*> outputVector;
	public:
		Logger() = default;;
		~Logger();
		Logger(std::ostream* stream);
		Logger(std::vector<std::ostream*> out);
		void addStream(std::ostream* stream);
		void removeStream(std::ostream* stream);

		template<typename T>
		Logger& operator<<(const T& type) {
			for (std::ostream *out : outputVector) {
				*out << type;
			}
			return *this;
		}
	};
}
