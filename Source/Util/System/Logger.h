#pragma once

#include <fstream>

namespace White {
namespace Util {

class Logger {
public:
    static Logger& GetInstance(); 
    void Init(std::string filename);
    void Close();
    template<typename T>
    void Log(T value);
    Logger& 
    operator<<(short value);
    Logger& 
    operator<<(unsigned short value);
    Logger& 
    operator<<(int value);
    Logger& 
    operator<<(unsigned int value);
    Logger& 
    operator<<(long value);
    Logger& 
    operator<<(unsigned long value);
    Logger& 
    operator<<(long long value);
    Logger& 
    operator<<(unsigned long long value);
    Logger& 
    operator<<(float value);
    Logger& 
    operator<<(double value);
    Logger& 
    operator<<(long double value);
    Logger& 
    operator<<(bool value);
    Logger& 
    operator<<(const void* value);
    Logger& 
    operator<<(std::nullptr_t);
    Logger&
    operator<<(std::string value);


protected:
    std::string filename;
    std::ofstream out; 

private:
    Logger();
};

template<typename T>
void Logger::Log(T value) {
    out.open(filename, std::ios::app);
    out << value << std::endl;
    out.close();
}

}
}
