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

bool Accounts::add(const QVariantMap &accountInfos)
{
    kDebug();

    account_t account;
    QString login;
    QString password;
    QString label;
    bool retVal = true;

    if (accountInfos.contains("Login") && !accountInfos.value("Login").toString().isEmpty()) {
        login = accountInfos.value("Login").toString();
    } else {
        retVal = false;
    }

    if (accountInfos.contains("Password") && !accountInfos.value("Password").toString().isEmpty()) {
        password = accountInfos.value("Password").toString();
    } else {
        retVal = false;
    }

    if (accountInfos.contains("Label")) {
        label = accountInfos.value("Label").toString();
    }

    // Account ID
    QString accountId = QString("%1:%2").arg(login).arg(label);

    // We cannot add an account without login and/or password
    if (!retVal) {
        kWarning() << QString("Cannot add %1 (invalid data)").arg(accountId);
        return false;
    }

    // Check whether the account already exist or not
    if (m_accounts.contains(accountId)) {
        // Use previous account data
        account = m_accounts.value(accountId);
    }

    account.password = password;

    if (accountInfos.contains("Display")) {
        account.display = accountInfos.value("Display").toString();
    }

    if (accountInfos.contains("DisableNotifications")) {
        account.disableNotifications = accountInfos.value("DisableNotifications").toBool();
    }

    if (accountInfos.contains("ExcludeFromTotalCount")) {
        account.excludeFromTotalCount = accountInfos.value("ExcludeFromTotalCount").toBool();
    }

    m_accounts[accountId] = account;

    return true;
} // add()

void Accounts::updateData(const QString &accountId, const Plasma::DataEngine::Data &data)
{
    kDebug();
    // Remove previous "new entries"
    m_accounts[accountId].newMailEntries.clear();

    // Don't compare old and new mail entries the first time we run
    // (in that case unreadMailCount should be == -1)
    if (m_accounts[accountId].unreadMailCount >= 0) {
        foreach (QVariant entry, data.value("entries").toList()) {
            if (!m_accounts.value(accountId).mailEntries.contains(entry)) {
                m_accounts[accountId].newMailEntries.append(entry);
            }
        }
    }

    // Update mail entries
    m_accounts[accountId].mailEntries = data.value("entries").toList();

    // Update unread mail count
    m_accounts[accountId].unreadMailCount = data.value("fullcount").toInt();
} // updateData()

void Accounts::remove(const QString &accountId)
{
    kDebug();
    m_accounts.remove(accountId);
} // remove()

int Accounts::size() const
{
    kDebug();
    return m_accounts.size();
} // size()

QStringList Accounts::idList() const
{
    kDebug();
    return m_accounts.keys();
} // idList()

uint Accounts::totalUnreadMailCount() const
{
    kDebug();
    uint count = 0;

    foreach(QString accountId, idList()) {
        // Account unreadMailCount is an int and can be -1
        if (m_accounts.value(accountId).unreadMailCount > 0 &&
            !m_accounts.value(accountId).excludeFromTotalCount) {
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

bool Accounts::disableNotifications(const QString &accountId) const
{
    kDebug();
    return m_accounts.value(accountId).disableNotifications;
} // disableNotifications()

bool Accounts::excludeFromTotalCount(const QString &accountId) const
{
    kDebug();
    return m_accounts.value(accountId).excludeFromTotalCount;
} // excludeFromTotalCount()

int Accounts::unreadMailCount(const QString &accountId) const
{
    kDebug();
    return m_accounts.value(accountId).unreadMailCount;
} // unreadMailCount()

QVariantList Accounts::mailEntries(const QString &accountId) const
{
    kDebug();
    return m_accounts.value(accountId).mailEntries;
} // mailEntries()

QVariantList Accounts::newMailEntries(const QString &accountId) const
{
    kDebug();
    return m_accounts.value(accountId).newMailEntries;
} // newMailEntries()
