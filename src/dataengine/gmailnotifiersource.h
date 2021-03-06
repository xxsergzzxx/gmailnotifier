/*
** Copyright (C) 2008-2009 Gilles CHAUVIN <gcnweb+gmailnotifier@gmail.com>
**
** $Id$
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


#ifndef __GMAILNOTIFIERSOURCE_H__
#define __GMAILNOTIFIERSOURCE_H__


// Plasma
#include <Plasma/DataContainer>
// KDE
#include <KDE/KUrl>
// forward declarations
class KJob;

namespace KIO
{
    class Job;
} // namespace KIO


class GmailNotifierSource : public Plasma::DataContainer
{
    Q_OBJECT

public:
    GmailNotifierSource(const QString &accountName, const QString &labelName, QObject *parent = 0);
    ~GmailNotifierSource();

    Plasma::Service* createService();

    void update();
    void setPassword(const QString &password);
    QString account() const;
    QString password() const;

private Q_SLOTS:
    void recv(KIO::Job *job, const QByteArray &data);
    void result(KJob *job);

private:
    QString atomFeedUrl(const QString &account) const;

    KUrl         m_url;
    KIO::Job    *m_job;
    QByteArray   m_atomFeed;
};


#endif // __GMAILNOTIFIERSOURCE_H__
