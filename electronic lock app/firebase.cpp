#include "firebase.h"


Firebase::Firebase(QObject *parent)
    : QObject(parent)
    , m_apiKey ( API_KEY )
    , m_idToken ( QString() )
{
    m_networkAccessManager = new QNetworkAccessManager( this );
    signUserIn();

    while ( !getAuthState() )
    {
        qApp->processEvents();
    }
    storeEvent( EVENT_CONNECTION_STATUS, CONNECTION_OK );
    storeEvent( EVENT_OPEN_REQUEST, OPEN_CONFIRMATION );

    listenEvents();

    //switch authentification refresh
    m_timer = new QTimer(this);
    connect( m_timer, &QTimer::timeout, this, &Firebase::refreshToken );
    m_timer->start( AUTH_REFRESH_TIME_SEC * 1000 );

}

Firebase::~Firebase()
{
    m_networkAccessManager->deleteLater();
}

bool Firebase::getAuthState()
{
    return m_isAuth;
}

void Firebase::storeEvent( const char* event, const QString & newState)
{
    //qDebug()<<"Store event"<<newState;
    QString sUrl = STR_URL_EVENT + m_idToken;

    QVariantMap variantPayload;
    variantPayload[event] = newState;

    QJsonDocument jsonDocPayload = QJsonDocument::fromVariant( variantPayload );

    QNetworkRequest newRequest( (QUrl( sUrl )) );
    newRequest.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/json"));

    m_networkReply = m_networkAccessManager->sendCustomRequest( newRequest, "PATCH", jsonDocPayload.toJson());

    connect( m_networkReply, &QNetworkReply::readyRead, this, &Firebase::storeEventReadyRead );
}

void Firebase::signUserIn()
{
    //qDebug()<<"signUserIn";
    QString signInEndpoint = STR_URL_SIGN_IN + m_apiKey;

    QVariantMap variantPayload;
    variantPayload["email"] = LOGIN_MAIL;
    variantPayload["password"] = LOGIN_PASS;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonDocPayload = QJsonDocument::fromVariant( variantPayload );

    QNetworkRequest newRequest( (QUrl( signInEndpoint )) );
    newRequest.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/json"));

    QNetworkReply * networkReply;
    networkReply = m_networkAccessManager->post( newRequest, jsonDocPayload.toJson());

    connect( networkReply, &QNetworkReply::readyRead, this, &Firebase::signUserInReadyRead);
}

void Firebase::refreshToken()
{
    //qDebug()<<"call refreshToken";
    QString signInEndpoint = STR_URL_REFRESH + m_apiKey;

    QVariantMap variantPayload;
    variantPayload["grant_type"] =  "refresh_token";
    variantPayload["refresh_token"] = m_refreshToken;

    QJsonDocument jsonDocPayload = QJsonDocument::fromVariant( variantPayload );

    QNetworkRequest newRequest( (QUrl( signInEndpoint )) );
    newRequest.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/application/x-www-form-urlencoded"));

    QNetworkReply * networkReply;
    networkReply = m_networkAccessManager->post( newRequest, jsonDocPayload.toJson());

    connect( networkReply, &QNetworkReply::readyRead, this, &Firebase::signUserInReadyRead);
}

void Firebase::signUserInReadyRead()
{
    //qDebug()<<"signUserInReadyRead";
    QNetworkReply *networkReply = qobject_cast<QNetworkReply*>(sender());
    QByteArray response = networkReply->readAll();

    if ( response.isEmpty() || response.isNull() )
    {
        //qDebug()<<"is null";

        signUserIn();
        return;
    }

   if ( response.endsWith("\n}\n") )
   {
       m_signUserInBuffer += response;
       response = m_signUserInBuffer;
       m_signUserInBuffer = "";

       response = response.right( response.size()-response.indexOf("{") );

       //qDebug() << "Iterate over ";
        QJsonDocument jsonDocument = QJsonDocument::fromJson( response );
        QJsonObject json = jsonDocument.object();
        foreach(const QString& key, json.keys())
            {
        QJsonValue value = json.value(key);
        }

        if ( jsonDocument.object().contains("error") )
        {
            //qDebug() << "Error occured!" << response;
            signUserIn();
            return;
        }
        else if ( jsonDocument.object().contains("idToken"))
        {
            m_idToken = jsonDocument.object().value("idToken").toString();
            m_refreshToken = jsonDocument.object().value("refresh_token").toString();
            //qDebug() << "User signed in successfully!";

            m_isAuth = true;
        }
                else
        {
        //qDebug() << "Response from else :" << response;
        }
   }
   else
   {
   m_signUserInBuffer += response;
   }
}

