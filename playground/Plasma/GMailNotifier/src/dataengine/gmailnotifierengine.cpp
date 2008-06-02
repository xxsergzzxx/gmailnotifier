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
    QString  request;
       QUrl  url;
      QHttp *http;
        int  httpReqId;

    Private()
        : http(new QHttp)
        , httpReqId(0)
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
    connect(d->http, SIGNAL(requestFinished(int, bool)),
            this, SLOT(httpRequestFinished(int, bool)));
} // ctor()


GMailNotifierEngine::~GMailNotifierEngine()
{
    delete d;
} // dtor()


/*
** Protected
*/
void GMailNotifierEngine::init()
{
    // 5 mins ought to be enough for anyone :)
    setMinimumPollingInterval( 1000 * 60 * 5 );
} // init()

bool GMailNotifierEngine::sourceRequestEvent(const QString &request)
{
    // We expect a request like "username:password"
    QChar sep(':');
    request.trimmed();
    kDebug() << request;
    if (request.startsWith(sep) || request.endsWith(sep) || request.count(sep) != 1) {
        kDebug() << "Bad request! \"username:password\" expected.";
        return false;
    }

    int sepPos = request.indexOf(sep);
    QString username = request.left(sepPos);
    QString password = request.right(request.size()-(sepPos+1));

    // Prevent multiple requests for now
    kDebug() << d->httpReqId;
    if (d->httpReqId) {
        kDebug() << "Pending request, cannot execute now!";
        return false;
    } else {
        kDebug() << "Requesting data...";
        d->request = request;
        d->http->setHost(d->url.host(), QHttp::ConnectionModeHttps, d->url.port());
        d->http->setUser(username, password);
        d->httpReqId = d->http->get(d->url.path());
    }

    setData(request, Plasma::DataEngine::Data());

    return true;
} // sourceRequestEvent()


/*
** Private Q_SLOTS
*/
void GMailNotifierEngine::httpRequestFinished(const int &requestId, const bool &error)
{
    if (requestId != d->httpReqId) {
        return;
    }
    if (error) {
        QVariantList errorInfos;
        errorInfos << d->http->lastResponse().statusCode();
        errorInfos << d->http->errorString();
        setData(d->request, "Error", errorInfos);
        d->httpReqId = 0;
        return;
    }

    Plasma::DataEngine::Data results;
    results = GMailAtomFeedParser::parseFeed(d->http->readAll());
    setData(d->request, results);
    d->httpReqId = 0;

} // httpRequestFinished()


#include "gmailnotifierengine.moc"
