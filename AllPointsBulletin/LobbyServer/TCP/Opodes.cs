﻿/*
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

[Flags]
public enum Opcodes : uint
{
    ASK_LOGIN = 0x3E8,
    LOGIN_PROOF = 0x3E9,
    ASK_CHARACTER_INFO = 0x3EA,
    ASK_WORLD_LIST = 0x3EB,
    ASK_CHARACTER_NAME_CHECK = 0x3EC,
    ASK_CHARACTER_NAME_CHANGE = 0x3ED,
    ASK_CHARACTER_CREATE = 0x3EE,
    ASK_CHARACTER_DELETE = 0x3EF,
    ASK_WORLD_ENTER = 0x3F0,
    ASK_CONFIGFILE_LOAD = 0x3F1,
    ASK_CONFIGFILE_SAVE = 0x3F2,

    // Login Server OpCodes, S->C
    ERROR = 0x7D0,
    KICK = 0x7D1,
    LOGIN_PUZZLE = 0x7D2,
    LOGIN_SALT = 0x7D3,
    ANS_LOGIN_SUCCESS = 0x7D4,
    ANS_LOGIN_FAILED = 0x7D5,
    CHARACTER_LIST = 0x7D6,
    ANS_CHARACTER_INFO = 0x7D7,
    WORLD_LIST = 0x7D8,
    ANS_CHARACTER_NAME_CHECK = 0x7D9,
    ANS_CHARACTER_NAME_CHANGE = 0x7DA,
    ANS_CHARACTER_CREATE = 0x7DB,
    ANS_CHARACTER_DELETE = 0x7DC,
    ANS_WORLD_ENTER = 0x7DD,
    WORLD_STATUS = 0x7DE,
    ANS_CONFIGFILE_LOAD = 0x7DF,
    ANS_CONFIGFILE_SAVE = 0x7E0,
    POINTS_CHANGE = 0x7E1,
};