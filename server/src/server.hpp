#ifndef SERVER_HPP
#define SERVER_HPP

#include "database_manager.hpp"
#include "json.hpp"
#include "library.hpp"
#include "message_type.hpp"
#include "user.hpp"

#include <QDate>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QtNetwork>
#include <optional>
#include <string>
#include <unordered_map>

class Server : public QObject {
    Q_OBJECT

private:
    Library library;
    std::unordered_map<MessageType, std::function<void(nlohmann::json const&, QTcpSocket*)>> messageHandler;

public:
    explicit Server(QObject* parent = nullptr);
    virtual ~Server();

private slots:
    void newConnection();
    void sendData(QTcpSocket* clientSocket, const nlohmann::json& data);
    void receiveData();
    void disconnect();

private:
    void initServer();
    void loginUser(const User& name, QTcpSocket* clientSocket);
    void registerUser(const User& user, QTcpSocket* clientSocket);
    void changeUsername(const std::string& name, const std::string& password, QTcpSocket* clientSocket);
    void changePassword(const std::string& name, const std::string& password, const std::string& oldpassword,
        const std::string& newpassword, const std::string& confirmpassword, QTcpSocket* clientSocket);
    void logout(QTcpSocket* clientSocket);
    void deleteAccount(const std::string& name, QTcpSocket* clientSocket);
    void sendBooksArray(MessageType messageType, std::vector<Book> books, QTcpSocket* clientSocket);
    void sendAllBooks(QTcpSocket* clientSocket);
    void sendUserBooks(const std::string& name, QTcpSocket* clientSocket);
    void searchBooks(const std::string& query, QTcpSocket* clientSocket);

    void borrowBook(const std::string& booktitle, const std::string& name, QTcpSocket* clientSocket);
    void getBorrowedBooksDate(const std::string& booktitle, const std::string& name, QTcpSocket* clientSocket);
    void returnBook(const std::string& booktitle, const std::string& name, QTcpSocket* clientSocket);

    DatabaseManager database;
    QTcpServer* tcpServer = nullptr;
    std::unordered_map<QTcpSocket*, std::pair<std::optional<User>, QDataStream*>> connections;
};

#endif // SERVER_HPP
