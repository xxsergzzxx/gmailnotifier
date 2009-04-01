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
#include <QtCore/QString>
#include <QtCore/QVariantMap>


class Accounts
{
public:
    Accounts();
    ~Accounts();

    // Append a new account to the account list
    bool addAccount(QVariantMap &accountInfos);
    // Clear all accounts
    void clearAccounts();

private:
    struct account_t {
        QString login;
        QString password;
        QString label;
        QString display;
        bool    bypassNotifications;
    };

    QMap<QString, account_t>    m_accounts;
};


#endif // __ACCOUNTS_H__
