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
                       .arg(ui.leLogin->text());
    } else {
        itemText = QString("%1 [%2@gmail.com]")
                       .arg(ui.leDisplay->text())
                       .arg(ui.leLogin->text());
    }

    int pos = accountPosition(ui.leLogin->text());
    QListWidgetItem *item;
    if (pos == -1) {
        // Add
        item = new QListWidgetItem();
    } else {
        // Modify
        item = ui.listAccounts->item(pos);
    }

    QVariantMap data;
    data["Login"]    = ui.leLogin->text();
    data["Password"] = ui.lePassword->text();
    data["Display"]  = ui.leDisplay->text();
    item->setText(itemText);
    item->setData(Qt::UserRole, data);

    if (pos == -1) {
        ui.listAccounts->addItem(item);
        setUpDownButtonsEnabled();
    }

    ui.listAccounts->scrollToItem(item);

    adaptAddModifyButtonLabel();
} // on_btnAddModify_clicked()

void GMailNotifierAppletConfig::on_btnDelete_clicked()
{
    ui.listAccounts->takeItem(ui.listAccounts->currentRow());

    if (ui.listAccounts->count() == 0) {
        ui.btnDelete->setEnabled(false);
    }

    ui.listAccounts->setCurrentRow(-1);
    ui.btnDelete->setEnabled(false);
    adaptAddModifyButtonLabel();
    setUpDownButtonsEnabled();
} // on_btnDelete_clicked()

void GMailNotifierAppletConfig::on_btnUp_clicked()
{
    int pos = ui.listAccounts->currentRow();
    QListWidgetItem *item = ui.listAccounts->takeItem(pos);
    ui.listAccounts->insertItem(pos-1, item);
    ui.listAccounts->setCurrentRow(pos-1);

    setUpDownButtonsEnabled();
} // on_btnUp_clicked()

void GMailNotifierAppletConfig::on_btnDown_clicked()
{
    int pos = ui.listAccounts->currentRow();
    QListWidgetItem *item = ui.listAccounts->takeItem(pos);
    ui.listAccounts->insertItem(pos+1, item);
    ui.listAccounts->setCurrentRow(pos+1);

    setUpDownButtonsEnabled();
} // on_btnDown_clicked()

void GMailNotifierAppletConfig::on_listAccounts_itemPressed(QListWidgetItem *item)
{
    QVariantMap data(item->data(Qt::UserRole).toMap());
    ui.leLogin->setText(data["Login"].toString());
    ui.lePassword->setText(data["Password"].toString());
    ui.leDisplay->setText(data["Display"].toString());

    ui.btnDelete->setEnabled(true);
    setUpDownButtonsEnabled();
} // on_listAccounts_itemPressed()

void GMailNotifierAppletConfig::setAddModifyButtonEnabled()
{
    bool enabled = (!ui.leLogin->text().isEmpty() &&
                    !ui.lePassword->text().isEmpty());
    ui.btnAddModify->setEnabled(enabled);
} // setAddModifyButtonEnabled()

void GMailNotifierAppletConfig::setUpDownButtonsEnabled()
{
    bool enabled = (ui.listAccounts->count() >= 2 &&
                    ui.listAccounts->currentRow() != -1);

    bool upEnabled = (enabled && ui.listAccounts->currentRow() != 0);
    bool downEnabled = (enabled &&
                        ui.listAccounts->currentRow() + 1 != ui.listAccounts->count());

    ui.btnUp->setEnabled(upEnabled);
    ui.btnDown->setEnabled(downEnabled);
} // setUpDownButtonsEnabled()

void GMailNotifierAppletConfig::adaptAddModifyButtonLabel()
{
    if (ui.leLogin->text().isEmpty() || ui.lePassword->text().isEmpty()) {
        return;
    }

    QString buttonText;
    if (accountPosition(ui.leLogin->text()) != -1) {
        buttonText = i18n("Modify");
    } else {
        buttonText = i18n("Add");
    }

    if (buttonText != ui.btnAddModify->text().remove('&')) {
        ui.btnAddModify->setText(buttonText);
    }
} // adaptAddModifyButtonLabel()


/*
** private
*/
int GMailNotifierAppletConfig::accountPosition(const QString &email)
{
    int i=0;
    int pos=-1;
    while(i < ui.listAccounts->count()) {
        QListWidgetItem *item(ui.listAccounts->item(i));
        if (email == item->data(Qt::UserRole).toMap()["Login"]) {
            pos = i;
            break;
        }
        ++i;
    }

    return pos;
} // accountPosition()


#include "gmailnotifierappletconfig.moc"
