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

#ifndef __GMAILNOTIFIER_DATAENGINE_H__
#define __GMAILNOTIFIER_DATAENGINE_H__

// Plasma
#include <Plasma/DataEngine>

// KDE
class KJob;
namespace KIO
{
    class Job;
}


class GmailNotifierEngine : public Plasma::DataEngine
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap passwords READ passwords WRITE setPasswords)


public:
    GmailNotifierEngine(QObject *parent, const QVariantList &args);
    ~GmailNotifierEngine();

    QVariantMap passwords() const;
    void setPasswords(const QVariantMap &passwords);

protected:
    void init();
    bool sourceRequestEvent(const QString &request);
    bool updateSourceEvent(const QString &request);

private Q_SLOTS:
    void recv(KIO::Job *job, const QByteArray &data);
    void result(KJob *job);

private:
    class Private;
    Private * const d;
};


K_EXPORT_PLASMA_DATAENGINE(gmailnotifierengine, GmailNotifierEngine)


#endif // __GMAILNOTIFIER_DATAENGINE_H__
