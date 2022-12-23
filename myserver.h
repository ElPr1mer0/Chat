#ifndef MYSERVER_H
#define MYSERVER_H
#include <QWidget>


class QTcpServer;
class QTextEdit;
class QTcpSocket;

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

#endif // MYSERVER_H
