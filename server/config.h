#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config
{
    unsigned short m_port;
    size_t m_max_fileSize;
    bool m_is_open;
    std::string m_host;
    std::string m_config_filename;
    std::string m_prefix_filename;
    int m_timeout;

    void loadConfig(const std::string &in_filename);
    void makeDefaultConfig();


public:
    Config();
    Config(const std::string &in_filename);

    unsigned short port() const;
    void setPort(const unsigned short &port);

    size_t maxFileSize() const;
    void setMaxFileSize(const size_t &fileSize);

    bool isOpen() const;
    void saveConfig();

    std::string host() const;
    void setHost(const std::string &host);

    std::string fileNamePrefix() const;
    void setFileNamePrefix(const std::string &fileNamePrefix);

    std::string filename() const;

    int timeout() const;
    void setTimeout(const int &time);
};

#endif // CONFIG_H
