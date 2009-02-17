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


// QtGui
#include <QtGui/QColor>
// QtCore
#include <QtCore/QString>


#ifndef __DEFAULTS_H__
#define __DEFAULTS_H__


// Default values
class Defaults
{
private:
    Defaults(){};

public:
    static int     pollingInterval;

    static bool    displayLogo;
    static QString background;
    static QString iconTextColor;
    static QString dialogTextColor;
};


#endif // __DEFAULTS_H__
