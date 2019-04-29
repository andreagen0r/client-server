#include <iostream>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>

#include "config.h"
#include "log.h"

using namespace boost::asio;

int main()
{
    Config m_conf;
    Log m_log;

    bool timeout{false};

    // Pode sobreescrever os valores do arquivo de config
    //    conf.setPort(9090);
    //    conf.setMaxFileSize(25);
    //    conf.setHost("127.0.0.1");
    //    m_conf.setFileNamePrefix("MEULOG_");
    //    m_conf.saveConfig(); // Necessário chamar esse método para salvar os valores

    const unsigned short port{m_conf.port()};
    const std::string host{m_conf.host()};
    const size_t max_file_size{m_conf.maxFileSize()};
    const std::string file_prefix{m_conf.fileNamePrefix()};
    const int m_timeout{m_conf.timeout()};

    std::cout << "Dados do servidor para conexão: \n"
              << "Host: " << host << "\n"
              << "Port: " << port << std::endl;

    io_service ioservice;
    ip::tcp::endpoint endpoint(ip::address::from_string(host), port);
    ip::tcp::acceptor acceptor(ioservice, endpoint);

    while (!timeout) {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        // cria um novo ponteiro de socket e passa para o acceptor aceitar a conexão.
        boost::shared_ptr<ip::tcp::socket> socket(new ip::tcp::socket(ioservice));
        acceptor.accept(*socket);


        // Cria uma nova thead e passa uma lambda expression
        boost::thread m_thread ([&, socket, max_file_size, file_prefix] {

            // Imprime mensagem no servidor informando o endereço do cliente que fez a conexão
            std::cout << "Conexao solicitade pelo cliente: " << socket->remote_endpoint() << '\n';

            // Cria mensagem que será enviada para o cliente
            const char *msg = "Conexão estabelecida com o servidor";

            try {
                boost::system::error_code error_code;
                char recvbuf[BUFSIZ];
                auto size = read(*socket, buffer(recvbuf, BUFSIZ), error_code);

                if (!error_code || error_code == error::eof) {
                    recvbuf[size] = 0;
                    std::cout << "Mensagem recebida: " << recvbuf << '\n';
                    write(*socket, const_buffer(msg, strlen(msg)));

                    // Salva o log dos dados recebidos
                    m_log.salvarLog(file_prefix, max_file_size, recvbuf);

                    socket->close();
                }

            } catch (std::exception& e) {
                std::cout << "Erro encontrado " << e.what() << std::endl;
            }
        });

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        if(time_span.count() >= m_timeout){
            timeout = true;
            m_thread.detach();
        }

    }

    if(timeout){
        std::cout << "\n" << "O tempo da conexão expirou." << std::endl;
    }
}



