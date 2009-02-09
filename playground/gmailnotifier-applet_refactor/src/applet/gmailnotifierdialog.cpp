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

void GmailNotifierDialog::setAccounts(const QList<QMap<QString, QString> > &accounts,
                                      const QMap<QString, uint> &unreadMailCount)
{
    kDebug();

    // Remove any existing labels
    QLayoutItem *item;
    while ((item = m_layoutAccounts->takeAt(0)) != 0) {
        delete item->widget();
        delete item;
    }

    // Populate...
    QList<QMap<QString, QString> >::ConstIterator it;
    int row = 0;
    for (it = accounts.constBegin(); it != accounts.constEnd(); ++it) {
        QString display;
        if (!it->value("Display").isEmpty()) {
            display = it->value("Display");
        } else {
            QString label = (it->value("Label").isEmpty()) ? "inbox" : it->value("Label");
            display = QString("%1/%2").arg(it->value("Login")).arg(label);
        }

        QString loginNLabel = QString("%1:%2").arg(it->value("Login")).arg(it->value("Label"));

        QLabel *lblAccount = new QLabel(display);
        lblAccount->setObjectName(QString("lblAccount_%1").arg(loginNLabel));
        m_layoutAccounts->addWidget(lblAccount, row, 0, Qt::AlignLeft | Qt::AlignVCenter);

        QString lblMailCountTxt;
        if (unreadMailCount.contains(loginNLabel)) {
            lblMailCountTxt = QString("%1").arg(unreadMailCount[loginNLabel]);
        } else {
            lblMailCountTxt = "----";
        }
        QLabel *lblMailCount = new QLabel(lblMailCountTxt);
        lblMailCount->setObjectName(QString("lblMailCount_%1").arg(loginNLabel));
        m_layoutAccounts->addWidget(lblMailCount, row, 1, Qt::AlignRight | Qt::AlignVCenter);

        ++row;
    }
} //setAccounts()

void GmailNotifierDialog::updateMailCount(const QString &source, const Plasma::DataEngine::Data &data)
{
    kDebug();

    QLabel *label;
    if (!(label = m_widget->findChild<QLabel *>("lblMailCount_"+source))) {
        kDebug() << QString("No existing QLabel for %1. Skipping...").arg(source);
        return;
    }
    QString content;
    if (!data["error"].toString().isEmpty()) {
        kDebug() << "Error:" << data["error"].toString();
        content = "Err.";
    } else {
        content = data["fullcount"].toString();
    }
    label->setText(content);
} // updateMailCount()


/*
** private:
*/
void GmailNotifierDialog::buildDialog()
{
    kDebug();

    // Use white colored text
    // TODO: Maybe provide a way for the user to set his own colors
    QPalette palette(m_widget->palette());
    QBrush brush(QColor(Qt::white));
    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
    m_widget->setPalette(palette);

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
