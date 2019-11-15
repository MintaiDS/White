#pragma once

#include <string>
#include <fstream>

namespace White {
namespace Util {

class Logger {
public:
    static Logger& GetInstance();
    void Init(std::wstring path);
    void Flush();
    template<typename T> void Log(T value);
    template<typename T> Logger& operator<<(T value);
    std::ofstream& Out();

private:
    Logger();
    
    std::wstring path; 
    std::ofstream file;
};

}
}
