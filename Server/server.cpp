#include "server.h"
#include "ui_server.h"

using namespace std;

Server::Server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);

    connect(&server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

    server.listen(QHostAddress::Any, 12345);
}

Server::~Server()
{
    delete ui;
    server.close();
}

void Server::acceptConnection()
{
    client = server.nextPendingConnection();

    connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
}

void Server::startRead()
{
    char buffer[1024];

    QString bufferString(buffer);

    int slicePos = bufferString.indexOf("m");
    QString factor2String = bufferString.right(slicePos);
    QString factor1String = bufferString.left(slicePos);

    int factor1 = factor1String.toInt();
    int factor2 = factor2String.toInt();
    int product = factor1 * factor2;

    QString productString = QString::number(product);

    client->read(buffer, client->bytesAvailable());
    client->close();

    qDebug() << factor1String << " * " << factor2String << " = " << productString;
}
