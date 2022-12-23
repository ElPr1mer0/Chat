#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QTcpSocket>

class QTextEdit;
class QLineEdit;

class MYCLIENT: public QWidget{
    Q_OBJECT

private:
    QTcpSocket* m_pTcpSocket;
    QTextEdit* m_ptxtInfo;
    QLineEdit* m_ptxtInput;
    quint16 m_nNextBlockSize;

public:
    MYCLIENT(const QString& strHost, int nPort, QWidget* pwgt = 0);

private slots:
    void slotReadyRead();
    void slotError (QAbstractSocket:: SocketError);
    void slotSendToServer ();
    void slotConnected ();
};

#endif // MYCLIENT_H
