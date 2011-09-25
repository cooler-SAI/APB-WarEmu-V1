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
using System.IO;

namespace Common
{
    public class BlockInfo
    {
        private string _BlockName = "";
        private Dictionary<string, string> _Values = new Dictionary<string, string>();

        public BlockInfo(string name)
        {
            _BlockName = name;
        }

        public void AddValue(string name, string value)
        {
            if (_Values.ContainsKey(name))
                _Values[name] = value;
            else _Values.Add(name, value);
        }

        public T GetValue<T>(string name)
        {
            if (_Values.ContainsKey(name))
                return (T)Convert.ChangeType(_Values[name], typeof(T));
            else return (T)Convert.ChangeType(0, typeof(T));
        }

        public void SaveTo(StreamWriter SW)
        {
            SW.WriteLine("[" + _BlockName + "]");
            foreach (KeyValuePair<string, string> Pair in _Values)
                SW.WriteLine(Pair.Key + "=" + Pair.Value);
            SW.WriteLine();
        }

    }
}
