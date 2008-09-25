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

// KDE
#include <KDE/KDebug>


/*
** public
*/
GmailNotifierAppletConfig::GmailNotifierAppletConfig(QWidget *parent)
    : QWidget(parent)
{
    kDebug();
    ui.setupUi(this);

    QStringList backgroundHints;
    //backgroundHints << "Default" << "Standard" << "Translucent" << "Shadowed" << "None";
    backgroundHints << "Standard" << "Translucent";
    ui.comboBackground->addItems(backgroundHints);
} // ctor()

GmailNotifierAppletConfig::~GmailNotifierAppletConfig()
{
    kDebug();
} // dtor()

void GmailNotifierAppletConfig::importConfig(const QVariantMap &data)
{
    kDebug();
    ui.cbDisplayLogo->setChecked(data["DisplayLogo"].toBool());

    int pos = ui.comboBackground->findText(data["Background"].toString(), Qt::MatchExactly);
    ui.comboBackground->setCurrentIndex(pos);

    ui.spinPollingInterval->setValue(data["PollingInterval"].toInt());

    QVariantList accounts(data["Accounts"].toList());
    foreach (QVariant data, accounts) {
        addItemToList(data.toMap());
    }
} // importConfig()

QVariantMap GmailNotifierAppletConfig::exportConfig()
{
    kDebug();
    QVariantMap data;
    data["DisplayLogo"] = ui.cbDisplayLogo->isChecked();
    data["Background"] = ui.comboBackground->currentText();
    data["PollingInterval"] = ui.spinPollingInterval->value();

    QVariantList accountList;
    for (int i=0; i<ui.listAccounts->count(); ++i) {
        QListWidgetItem *item(ui.listAccounts->item(i));
        QVariant accountInfos(item->data(Qt::UserRole));
        accountList << accountInfos;
    }
    data["Accounts"] = accountList;

    return data;
} // exportConfig()


/*
** private slots
*/
void GmailNotifierAppletConfig::on_leLogin_textChanged(const QString &text)
{
    kDebug();
    Q_UNUSED(text)
    setAddModifyButtonEnabled();
    adaptAddModifyButtonLabel();
} // on_leLogin_textChanged()

void GmailNotifierAppletConfig::on_leLabel_textChanged(const QString &text)
{
    kDebug();
    Q_UNUSED(text)
    setAddModifyButtonEnabled();
    adaptAddModifyButtonLabel();
} // on_leLabel_textChanged()

void GmailNotifierAppletConfig::on_lePassword_textChanged(const QString &text)
{
    kDebug();
    Q_UNUSED(text)
    setAddModifyButtonEnabled();
} // on_lePassword_textChanged()

void GmailNotifierAppletConfig::on_btnAddModify_clicked()
{
    kDebug();
    QVariantMap data;
    data["Login"]    = ui.leLogin->text();
    data["Password"] = ui.lePassword->text();
    data["Label"]    = ui.leLabel->text();
    data["Display"]  = ui.leDisplay->text();

    int pos = accountPosition(ui.leLogin->text(), ui.leLabel->text());
    if (pos == -1) {
        // Add
        addItemToList(data);
        setUpDownButtonsEnabled();
    } else {
        // Modify
        QListWidgetItem *item = ui.listAccounts->item(pos);
        item->setText(listItemText(data));
        item->setData(Qt::UserRole, data);
    }

    adaptAddModifyButtonLabel();
} // on_btnAddModify_clicked()

void GmailNotifierAppletConfig::on_btnDelete_clicked()
{
    kDebug();
//    ui.listAccounts->removeItemWidget(ui.listAccounts->currentItem());
    ui.listAccounts->takeItem(ui.listAccounts->currentRow());
    ui.listAccounts->setCurrentRow(-1);
    ui.btnDelete->setEnabled(false);
    adaptAddModifyButtonLabel();
    setUpDownButtonsEnabled();
} // on_btnDelete_clicked()

void GmailNotifierAppletConfig::on_btnUp_clicked()
{
    kDebug();
    moveItem(-1);

    setUpDownButtonsEnabled();
} // on_btnUp_clicked()

