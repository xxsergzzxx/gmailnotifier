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


// Own
#include "gmailnotifierdialog.h"
// KDE
#include <KDE/KDebug>


/*
** public:
*/
GmailNotifierDialog::GmailNotifierDialog(QObject *parent)
    : QObject(parent)
    , m_widget(new QWidget)
    , m_layoutMain(0)
    , m_layoutAccounts(0)
    , m_logoGmail(0)
{
    kDebug();

    // No background
    m_widget->setAttribute(Qt::WA_NoSystemBackground);

    buildDialog();
} // ctor()

GmailNotifierDialog::~GmailNotifierDialog()
{
    kDebug();
} // dtor()

QWidget* GmailNotifierDialog::widget()
{
    kDebug();
    return m_widget;
} // widget();

void GmailNotifierDialog::setDisplayLogo(const bool &visible)
{
    kDebug();

    m_logoGmail->setVisible(visible);
} // setDisplayLogo()

void GmailNotifierDialog::setTextColor(const QColor &color)
{
    // Text color
    QPalette palette(m_widget->palette());
    QBrush brush(color);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
    m_widget->setPalette(palette);
} // setTextColor()

void GmailNotifierDialog::setAccounts(const Accounts &accounts)
{
    kDebug();

    // Remove any existing labels
    QLayoutItem *item;
    while ((item = m_layoutAccounts->takeAt(0)) != 0) {
        delete item->widget();
        delete item;
    }

    // Populate...
    int row = 0;
    foreach(QString accountId, accounts.idList()) {
        QLabel *lblAccount = new QLabel(accounts.display(accountId));
        lblAccount->setObjectName(QString("lblAccount_%1").arg(accountId));
        m_layoutAccounts->addWidget(lblAccount, row, 0, Qt::AlignLeft | Qt::AlignVCenter);

        QString lblMailCountTxt;
        if (accounts.unreadMailCount(accountId) < 0) {
            lblMailCountTxt = "----";
        } else {
            lblMailCountTxt = QString("%1").arg(accounts.unreadMailCount(accountId));
        }
        QLabel *lblMailCount = new QLabel(lblMailCountTxt);
        lblMailCount->setObjectName(QString("lblMailCount_%1").arg(accountId));
        m_layoutAccounts->addWidget(lblMailCount, row, 1, Qt::AlignRight | Qt::AlignVCenter);

        ++row;
    }
} //setAccounts()

void GmailNotifierDialog::updateMailCount(const QString &accountId, const QString &text)
{
    kDebug();

    QLabel *label;
    if (!(label = m_widget->findChild<QLabel *>("lblMailCount_"+accountId))) {
        kDebug() << QString("No existing QLabel for %1. Skipping...").arg(accountId);
        return;
    }

    label->setText(text);
} // updateMailCount()


/*
** private:
*/
void GmailNotifierDialog::buildDialog()
{
    kDebug();

    // Main layout
    m_layoutMain = new QVBoxLayout(m_widget);
    m_layoutMain->setObjectName("QVBoxLayout m_layoutMain");
    m_layoutMain->setSpacing(0);
    m_layoutMain->setMargin(10);

    // Gmail logo
    m_logoGmail = new QLabel();
    m_logoGmail->setPixmap(QPixmap(":/images/gmail_logo.png"));
    m_logoGmail->setAlignment(Qt::AlignCenter);
    m_layoutMain->addWidget(m_logoGmail);

    m_layoutMain->addSpacerItem(new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

    // Accounts/mailboxes/labels list
    m_layoutAccounts = new QGridLayout();
    m_layoutAccounts->setObjectName("QGridLayout m_layoutAccounts");
    m_layoutAccounts->setSpacing(5);
    m_layoutAccounts->setHorizontalSpacing(30);
    m_layoutAccounts->setMargin(0);
    m_layoutAccounts->setSizeConstraint(QLayout::SetNoConstraint);
    
    m_layoutMain->addLayout(m_layoutAccounts);

    m_layoutMain->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
} // buildDialog()


#include "gmailnotifierdialog.moc"
