/* Copyright (c) 2001 IBK-Landquart-Switzerland. All rights reserved.
 *
 * Module      :  ZCsl.h
 * Application :  CSL kernel
 * Purpose     :  Library settings
 *
 * Date        Description                                 Who
 * --------------------------------------------------------------------------
 * 2001.05.25  First implementation                        P.Koch, IBK
 * 2001.07.07  Renaming from css to csl                    P.Koch, IBK
 *
 * OPEN SOURCE LICENSE
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 2.1 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; if not, write to IBK at info@ibk-software.ch.
 */

#ifndef _ZCSL_H_
#define _ZCSL_H_

#include <ZBase.h>

#define CSL_MAJOR_VERSION 4
#define CSL_MINOR_VERSION 3

#define ZCslLink0 ZExport0
#define ZCslLink(type) ZExport(type)

#endif /* _ZCSL_H_ */