void Firebase::listenEvents ()
{
      QString endPoint = QString(STR_URL_EVENT) + m_idToken;
      QNetworkRequest newRequest( (QUrl( endPoint )) );
      newRequest.setRawHeader("Accept",
                           "text/event-stream");
      QNetworkReply * networkReply;
      networkReply = m_networkAccessManager->get(newRequest);
      connect( networkReply, &QNetworkReply::readyRead, this, &Firebase::eventReplyReadyRead);
}

void Firebase::eventReplyReadyRead()
{
    QNetworkReply *replay = qobject_cast<QNetworkReply*>(sender());
    if (replay)
    {

        QByteArray response = replay->readAll();
        //qDebug() << "Event response was: " << response;

        if (response.indexOf( "event: auth_revoked\ndata: credential is no longer valid\n\nevent: cancel\ndata: Permission denied\n\n") == 0 )
        {
            signUserIn();
            return;
        }

        if ( response.left(sizeof (EVENT_RESPONSE_PREFIX)).contains(EVENT_RESPONSE_PREFIX) )
        {
            int jsonBegin = response.indexOf("{");
            int jsonEnd   = response.lastIndexOf("}");
            QByteArray jsonOnly = response.mid( jsonBegin+1, jsonEnd-jsonBegin-1 );

            jsonBegin = jsonOnly.indexOf("{");
            jsonEnd   = jsonOnly.lastIndexOf("}");
            QByteArray jsonInsideOnly = jsonOnly.mid( jsonBegin, jsonEnd-jsonBegin+1 );

            //qDebug() << "jsonInsideOnly: " << jsonInsideOnly;

            QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonInsideOnly);

            QStringList listOfKeys = jsonDocument.object().keys();
            for (const auto & key : listOfKeys)
            {
                //qDebug() << key <<": "<<jsonDocument.object().value(key).toString();
                if ( (key==EVENT_OPEN_REQUEST) && (jsonDocument.object().value(key).toString().toUpper()==QString(OPEN_REQUEST).toUpper()) )
                {
                    emit openRequest();
                }

                if ( (key==EVENT_CONNECTION_STATUS) && (jsonDocument.object().value(key).toString().toUpper()==QString(CONNECTION_CHECK).toUpper() ))
                {
                    storeEvent(EVENT_CONNECTION_STATUS, CONNECTION_OK );
                }
            }
        }
    }
}

void Firebase::checkPassword( QString & password )
{
    m_typedPassword = password;
    QString endPoint = STR_URL_KEYS + m_idToken;
    QNetworkReply * networkReply;

    networkReply = m_networkAccessManager->get( QNetworkRequest(QUrl(endPoint)));

    connect( networkReply, &QNetworkReply::readyRead, this, &Firebase::passwordReadyRead );
}

void Firebase::passwordReadyRead()
{
    QNetworkReply *replay = qobject_cast<QNetworkReply*>(sender());
    QByteArray response = replay->readAll();

    //  Follow JSON Object inside of respond
    //  {
    //      "name1":{"child_key1":{"name1":password1}}
    //      "name2":{"child_key2":{"name2":password2}}
    //      .......
    //  }

    //qDebug()<<"Response from lockKeysReadyRead: "<<response;
    QJsonDocument jsonDocument = QJsonDocument::fromJson( response );
    QJsonObject json = jsonDocument.object();
    //listOfJsonKeys included: {name1, name2, ....}
    QStringList listOfJsonKeys = json.keys();

    m_passwordOwner = "";

    for (const auto & name : listOfJsonKeys)
    {
        QString child_key = json.value(name).toObject().keys()[0];
        //insideJson it is {"name":password}
        QJsonObject insideJson = json.value(name).toObject().value( child_key ).toObject();
        QString password = QString::number ( insideJson.value( name ).toInt() );
        //qDebug() << name <<": "<<password;

        if ( password == m_typedPassword)
        {
            if ( m_passwordOwner == QString("") )
            {
                m_passwordOwner = name;
            }
            else
            {
                m_passwordOwner += QString(" or ") + name;
            }
        }
    }
    if ( m_passwordOwner != QString("") )
    {
        //qDebug() <<m_passwordOwner<<" opened door";
        emit openRequest();
        //hier could be additional log function in Firebase
    }
    else
    {
        emit wrongPassword();
    }

}

void Firebase::storeEventReadyRead()
{
    //this information is not used
    QByteArray response = m_networkReply->readAll();
}
