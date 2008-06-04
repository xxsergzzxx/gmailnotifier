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


// Own
#include "gmailnotifierappletconfig.h"

// Qt
#include <QtCore/QDebug>


class GMailNotifierAppletConfig::Private
{
public:
    Private() {}
    ~Private() {}
}; // Private()


/*
** public
*/
GMailNotifierAppletConfig::GMailNotifierAppletConfig(QWidget *parent)
    : QWidget(parent)
    , d(new Private)
{
    ui.setupUi(this);

    // Background Hints
    QStringList backgroundHints;
    backgroundHints << "Default" << "Standard" << "Translucent" << "Shadowed";
    ui.comboBackground->addItems(backgroundHints);
} // ctor()

GMailNotifierAppletConfig::~GMailNotifierAppletConfig()
{
    delete d;
} // dtor()


/*
** private slots
*/
void GMailNotifierAppletConfig::on_leLogin_textChanged(const QString &text)
{
    Q_UNUSED(text)
    setAddModifyButtonEnabled();
    adaptAddModifyButtonLabel();
} // on_leLogin_textChanged()

void GMailNotifierAppletConfig::on_lePassword_textChanged(const QString &text)
{
    Q_UNUSED(text)
    setAddModifyButtonEnabled();
} // on_lePassword_textChanged()

void GMailNotifierAppletConfig::on_btnAddModify_clicked()
{
    QString itemText;
    if (ui.leDisplay->text().isEmpty()) {
        itemText = QString("%1@gmail.com")
                       .arg(ui.leLogin->text().toLower().trimmed());
    } else {
        itemText = QString("%1 [%2@gmail.com]")
                       .arg(ui.leDisplay->text())
                       .arg(ui.leLogin->text().toLower().trimmed());
    }

    QListWidgetItem *item = new QListWidgetItem();
    QVariantMap data;
    data["Login"]    = ui.leLogin->text();
    data["Password"] = ui.lePassword->text();
    data["Display"]  = ui.leDisplay->text();
    item->setText(itemText);
    item->setData(Qt::UserRole, data);
    ui.listAccounts->addItem(item);
} // on_btnAddModify_clicked()

void GMailNotifierAppletConfig::setAddModifyButtonEnabled()
{
    bool enabled = (!ui.leLogin->text().isEmpty() && !ui.lePassword->text().isEmpty());
    ui.btnAddModify->setEnabled(enabled);
} // setAddModifyButtonEnabled()

void GMailNotifierAppletConfig::adaptAddModifyButtonLabel()
{
    if (ui.leLogin->text().isEmpty()) {
        return;
    }

    bool existsFlag = false;
    int i=0;
    while(i < ui.listAccounts->count()) {
        QListWidgetItem *item(ui.listAccounts->item(i));
        if (ui.leLogin->text() == item->data(Qt::UserRole).toMap()["Login"]) {
            existsFlag = true;
            break;
        }
        ++i;
    }

    if (existsFlag) {
        ui.btnAddModify->setText("Modify");
    } else {
        ui.btnAddModify->setText("Add");
    }
} // adaptAddModifyButtonLabel()


#include "gmailnotifierappletconfig.moc"
