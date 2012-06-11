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
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

[DataTable(PreCache = true, TableName = "WorldsInfo", DatabaseName = "CharDB")]
[Serializable]
public class DBWorldInfo : DataObject
{
    private int _Id;
    private string _Name;
    private byte _Status;
    private byte _Population;
    private byte _Enf;
    private byte _Crim;

    public DBWorldInfo()
    {

    }

    [DataElement(Unique = true)]
    public int Id
    {
        get { return _Id; }
        set
        {
            _Id = value;
            Dirty = true;
        }
    }

    [DataElement(Unique = true, Varchar = 255)]
    public string Name
    {
        get{ return _Name; }
        set
        {
            _Name = value;
            Dirty = true;
        }
    }

    [DataElement]
    public byte Status
    {
        get
        {
            return _Status;
        }
        set
        {
            _Status = value;
            Dirty = true;
        }
    }

    [DataElement]
    public byte Population
    {
        get{ return _Population; }
        set
        {
            _Population = value;
            Dirty = true;
        }
    }

    [DataElement]
    public byte Enf
    {
        get { return _Enf; }
        set
        {
            _Enf = value;
            Dirty = true;
        }
    }

    [DataElement]
    public byte Crim
    {
        get { return _Crim; }
        set
        {
            _Crim = value;
            Dirty = true;
        }
    }
}