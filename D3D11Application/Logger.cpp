#include "Logger.h"

Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}

void Logger::addListener(Logger::Listener& listener)
{
    listeners.push_back(&listener);
}

void Logger::notifyListeners(const std::string& text)
{
    for (auto l : listeners)
    {
        l->onLog(text);
    }
}

void Logger::removeListener(Logger::Listener& listener)
{
    listeners.remove(&listener);
}
