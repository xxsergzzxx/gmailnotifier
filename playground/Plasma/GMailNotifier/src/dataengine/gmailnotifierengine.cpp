/*
** Copyright (C) 2008 Gilles CHAUVIN <gcnweb+kde@gmail.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2  of  the  License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be  useful, but
** WITHOUT  ANY  WARRANTY;  without  even  the   implied   warranty  of
** MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  GNU
** General Public License for more details.
**
** You should have received a copy of the GNU  General  Public  License
** along with  this  program;  if  not,  write  to  the  Free  Software
** Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

// Own
#include "gmailnotifierengine.h"
#include "gmailatomfeedparser.h"

// Qt
#include <QtCore/QUrl>
#include <QtNetwork/QHttp>


class GMailNotifierEngine::Private
{
public:
                  QUrl  url;
                 QHttp *http;
           QVariantMap  passwordList;
    QMap<int, QString>  pendingRequests;

    Private()
        : http(new QHttp)
    {
        // ATOM feed base URL
        url.setUrl("https://mail.google.com:443/mail/feed/atom/", QUrl::StrictMode);
    }
    ~Private() {}
}; // Private()


/*
** Public
*/
GMailNotifierEngine::GMailNotifierEngine(QObject *parent, const QVariantList &args)
    : Plasma::DataEngine(parent, args)
    , d(new Private)
{
    kDebug();
    connect(d->http, SIGNAL(requestFinished(int, bool)),
            this, SLOT(httpRequestFinished(int, bool)));
} // ctor()


GMailNotifierEngine::~GMailNotifierEngine()
{
    kDebug();
    delete d;
} // dtor()

QVariantMap GMailNotifierEngine::passwords() const
{
    kDebug();
    return d->passwordList;
} // passwords()

void GMailNotifierEngine::setPasswords(const QVariantMap &passwords)
{
    kDebug();
    d->passwordList = passwords;
} // setPasswords()


/*
** Protected
*/
void GMailNotifierEngine::init()
{
    kDebug();
    // 5 mins ought to be enough for anybody :)
    //setMinimumPollingInterval( 1000 * 60 * 5 );
} // init()

bool GMailNotifierEngine::sourceRequestEvent(const QString &request)
{
    kDebug();
    // No need to continue if the password list is empty...
    if (d->passwordList.isEmpty()) {
        setData(request, "error", "No passwords set!");
        return false;
    }

    // We expect a request like "username:label"
    QChar sep(':');
    request.trimmed();
    if (request.startsWith(sep) || request.endsWith(sep) || request.count(sep) != 1) {
        kDebug() << "Bad request! \"username:label\" expected.";
        return false;
    }

    QStringList splittedRequest = request.split(sep);
    QString username(splittedRequest[0]);
    QString label(splittedRequest[1]);

    kDebug() << "Requesting data..." << username << label;
    QString path;
    if (label == "inbox") {
        path = d->url.path();
    } else {
        path = d->url.path() + label;
    }
    d->http->setHost(d->url.host(), QHttp::ConnectionModeHttps, d->url.port());
    d->http->setUser(username, d->passwordList[username].toString());
    int reqId = d->http->get(path);

    d->pendingRequests.insert(reqId, request);
    setData(request, Plasma::DataEngine::Data());

    return true;
} // sourceRequestEvent()


/*
** Private Q_SLOTS
*/
void GMailNotifierEngine::httpRequestFinished(const int &requestId, const bool &error)
{
    kDebug();
    if (!d->pendingRequests.contains(requestId)) {
        return;
    }
    if (error) {
//        QVariantList errorInfos;
//        errorInfos << d->http->lastResponse().statusCode();
//        errorInfos << d->http->errorString();
//        setData(d->request, "error", errorInfos);
        setData(d->pendingRequests[requestId], "error", d->http->errorString());
        d->pendingRequests.remove(requestId);
        return;
    }

    Plasma::DataEngine::Data results;
    results = GMailAtomFeedParser::parseFeed(d->http->readAll());
    setData(d->pendingRequests[requestId], results);
    d->pendingRequests.remove(requestId);

} // httpRequestFinished()


#include "gmailnotifierengine.moc"
