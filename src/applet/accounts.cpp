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
    bool retVal = true;

    if (accountInfos.contains("Login") && !accountInfos.value("Login").toString().isEmpty()) {
        account.login = accountInfos.value("Login").toString();
    } else {
        retVal = false;
    }

    if (accountInfos.contains("Password") && !accountInfos.value("Password").toString().isEmpty()) {
        account.password = accountInfos.value("Password").toString();
    } else {
        retVal = false;
    }

    if (accountInfos.contains("Label")) {
        account.label = accountInfos.value("Label").toString();
    }

    // Account ID
    QString accountId = QString("%1:%2").arg(account.login).arg(account.label);

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
        account.display = accountInfos.value("Display").toString();
    }

    if (accountInfos.contains("BypassNotifications")) {
        account.bypassNotifications = accountInfos.value("BypassNotifications").toBool();
    }

    m_accounts[accountId] = account;

    return true;
} // addAccount()

void Accounts::updateAccountData(const QString &accountId, const Plasma::DataEngine::Data &data)
{
    // Update unread mail count
    m_accounts[accountId].unreadMailCount = data.value("fullcount").toInt();
} // updateAccountData()

int Accounts::size() const
{
    kDebug();
    return m_accounts.size();
} // size()

QStringList Accounts::accountIds() const
{
    kDebug();
    return m_accounts.keys();
} // accountIds()

uint Accounts::totalUnreadMailCount() const
{
    uint count = 0;

    foreach(QString accountId, accountIds()) {
        // Account unreadMailCount is an int and can be -1
        if (m_accounts.value(accountId).unreadMailCount > 0) {
            count += m_accounts.value(accountId).unreadMailCount;
        }
    }

    return count;
} // totalUnreadMailCount()

void Accounts::clear()
{
    kDebug();
    m_accounts.clear();
} // clear()


/*
** Account data
*/
QString Accounts::login(const QString &accountId) const
{
    kDebug();
    return m_accounts.value(accountId).login;
} // login()

QString Accounts::password(const QString &accountId) const
{
    kDebug();
    return m_accounts.value(accountId).password;
} // password()

QString Accounts::label(const QString &accountId) const
{
    kDebug();
    return m_accounts.value(accountId).label;
} // label()

QString Accounts::display(const QString &accountId) const
{
    kDebug();
    QString display;

    if (m_accounts.value(accountId).display.isEmpty()) {
        QString label = (m_accounts.value(accountId).label.isEmpty()) ? "inbox" : m_accounts.value(accountId).label;
        display = QString("%1/%2").arg(m_accounts.value(accountId).login).arg(label);
    } else {
        display = m_accounts.value(accountId).display;
    }

    return display;
} // display()

bool Accounts::bypassNotifications(const QString &accountId) const
{
    kDebug();
    return m_accounts.value(accountId).bypassNotifications;
} // bypassNotifications()

int Accounts::unreadMailCount(const QString &accountId) const
{
    kDebug();
    return m_accounts.value(accountId).unreadMailCount;
} // unreadMailCount()
