#include "Logger.h"

namespace White {
namespace Util {

Logger& Logger::GetInstance() {
    static Logger logger;

    return logger;
}

void Logger::Init(std::wstring path) {
    this->path = path;
}

void Logger::Flush() {
    file.open(path);
    file.close();
}

template<typename T> void Logger::Log(T value) {
    file.open(path, std::ios::app);
    file << value;
    file.close();
}

template<typename T> Logger& Logger::operator<<(T value) {
    log(value)

    return *this;
}

std::ofstream& Logger::Out() {
    return file;
}

Logger::Logger() {}

}
}
