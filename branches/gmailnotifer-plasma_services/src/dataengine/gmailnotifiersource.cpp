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
    m_url.setUrl(QString("https://mail.google.com:443/mail/feed/atom/%1").arg(labelName), QUrl::StrictMode);
    m_url.setUserName(accountName);

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
        return;
    }

    m_job = KIO::get(m_url, KIO::Reload, KIO::HideProgressInfo);
    connect(m_job, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(recv(KIO::Job*, const QByteArray&)));
    connect(m_job, SIGNAL(result(KJob*)), this, SLOT(result(KJob*)));
} // update()

void GmailNotifierSource::setPassword(const QString &password)
{
    kDebug();

    m_url.setPassword(password);
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
void GmailNotifierSource::recv(KIO::Job * /*job*/, const QByteArray &data)
{
    m_atomFeed += data;
} // recv()

void GmailNotifierSource::result(KJob *job)
{
    if (job != m_job) {
        kDebug() << "Fail! Job is not our job!";
        return;
    }

    removeAllData();
    if (job->error()) {
        kDebug() << "Job error!";
        setData("error", job->errorText());
    } else {
        emit dataUpdated(objectName(), GmailAtomFeedParser::parseFeed(m_atomFeed));
    }

    Plasma::DataContainer::checkForUpdate();
    m_atomFeed.clear();
    m_job = 0;
} // result()


#include "gmailnotifiersource.moc"
