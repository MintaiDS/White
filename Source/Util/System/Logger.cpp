#include "Logger.h"

#include <sstream>

namespace White {
namespace Util {

Logger::Logger() {}

Logger& Logger::GetInstance() {
    static Logger loggerInstance;

    return loggerInstance;
}

void Logger::Init(std::string filename) {
    remove(filename.c_str());
    this->filename = filename;
}

void Logger::Close() {
    out.close();
}

Logger& 
Logger::operator<<(short value) {
    out.open(filename, std::ios::app);
    out << value << std::endl;
    out.close();

    return *this;
}

Logger& 
Logger::operator<<(unsigned short value) {
    out.open(filename, std::ios::app);
    out << value << std::endl;
    out.close();

    return *this;
}

Logger& 
Logger::operator<<(int value) {
    out.open(filename, std::ios::app);
    out << value << std::endl;
    out.close();

    return *this;
}

Logger& 
Logger::operator<<(unsigned int value) {
    out.open(filename, std::ios::app);
    out << value << std::endl;
    out.close();

    return *this;
}

Logger& 
Logger::operator<<(long value) {
    out.open(filename, std::ios::app);
    out << value << std::endl;
    out.close();

    return *this;
}

Logger& 
Logger::operator<<(unsigned long value) {
    out.open(filename, std::ios::app);
    out << value << std::endl;
    out.close();

    return *this;
}

Logger& 
Logger::operator<<(long long value) {
    out.open(filename, std::ios::app);
    out << value << std::endl;
    out.close();

    return *this;
}

Logger& 
Logger::operator<<(unsigned long long value) {
    out.open(filename, std::ios::app);
    out << value << std::endl;
    out.close();

    return *this;
}

Logger& 
Logger::operator<<(float value) {
    out.open(filename, std::ios::app);
    out << value << std::endl;
    out.close();

    return *this;
}

Logger& 
Logger::operator<<(double value) {
    out.open(filename, std::ios::app);
    out << value << std::endl;
    out.close();

    return *this;
}

Logger& 
Logger::operator<<(long double value) {
    out.open(filename, std::ios::app);
    out << value << std::endl;
    out.close();

    return *this;
}

Logger& 
Logger::operator<<(bool value) {
    out.open(filename, std::ios::app);
    out << value << std::endl;
    out.close();

    return *this;
}

Logger& 
Logger::operator<<(const void* value) {
    out.open(filename, std::ios::app);
    out << value << std::endl;
    out.close();

    return *this;
}

Logger& 
Logger::operator<<(std::nullptr_t) {
    return *this;
}

//Logger& 
//Logger::operator<<(std::basic_streambuf<char, std::char_traits<char>>* sb) {
//    out.open(filename, std::ios::app);
//    auto buf = sb.rdbuf();
//    out.rdbuf();
//    out.close();
//
//    return *this;
//}

Logger& 
Logger::operator<<(std::string value) {
    out.open(filename, std::ios::app);
    std::locale locale("");
    std::stringstream stream(value);
    out.imbue(locale);
    out << stream.str();
    out.close();

    return *this;
}



}
}
