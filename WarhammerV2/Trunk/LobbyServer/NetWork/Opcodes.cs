/*
 * Copyright (C) 2011 APS
 *	http://AllPrivateServer.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
using System;

enum Opcodes : ushort
{
    CMSG_CONNECT = 0x01,
    SMSG_CONNECT_RESPONSE = 0x02,
    CMSG_USER_LOGIN = 0x03,
    SMSG_USER_LOGIN_RESPONSE = 0x04,
    CMSG_AUTHENTICATE = 0x05,
    SMSG_AUTHENTICATE_RESPONSE = 0x06,
    CMSG_GET_SERVER_LIST = 0x07,
    SMSG_GET_SERVER_LIST_RESPONSE = 0x08,
    CMSG_GET_CHARACTER_SUMMARIES = 0x09,
    SMSG_GET_CHARACTER_SUMMARIES_RESPONSE = 0x0a,

    CMSG_REFRESH_AUTHENTICATION = 0x0d,
    SMSG_REFRESH_AUTHENTICATION_RESPONSE = 0x0e,
    CMSG_LOGIN_CLEARANCE = 0x0f,
    SMSG_LOGIN_CLEARANCE_RESPONSE = 0x10,
    CMSG_GET_PRODUCT_ACCOUNT_PROPERTY_LIST = 0x11,
    SMSG_GET_PRODUCT_ACCOUNT_PROPERTY_LIST_RESPONSE = 0x12,
    MAX_OPCODE = 0x12
};