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


// Own
#include "accounts.h"
// KDE
#include <KDE/KDebug>


/*
** public:
*/
Accounts::Accounts()
{
    kDebug();
} // ctor()

Accounts::~Accounts()
{
    kDebug();
} // dtor()

bool Accounts::addAccount(QVariantMap &accountInfos)
{
    kDebug();

    account_t account;
    QString accountId;
    bool retVal = true;

    if (accountInfos.contains("Login")) {
        if (!accountInfos.value("Login").toString().isEmpty()) {
            account.login = accountInfos.value("Login").toString();
        } else {
            retVal = false;
        }
    } else {
        retVal = false;
    }

    if (accountInfos.contains("Password")) {
        if (!accountInfos.value("Password").toString().isEmpty()) {
            account.password = accountInfos.value("Password").toString();
        } else {
            retVal = false;
        }
    } else {
        retVal = false;
    }

    if (accountInfos.contains("Label")) {
        account.password = accountInfos.value("Label").toString();
    }

    // Account ID
    accountId = QString("%1:%2").arg(account.login).arg(account.label);

    // We cannot add an account without login and/or password
    if (!retVal) {
        return false;
    } else {
        // Was the account previousy added ?
        if (m_accounts.contains(accountId)) {
            kWarning() << accountId << "was already added!";
            return false;
        }
    }

    if (accountInfos.contains("Display")) {
        account.password = accountInfos.value("Display").toString();
    }

    if (accountInfos.contains("BypassNotifications")) {
        account.bypassNotifications = accountInfos.value("BypassNotifications").toBool();
    } else {
        account.bypassNotifications = false;
    }

    m_accounts[accountId] = account;

    return true;
} // addAccount()

void Accounts::clearAccounts()
{
    kDebug();

    m_accounts.clear();
} // clearAccounts()
