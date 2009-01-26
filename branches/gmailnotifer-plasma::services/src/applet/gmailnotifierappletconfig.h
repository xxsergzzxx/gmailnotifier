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

#ifndef __GMAILNOTIFIER_APPLET_CONFIG_H__
#define __GMAILNOTIFIER_APPLET_CONFIG_H__


// Own
#include "ui_gmailnotifierappletconfig.h"

// Qt
#include <QtGui/QWidget>


class GmailNotifierAppletConfig : public QWidget
{
    Q_OBJECT

public:
    GmailNotifierAppletConfig(KConfigGroup cg, QWidget *parent = 0);
    ~GmailNotifierAppletConfig();

    KConfigGroup config();

private Q_SLOTS:
    void on_leLogin_textChanged(const QString &text);
    void on_leLabel_textChanged(const QString &text);
    void on_lePassword_textChanged(const QString &text);
    void on_btnAddModify_clicked();
    void on_btnDelete_clicked();
    void on_btnUp_clicked();
    void on_btnDown_clicked();
    void on_listAccounts_itemPressed(QListWidgetItem *item);

private:
    void setAddModifyButtonEnabled();
    void setUpDownButtonsEnabled();
    void adaptAddModifyButtonLabel();
    QString listItemText(const QMap<QString, QString> &data);
    void addItemToList(const QMap<QString, QString> &data);
    int accountPosition(const QString &login, const QString &label);
    void moveItem(const int &shift);
    QVariantMap QSM2QVM(const QMap<QString, QString> &data);
    QMap<QString, QString> QVM2QSM(const QVariantMap &data);

    Ui::GmailNotifierAppletConfig ui;

    KConfigGroup m_cg;
};


#endif // __GMAILNOTIFIER_APPLET_CONFIG_H__
