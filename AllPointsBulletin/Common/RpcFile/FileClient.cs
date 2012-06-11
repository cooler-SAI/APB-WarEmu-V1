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

using FrameWork;

namespace Common
{
    public class FileClient
    {
        private int _AcctId;
        private string BaseFolder = "/ClientFiles";
        private string Folder = Directory.GetCurrentDirectory();
        public bool CharInited = false;

        private Dictionary<string, ConfigFile> _Files = new Dictionary<string, ConfigFile>();

        public FileClient(int AcctId)
        {
            _AcctId = AcctId;
            CheckBase();
        }

        public string GetClientDir()
        {
            return Folder + BaseFolder + "/" + _AcctId;
        }
        public string GetClientDir(string WorldName, string CharName)
        {
            return GetClientDir() + "/" + WorldName + "/" + CharName;
        }

        public void CheckBase()
        {
            if (CharInited)
                return;
            Log.Info("FileClient", "CheckBase : " + GetClientDir());
            try
            {
                CharInited = true;
                Directory.CreateDirectory(GetClientDir());
            }
            catch (Exception e)
            {

            }

            DirectoryInfo Dir = new DirectoryInfo(GetClientDir());

            foreach (FileInfo F in Dir.GetFiles("*.ini",SearchOption.AllDirectories))
            {
                ConfigFile File = new ConfigFile(F.Name, F.DirectoryName, true);
                _Files.Add(F.Name, File);
            }
        }

        public void CheckCharBase(string WorldName,string CharName)
        {
            try
            {
                Directory.CreateDirectory(GetClientDir(WorldName,CharName));
            }
            catch (Exception e)
            {

            }
        }

        public ConfigFile GetConf(string name,bool create)
        {
            Log.Info("FileClient", "GetConf : " + name);

            if (_Files.ContainsKey(name))
                return _Files[name];
            else
            {
                Log.Debug("FileClient", "Le fichier n'existe pas !");

                if (!create || name.Length <= 0)
                    return null;
                else
                {
                    string dir = GetClientDir();
                    ConfigFile F = new ConfigFile(name, dir , true);
                    _Files.Add(name, F);
                    return F;
                }
            }
        }

        public ConfigFile GetConf(string name, string WorldName, string CharName, bool create)
        {
            CheckCharBase(WorldName, CharName);

            Log.Info("FileClient", "GetConf : " + name + ", CharName="+CharName);
            if (_Files.ContainsKey(name))
                return _Files[name];
            else
            {
                Log.Debug("FileClient", "Le fichier n'existe pas !");

                if (!create || WorldName.Length <=0 || CharName.Length <= 0)
                    return null;
                else
                {
                    string dir = GetClientDir(WorldName, CharName);
                    ConfigFile F = new ConfigFile(name, dir, true);
                    _Files.Add(name, F);
                    return F;
                }
            }
        }

        public int GetVersion(string name)
        {
            ConfigFile File = GetConf(name,true);

            if (File == null)
                return 0;
            else return File.Version;
        }
    }
}
