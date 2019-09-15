#pragma once

#include <string>
#include <list>
#include <iostream>
#include <sstream>

class Logger
{
public:
    class Listener
    {
    public:
        virtual void onLog(const std::string& text) = 0;
    };

    static Logger& instance();

    void addListener(Listener& listener);
    void removeListener(Listener& listener);

    template <typename T>
    Logger& operator << (T value)
    {
        std::cout << value;

        std::stringstream str;
        str << value;
        notifyListeners(str.str());

        return *this;
    }

private:
    Logger() {}

    void notifyListeners(const std::string& text);

private:
    std::list<Listener*> listeners;
};

#define LOG Logger::instance()