void GmailNotifierAppletConfig::on_btnDown_clicked()
{
    kDebug();
    moveItem(+1);

    setUpDownButtonsEnabled();
} // on_btnDown_clicked()

void GmailNotifierAppletConfig::on_listAccounts_itemPressed(QListWidgetItem *item)
{
    kDebug();
    QVariantMap data(item->data(Qt::UserRole).toMap());
    ui.leLogin->setText(data["Login"].toString());
    ui.lePassword->setText(data["Password"].toString());
    ui.leLabel->setText(data["Label"].toString());
    ui.leDisplay->setText(data["Display"].toString());

    ui.btnDelete->setEnabled(true);
    setUpDownButtonsEnabled();
    adaptAddModifyButtonLabel();
} // on_listAccounts_itemPressed()


/*
** private
*/
void GmailNotifierAppletConfig::setAddModifyButtonEnabled()
{
    kDebug();
    bool enabled = (!ui.leLogin->text().isEmpty() &&
                    !ui.lePassword->text().isEmpty());
    ui.btnAddModify->setEnabled(enabled);
} // setAddModifyButtonEnabled()

void GmailNotifierAppletConfig::setUpDownButtonsEnabled()
{
    kDebug();
    bool enabled = (ui.listAccounts->count() >= 2 &&
                    ui.listAccounts->currentRow() != -1);

    bool upEnabled = (enabled && ui.listAccounts->currentRow() != 0);
    bool downEnabled = (enabled &&
                        ui.listAccounts->currentRow() + 1 != ui.listAccounts->count());

    ui.btnUp->setEnabled(upEnabled);
    ui.btnDown->setEnabled(downEnabled);
} // setUpDownButtonsEnabled()

void GmailNotifierAppletConfig::adaptAddModifyButtonLabel()
{
    kDebug();
    if (ui.leLogin->text().isEmpty() || ui.lePassword->text().isEmpty()) {
        return;
    }

    QString buttonText;
    if (accountPosition(ui.leLogin->text(), ui.leLabel->text()) != -1) {
        buttonText = i18n("Modify");
    } else {
        buttonText = i18n("Add");
    }

    if (buttonText != ui.btnAddModify->text().remove('&')) {
        ui.btnAddModify->setText(buttonText);
    }
} // adaptAddModifyButtonLabel()

QString GmailNotifierAppletConfig::listItemText(const QVariantMap &data)
{
    kDebug();
    QString itemText;
    QString label = (data["Label"].toString().isEmpty()) ? "inbox" : data["Label"].toString();
    if (data["Display"].toString().isEmpty()) {
        itemText = QString("%1/%2").arg(data["Login"].toString()).arg(label);
    } else {
        itemText = QString("%1").arg(data["Display"].toString());
    }

    return itemText;
} // listItemText()

void GmailNotifierAppletConfig::addItemToList(const QVariantMap &data)
{
    kDebug();
    QListWidgetItem *item = new QListWidgetItem();
    item->setText(listItemText(data));
    item->setData(Qt::UserRole, data);
    ui.listAccounts->addItem(item);
//    ui.listAccounts->scrollToItem(item);
} // addItemToList()

int GmailNotifierAppletConfig::accountPosition(const QString &login, const QString &label)
{
    kDebug();
    int i=0;
    int pos=-1;
    while(i < ui.listAccounts->count()) {
        QListWidgetItem *item(ui.listAccounts->item(i));
        QVariantMap data(item->data(Qt::UserRole).toMap());
        if (login == data["Login"] && label == data["Label"]) {
            pos = i;
            break;
        }
        ++i;
    }

    return pos;
} // accountPosition()

void GmailNotifierAppletConfig::moveItem(const int &shift)
{
    kDebug();
    int pos = ui.listAccounts->currentRow();
    QListWidgetItem *item = ui.listAccounts->takeItem(pos);
    ui.listAccounts->insertItem(pos+shift, item);
    ui.listAccounts->setCurrentRow(pos+shift);
} // moveItem()


#include "gmailnotifierappletconfig.moc"
