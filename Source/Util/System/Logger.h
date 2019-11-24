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
