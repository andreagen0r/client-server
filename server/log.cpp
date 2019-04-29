#include "log.h"

#include <iostream>
#include <fstream>
#include <ctime>

Log::Log()
{

}

std::string Log::timestamp() const
{
    std::time_t now = std::time(nullptr);
    char timestamp[100];
    std::strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", std::localtime(&now));
    return static_cast<std::string>(timestamp);
}

void Log::salvarLog(const std::string file_prefix, const size_t limit_size, const char *buffer)
{
    std::cout << "Tamanho do buffer: " << strlen(buffer) << " bytes" << std::endl;
    std::cout << "Limite do buffer : " << limit_size << " bytes" << std::endl;

    std::string bufferBlock(buffer);

    long long contador{0};

    while(bufferBlock.size() != 0) {
        auto output_filename = file_prefix + timestamp() + "_" + std::to_string(contador) + ".txt";

        std::ofstream out;
        out.open(output_filename, std::ios::app);
        if(out.is_open()) {
            out << bufferBlock.substr(0, limit_size);
            out.close();
        }else {
            std::cout << "Não foi possível escrever no arquivo" << std::endl;
        }

        if(bufferBlock.size() > limit_size){
            ++contador;
        }

        bufferBlock.erase(0, limit_size);
    }

}

