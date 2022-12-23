#include "myclient.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QTcpServer>
#include <QMessageBox>



MYCLIENT::MYCLIENT(const QString &strHost, int nPort, QWidget *pwgt): QWidget (pwgt), m_nNextBlockSize(0){
   m_pTcpSocket = new QTcpSocket(this);
   m_pTcpSocket->connectToHost(strHost, nPort);
   connect(m_pTcpSocket, SIGNAL(connected()),this, SLOT(slotConnected()));
   connect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
   connect(m_pTcpSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));

   m_ptxtInfo = new QTextEdit;
   m_ptxtInput = new QLineEdit;

   m_ptxtInfo -> setReadOnly(true);

   QPushButton* pcmd = new QPushButton("&Send");
   connect(pcmd, SIGNAL(clicked()), SLOT(slotSendToServer()));
   connect(m_ptxtInput, SIGNAL(returnPressed()),
           this, SLOT(slotSendToServer()));

   QVBoxLayout* pvbxLayout = new QVBoxLayout;
   pvbxLayout->addWidget(new QLabel("<H1>Client</H1>"));
   pvbxLayout->addWidget(m_ptxtInfo);
   pvbxLayout->addWidget(m_ptxtInput);
   pvbxLayout->addWidget(pcmd);
   setLayout(pvbxLayout);
}

void MYCLIENT::slotReadyRead(){
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_6_2);

    for(;;){
        if(!m_nNextBlockSize){
            if(m_pTcpSocket->bytesAvailable()<sizeof(quint16)){
                break;
            }
            in>>m_nNextBlockSize;
        }

        if(m_pTcpSocket->bytesAvailable()< m_nNextBlockSize){
            break;
        }

        QTime time;
        QString str;
        in>>time>>str;

        m_ptxtInfo->append(time.toString() + " " + str);
        m_nNextBlockSize = 0;
    }
}

void MYCLIENT::slotError(QAbstractSocket::SocketError err){
    QString strError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                            "The host was not found.":
                            err == QAbstractSocket::RemoteHostClosedError?
                            "The remote host is closed.":
                            err == QAbstractSocket::ConnectionRefusedError?
                            "The connection was refused.":
                            QString(m_pTcpSocket->errorString()));

    m_ptxtInfo->append(strError);
}

void MYCLIENT::slotSendToServer(){
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out<<quint16(0)<<QTime::currentTime()<< m_ptxtInput->text();

    out.device()->seek(0);
    out<< quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
    m_ptxtInput->setText("");
}

void MYCLIENT::slotConnected(){
    m_ptxtInfo->append("Received the connecte() signal");
}


void MYSERVER::sendToClient(QTcpSocket *pSocket, const QString &str){
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out<<quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out<< quint16(arrBlock.size()-sizeof(quint16));

    pSocket->write(arrBlock);
}

MYSERVER::MYSERVER(int nPort, QWidget *pwgt): QWidget(pwgt), m_nNextBlockSize (0){
    m_ptcpServer = new QTcpServer(this);
    if(!m_ptcpServer->listen(QHostAddress::Any, nPort)){
        QMessageBox::critical(0,
                               "Server Error",
                               "Unable to start the server:"
                               + m_ptcpServer->errorString()
                               );
        m_ptcpServer->close();
        return;
    }

    connect(m_ptcpServer, SIGNAL(newConnection()),
            this, SLOT(slotNewConnection())
            );

    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);

    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
    pvbxLayout->addWidget(m_ptxt);
    setLayout(pvbxLayout);

}

void MYSERVER::slotNewConnection(){
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater())
            );
    connect(pClientSocket, SIGNAL(readyRead()),
            this, SLOT(slotReadClient())
            );

    sendToClient(pClientSocket, "Server Response: Connected!");
}

void MYSERVER::slotReadClient(){
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_6_2);

    for(;;){
        if(!m_nNextBlockSize){
            if(pClientSocket->bytesAvailable()< sizeof(quint16)){
                break;
            }
            in>>m_nNextBlockSize;
        }

        if(pClientSocket->bytesAvailable() < m_nNextBlockSize){
            break;
        }

        QTime time;
        QString str;
        in>>time>>str;

        QString strMessage = time.toString() + " " + "Client has sent - " + str;
        m_ptxt->append(strMessage);

        m_nNextBlockSize = 0;

        sendToClient(pClientSocket,"Server Response: Received\"" + str + "\"");


    }
}
