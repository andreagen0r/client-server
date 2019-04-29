#ifndef LOG_H
#define LOG_H

#include <string>
#include <fstream>
#include <iostream>

class Log
{
    std::string timestamp() const;

public:
    Log();
    void salvarLog(const std::string file_prefix, const size_t limit_size, const char *buffer);    
};

#endif // LOG_H
