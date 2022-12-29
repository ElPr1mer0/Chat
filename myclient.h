#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QTcpSocket>

class QTextEdit;
class QLineEdit;
class QTcpServer;


class MYCLIENT: public QWidget{
    Q_OBJECT

public:
    QString localhostIP;
private:
    QTcpSocket* m_pTcpSocket;
    QTextEdit* m_ptxtInfo;
    QLineEdit* m_ptxtInput;
    QLineEdit *ld_friend_ip;



    quint16 m_nNextBlockSize;
    QString friend_ip;
    int friend_port;

public:
    MYCLIENT(const QString& strHost, int nPort, QWidget* pwgt = 0);

private slots:
    void slotReadyRead();
    void slotError (QAbstractSocket:: SocketError);
    void slotSendToServer ();
    void slotConnected ();

    void myConnect();

};






class MYSERVER: public QWidget{
    Q_OBJECT

private:
        QTcpServer* m_ptcpServer;
        QTextEdit* m_ptxt;
        quint16 m_nNextBlockSize;

private:
        void sendToClient(QTcpSocket* pSocket, const QString& str);

public:
        MYSERVER (int nPort, QWidget* pwgt = 0);

public slots:
        virtual void slotNewConnection();
        void slotReadClient();
};



#endif // MYCLIENT_H
