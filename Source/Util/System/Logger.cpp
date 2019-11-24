#include "Logger.h"

namespace White {
namespace Util {

Logger::Logger() {}

Logger& Logger::GetInstance() {
    static Logger loggerInstance;

    return loggerInstance;
}

void Logger::Init(std::string filename) {
    this->filename = filename;
}

}
}
