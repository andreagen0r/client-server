#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using namespace boost::asio;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port\n";
        exit(1);
    }

    const char *host = argv[1];
    const char *port = argv[2];

    io_service ioservice;
    ip::tcp::resolver resolver(ioservice);
    try {
        ip::tcp::resolver::query query(ip::tcp::v4(), host, port);
        ip::tcp::resolver::iterator end, it = resolver.resolve(query);

        ip::tcp::endpoint server(it->endpoint());
        std::cout << "Solicitando conexão ao servidor: " << server << '\n';
        std::cout << "Digite \":exit\" para sair " << '\n';
        ip::tcp::socket socket(ioservice, ip::tcp::v4());

        std::string message;

        do {
            std::cout << "Digite sua mensagem: ";
            std::getline(std::cin, message);

            if(message == ":exit") {
                socket.close();
                break;
            }

            socket.connect(server);
            write(socket, buffer(message.c_str(),message.size()));
            socket.shutdown(ip::tcp::socket::shutdown_send);

            char msg[BUFSIZ];
            boost::system::error_code error_code;
            size_t size = read(socket, buffer(msg, BUFSIZ), error_code);

            if (!error_code || error_code == error::eof) {
                msg[size] = 0;
                std::cout << msg << '\n'; // Resposta do servidor
            }
            else {
                std::cerr << "Erro lendo resposta do servidor: "<< error_code.message() << '\n';
            }

            socket.close();

        } while(true);

    } catch (std::exception& e) {
        std::cerr << "Erro encontrado: " << e.what() << '\n';
    }

  return 0;
}
