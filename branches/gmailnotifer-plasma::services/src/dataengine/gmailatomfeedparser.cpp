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
#include "gmailatomfeedparser.h"

// Qt
#include <QtCore/QByteArray>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>


Plasma::DataEngine::Data GmailAtomFeedParser::parseFeed(const QByteArray &feed)
{
    Plasma::DataEngine::Data data;
    QList<QVariant>          entries;

    QDomDocument doc("GmailATOMFeed");
    doc.setContent(feed);

    QDomElement de = doc.documentElement();
    QDomNode dn = de.firstChild();
    while(!dn.isNull())
    {
        if (dn.isElement()) {
            QDomElement de = dn.toElement();
            QString tag(de.tagName().toLower());

            if (tag == "title") {
                data["title"] = de.text();
            }
            else if (tag == "tagline") {
                data["tagline"] = de.text();
            }
            else if (tag == "fullcount") {
                data["fullcount"] = de.text().toInt();
            }
            else if (tag == "link") {
                data["link"] = QUrl(de.attribute("href"), QUrl::StrictMode);
            }
            else if (tag == "modified") {
                QDateTime dt = QDateTime::fromString(de.text(), Qt::ISODate);
                dt.setTimeSpec(Qt::UTC);
                data["modified"] = dt;
            }
            else if (tag == "entry") {
                entries << parseEntry(dn);
            }

            dn = dn.nextSibling();
        }
    }

    data["entries"] = entries;

    return data;
} // parseFeed()


QVariantMap GmailAtomFeedParser::parseEntry(const QDomNode &node)
{
    QVariantMap mail;

    QDomNode dn = node.firstChild();
    while(!dn.isNull())
    {
        if (dn.isElement()) {
            QDomElement de = dn.toElement();
            QString tag(de.tagName().toLower());

            if (tag == "title") {
                mail["title"] = de.text();
            }
            else if (tag == "summary") {
                mail["summary"] = de.text();
            }
            else if (tag == "link") {
                mail["link"] = QUrl(de.attribute("href"), QUrl::StrictMode);
            }
            else if (tag == "modified") {
                QDateTime dt = QDateTime::fromString(de.text(), Qt::ISODate);
                dt.setTimeSpec(Qt::UTC);
                mail["modified"] = dt;
            }
            else if (tag == "issued") {
                QDateTime dt = QDateTime::fromString(de.text(), Qt::ISODate);
                dt.setTimeSpec(Qt::UTC);
                mail["issued"] = dt;
            }
            else if (tag == "id") {
                mail["id"] = de.text();
            }
            else if (tag == "author") {
                QDomNode dn = de.firstChild();
                while(!dn.isNull())
                {
                    if (dn.isElement()) {
                        QDomElement de = dn.toElement();
                        QString tagName(de.tagName().toLower());

                        if (tagName == "name") {
                            mail["author_name"] = de.text();
                        }
                        else if (tagName == "email") {
                            mail["author_email"] = de.text();
                        }
                    }

                    dn = dn.nextSibling();
                }
            } 
        }

        dn = dn.nextSibling();
    }

    return mail;
} // parseEntry()
