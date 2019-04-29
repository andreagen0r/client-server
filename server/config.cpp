#include "config.h"

#include <iostream>
#include <chrono>
#include <thread>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace bfs = boost::filesystem;
namespace bpt = boost::property_tree;

Config::Config() :
    Config("config.json")
{
    // Contrutor delegado para o construtor com parâmetro
}

Config::Config(const std::string &in_filename) :
    m_port(9090),
    m_max_fileSize(25),
    m_is_open(false),
    m_host("127.0.0.1"),
    m_config_filename(in_filename),
    m_prefix_filename("LOG_"),
    m_timeout(20)
{    
    bfs::ifstream in;
    in.open(in_filename);

    if(in.is_open())
    {
        m_is_open = true;

        // lê os dados do arquivo e seta as variáveis
        loadConfig(in_filename);
    }
    else
    {
        m_is_open = false;
        makeDefaultConfig();
    }

    in.clear();
    in.close();
}

unsigned short Config::port() const
{
    return m_port;
}

void Config::setPort(const unsigned short &port)
{
    m_port = port;
}

size_t Config::maxFileSize() const
{
    return m_max_fileSize;
}

void Config::setMaxFileSize(const size_t &fileSize)
{
    m_max_fileSize = fileSize;
}

bool Config::isOpen() const
{
    return m_is_open;
}

void Config::saveConfig()
{
    bpt::ptree root;

    root.put("host", m_host);
    root.put("port", m_port);
    root.put("maxFileSize", m_max_fileSize);
    root.put("filePrefix", m_prefix_filename);
    root.put("timeout", m_timeout);

    bpt::write_json(m_config_filename, root);
}

std::string Config::host() const
{
    return m_host;
}

void Config::setHost(const std::string &host)
{
    m_host = host;
}

std::string Config::fileNamePrefix() const
{
    return m_prefix_filename;
}

void Config::setFileNamePrefix(const std::string &fileNamePrefix)
{
    m_prefix_filename = fileNamePrefix;
}

std::string Config::filename() const
{
    return m_config_filename;
}

int Config::timeout() const
{
    return m_timeout;
}

void Config::setTimeout(const int &time)
{
    m_timeout = time;
}

void Config::loadConfig(const std::string &in_filename)
{
    bpt::ptree root;
    bpt::read_json(in_filename, root);

    // Apenas para debug
//    bpt::write_json(std::cout, roots);

    m_host = root.get<std::string>("host");
    m_port = root.get<unsigned short>("port");
    m_max_fileSize = root.get<size_t>("maxFileSize");
    m_prefix_filename = root.get<std::string>("filePrefix");
    m_timeout = root.get<int>("timeout");
}

// Cria um arquivo config.json com os valores padrões
void Config::makeDefaultConfig()
{
    // Cria o root do objeto json
    bpt::ptree root;

    // carrega os dados padrões que estão no arquivo de configurações.
    root.put("host", m_host);
    root.put("port", m_port);
    root.put("maxFileSize", m_max_fileSize);
    root.put("filePrefix", m_prefix_filename);
    root.put("timeout", m_timeout);

    // Escreve o arquivo com os dados de configuração padrão
    bpt::write_json(m_config_filename, root);
}

