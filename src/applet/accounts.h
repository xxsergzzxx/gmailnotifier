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

    // Append a new account to the account list
    bool addAccount(QVariantMap &accountInfos);
    // Update data for account
    void updateAccountData(const QString &accountId, const Plasma::DataEngine::Data &data);
    // Total number of accounts
    int size() const;
    // Get all the current account Ids
    QStringList accountIds() const;
    // Total unread mail count
    uint totalUnreadMailCount() const;
    // Clear all the account data
    void clear();

    // Account data
    QString login(const QString &accountId) const;
    QString password(const QString &accountId) const;
    QString label(const QString &accountId) const;
    QString display(const QString &accountId) const;
    bool    bypassNotifications(const QString &accountId) const;
    int     unreadMailCount(const QString &accountdId) const;


private:
    struct account_t {
        QString login;               // Account login
        QString password;            // Account password
        QString label;               // Label
        QString display;             // Display
        bool    bypassNotifications; // Bypass Notifications?
        int     unreadMailCount;     // Unread mail count

        account_t() {
            login               = QString();
            password            = QString();
            label               = QString();
            display             = QString();
            bypassNotifications = false;
            unreadMailCount     = -1;
        };
    };

    QMap<QString, account_t> m_accounts;
};


#endif // __ACCOUNTS_H__
