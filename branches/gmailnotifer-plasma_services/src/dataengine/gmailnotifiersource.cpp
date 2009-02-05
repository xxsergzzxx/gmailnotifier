/*
** Copyright (C) 2008-2009 Gilles CHAUVIN <gcnweb+gmailnotifier@gmail.com>
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
#include "gmailnotifiersource.h"
#include "gmailnotifierservice.h"
#include "gmailatomfeedparser.h"
// KDE
#include <KIO/Job>


/*
** public:
*/
GmailNotifierSource::GmailNotifierSource(const QString &accountName, const QString &labelName, QObject *parent)
    : Plasma::DataContainer(parent)
    , m_job(0)
{
    kDebug();

    kDebug() << "AccountName:" << accountName;
    kDebug() << "LabelName:" << labelName;

    m_url.setUrl("https://mail.google.com:443/mail/feed/atom/", QUrl::StrictMode);
    m_url.setPath(m_url.path()+labelName); // Add the label afterward to workaround a bug
                                           // when labels contain characters such as [, ], ...
    m_url.setUserName(accountName);

    KUrl dbgUrl(m_url);
    dbgUrl.setPassword( (!dbgUrl.password().isEmpty() ? "**********" : QString()) );
    kDebug() << "Request URL:" << dbgUrl.toEncoded(QUrl::None);

    update();
} // ctor()


GmailNotifierSource::~GmailNotifierSource()
{
//    kDebug();
} // dtor()

Plasma::Service* GmailNotifierSource::createService()
{
    kDebug();

    return new GmailNotifierService(this);
} // createService()

void GmailNotifierSource::update()
{
    kDebug();

    if (m_job || (!account().isEmpty() && password().isEmpty())) {
        kDebug() << "Job running or incomplete credentials. Aborting.";
        kDebug() << "Account:" << account();
        kDebug() << "Password:" << (password().isEmpty() ? "**UNSET**" : "**********");
        return;
    }

    m_job = KIO::get(m_url, KIO::Reload, KIO::HideProgressInfo);
    connect(m_job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(recv(KIO::Job*, const QByteArray&)));
    connect(m_job, SIGNAL(result(KJob*)), this, SLOT(result(KJob*)));

    KUrl dbgUrl(m_url);
    dbgUrl.setPassword( (!dbgUrl.password().isEmpty() ? "**********" : QString()) );
    kDebug() << "Requesting ATOM feed:" << dbgUrl.toEncoded();

} // update()

void GmailNotifierSource::setPassword(const QString &password)
{
    kDebug();

    m_url.setPassword(password);
    kDebug() << "URL password:" << (m_url.password().isEmpty() ? "**UNSET**" : "**********");
    update();
} // setPassword()

QString GmailNotifierSource::account() const
{
    kDebug();

    return m_url.userName();
} // account()

QString GmailNotifierSource::password() const
{
    kDebug();

    return m_url.password();
} // password()


/*
** private Q_SLOTS:
*/
void GmailNotifierSource::recv(KIO::Job * job, const QByteArray &data)
{
    kDebug();

    if (job != m_job) {
        kWarning() << "Receiving data from" << job << "should be" << m_job;
        return;
    }

    //kDebug() << data;
    m_atomFeed += data;
} // recv()

void GmailNotifierSource::result(KJob *job)
{
    kDebug();

    // ?!
    if (job != m_job) {
        kDebug() << "Fail! Job is not our job!";
        return;
    }

    removeAllData();

//    kDebug() << "ATOM feed:" << m_atomFeed;
    kDebug() << "Job error:" << job->error();

    if (job->error()) {
        kDebug() << "Job error!";
        setData("error", job->errorString());
    } else {
        // Parse the ATOM feed
        GmailAtomFeedParser::parseFeed(m_atomFeed, this);
    }

    Plasma::DataContainer::checkForUpdate();
    m_atomFeed.clear();
    m_job = 0;
} // result()


#include "gmailnotifiersource.moc"
