#ifndef FIREBASE_H
#define FIREBASE_H

#include "define.h"
#include <QObject>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QVariantMap>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QTimer>

#include <QDebug>

class Firebase : public QObject
{
    Q_OBJECT
public:

    explicit Firebase(QObject *parent = nullptr);
    ~Firebase();

    // Firebase request for sign user in
    void signUserIn();

    // Firebase request for sending events
    void listenEvents();

    // Firebase request for storing event in data base
    void storeEvent( const char* event,   const QString & newState );

    // Firebase request for checking password
    void checkPassword( QString & password );

    // getAuthState() return True if connection to Firebase finished succesfully
    bool getAuthState();

public slots:
    //Firebase request for refreshing authentication
    void refreshToken();

    //Reading response from Firebase
    void signUserInReadyRead();
    void eventReplyReadyRead();
    void passwordReadyRead();
    void storeEventReadyRead();

signals:
    //will be send after password will be positive verify
    void openRequest();

private:

    void parseResponse( const QByteArray & reponse );
    QString m_apiKey;
    QNetworkAccessManager * m_networkAccessManager;
    QNetworkReply * m_networkReply;
    QString m_idToken;
    QString m_refreshToken;
    QString m_typedPassword;
    QString m_passwordOwner;
    bool m_isAuth=false;
    QByteArray m_signUserInBuffer = "";
    QTimer * m_timer;
};

#endif // FIREBASE_H
