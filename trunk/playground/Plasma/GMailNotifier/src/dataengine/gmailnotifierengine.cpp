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
    Private()
        : http(new QHttp)
        , httpReqId(0)
    {}
    ~Private() {}

    QString  request;
    QUrl     url;
    QHttp   *http;
    int      httpReqId;
}; // Private()


/*
** Public
*/
GMailNotifierEngine::GMailNotifierEngine(QObject *parent, const QVariantList &args)
    : Plasma::DataEngine(parent, args)
    , d(new Private)
{
    // ATOM feed URL
    d->url.setUrl("https://mail.google.com:443/mail/feed/atom", QUrl::StrictMode);

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
/*
void GMailNotifierEngine::init()
{
} // init()
*/

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
    if ( d->httpReqId == 0 ) {
        kDebug() << "Requesting data...";
        d->request = request;
        d->http->setHost(d->url.host(), QHttp::ConnectionModeHttps, d->url.port());
        d->http->setUser(username, password);
        d->httpReqId = d->http->get(d->url.path());
    } else {
        kDebug() << "Request pending, cannot execute now!";
        return false;
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
        qDebug() << d->http->errorString();
        d->httpReqId = 0;
        return;
    }

    Plasma::DataEngine::Data results;
    results = GMailAtomFeedParser::parseFeed(d->http->readAll());
    setData(d->request, results);

} // httpRequestFinished()


#include "gmailnotifierengine.moc"
