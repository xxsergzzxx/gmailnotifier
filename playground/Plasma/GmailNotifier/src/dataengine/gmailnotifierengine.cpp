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

// KDE
#include <KDE/KJob>
#include <KDE/KUrl>
#include <KDE/KIO/Job>
#include <KDE/KStringHandler>


class GmailNotifierEngine::Private
{
public:
    QVariantMap passwordList;
    KUrl baseUrl;
    QHash<KJob *, QString> jobs;
    QHash<KJob *, QByteArray> jobData;

    Private()
    {
        // ATOM feed base URL
        baseUrl.setUrl("https://mail.google.com:443/mail/feed/atom/", QUrl::StrictMode);
    }
    ~Private()
    {
    }
}; // Private()


/*
** Public
*/
GmailNotifierEngine::GmailNotifierEngine(QObject *parent, const QVariantList &args)
    : Plasma::DataEngine(parent, args)
    , d(new Private)
{
    kDebug();
} // ctor()


GmailNotifierEngine::~GmailNotifierEngine()
{
    kDebug();
    delete d;
} // dtor()

QVariantMap GmailNotifierEngine::passwords() const
{
    kDebug();
    return d->passwordList;
} // passwords()

void GmailNotifierEngine::setPasswords(const QVariantMap &passwords)
{
    kDebug();
    d->passwordList = passwords;
} // setPasswords()


/*
** Protected
*/
void GmailNotifierEngine::init()
{
    kDebug();
    // 1 min ought to be enough for anybody :)
    setMinimumPollingInterval(1000 * 60 * 1);
} // init()

bool GmailNotifierEngine::sourceRequestEvent(const QString &request)
{
    kDebug();
    return updateSourceEvent(request);
} // sourceRequestEvent()

bool GmailNotifierEngine::updateSourceEvent(const QString &request)
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
        setData(request, "error", "Bad request! \"username:label\" expected.");
        kDebug() << "Bad request! \"username:label\" expected.";
        return false;
    }

    QStringList splittedRequest = request.split(sep);
    QString username(splittedRequest[0]);
    QString label(splittedRequest[1]);

    kDebug() << "Requesting data..." << username << label;

    KUrl url(d->baseUrl);
    if (label != "inbox") {
        url.setPath(url.path()+label);
    }
    url.setUserName(username);
    url.setPassword(KStringHandler::obscure(d->passwordList[username].toString()));

    KIO::Job *job = KIO::get(url, KIO::Reload, KIO::HideProgressInfo);
    d->jobs[job] = request;
    connect(job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(recv(KIO::Job*, const QByteArray&)));
    connect(job, SIGNAL(result(KJob*)), this, SLOT(result(KJob*)));

    setData(request, Plasma::DataEngine::Data());

    return true;
} // updateSourceEvent()


/*
** Private Q_SLOTS
*/

void GmailNotifierEngine::recv(KIO::Job *job, const QByteArray &data)
{
    kDebug();
    d->jobData[job] += data;
} // recv()

void GmailNotifierEngine::result(KJob *job)
{
    kDebug();
    if (!d->jobs.contains(job)) {
        return;
    }

    if (job->error()) {
        kDebug() << job->errorText();
        setData(d->jobs[job], "error", job->errorText());
    } else {
        setData(d->jobs[job], "error", "");
        setData(d->jobs[job], GmailAtomFeedParser::parseFeed(d->jobData[job]));
    }

    d->jobs.remove(job);
    d->jobData.remove(job);
} // result()


#include "gmailnotifierengine.moc"
