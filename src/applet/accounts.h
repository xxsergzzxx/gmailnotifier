/*
** Copyright (C) 2009 Gilles CHAUVIN <gcnweb+gmailnotifier@gmail.com>
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


#ifndef __ACCOUNTS_H__
#define __ACCOUNTS_H__


// QtCore
#include <QtCore/QStringList>
#include <QtCore/QVariantMap>
// Plasma
#include <Plasma/DataEngine>


class Accounts
{
public:
    Accounts();
    ~Accounts();

    // Append a new account to the account list or update if it was
    // already added previously
    bool add(const QVariantMap &accountInfos);
    // Update data for account
    void updateData(const QString &accountId, const Plasma::DataEngine::Data &data);
    // Remove the listed accounts
    void remove(const QString &accountId);
    // Total number of accounts
    int size() const;
    // Get all the current account Ids
    QStringList idList() const;
    // Total unread mail count
    uint totalUnreadMailCount() const;
    // Clear all the account data
    void clear();

    // Account data
    QString      login(const QString &accountId) const;
    QString      password(const QString &accountId) const;
    QString      label(const QString &accountId) const;
    QString      display(const QString &accountId) const;
    bool         bypassNotifications(const QString &accountId) const;
    int          unreadMailCount(const QString &accountdId) const;
    QVariantList mailEntries(const QString &accountId) const;
    QVariantList newMailEntries(const QString &accountId) const;



private:
    struct account_t {
        QString      login;               // Account login
        QString      password;            // Account password
        QString      label;               // Label
        QString      display;             // Display
        bool         bypassNotifications; // Bypass Notifications?
        int          unreadMailCount;     // Unread mail count
        QVariantList mailEntries;         // Mail entries
        QVariantList newMailEntries;      // New mail entries (since the previous polling)

        account_t() {
            login               = QString();
            password            = QString();
            label               = QString();
            display             = QString();
            bypassNotifications = false;
            unreadMailCount     = -1;
            mailEntries         = QVariantList();
            newMailEntries      = QVariantList();
        };
    };

    QMap<QString, account_t> m_accounts;
};


#endif // __ACCOUNTS_H__
