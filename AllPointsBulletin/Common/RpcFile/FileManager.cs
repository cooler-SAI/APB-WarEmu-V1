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

using FrameWork.Logger;
using FrameWork.RpcV3;

namespace Common
{
    [V3RpcAttributes(new string[] { "LobbyFile", "WorldFile", "FileServer" })]
    public class FileManager : ARpc
    {
        public Dictionary<int, FileClient> _Clients = new Dictionary<int, FileClient>();

        public string[] _LoginBaseName = new string[5]
        {
                "AccountTutorialSettings.ini",
                "AccountPersistentUI.ini",
                "AccountPersistent3.ini",
                "AccountPersistent4.ini",
                "AccountPersistent5.ini"
        };

        public string[] _WorldBaseName = new string[5]
        {
                "CharacterPersistent1.ini",
                "CharacterPersistent2.ini",
                "CharacterPersistent3.ini",
                "CharacterPersistent4.ini",
                "CharacterPersistent5.ini"
        };

        public FileClient GetFileClient(int AcctId)
        {
            if (_Clients.ContainsKey(AcctId))
                return _Clients[AcctId];
            else
            {
                FileClient F = new FileClient(AcctId);
                _Clients.Add(AcctId, F);
                return F;
            }
        }

        public string GetFileName(int id, bool login)
        {
            if (login)
            {
                if (id > _LoginBaseName.Length || id == 0)
                    return "";
                else return _LoginBaseName[id - 1];
            }
            else
            {
                if (id > _WorldBaseName.Length || id == 0)
                    return "";
                else return _WorldBaseName[id - 1];
            }
        }

        public ConfigFile GetFile(int AcctId, int id, bool login, string WorldName, string CharName)
        {
            Log.Info("FileManager", "GetFile Id=" + id);

            if (login)
                return GetFileClient(AcctId).GetConf(GetFileName(id, login), true);
            else
                return GetFileClient(AcctId).GetConf(GetFileName(id, login), WorldName, CharName, true);
        }

        public int GetFileVersion(int AcctId, int id, bool login, string WorldName, string CharName)
        {
            Log.Info("FileManager", "GetFileVersion Id=" + id + ",CharName=" + CharName);
            return GetFile(AcctId, id, login, WorldName, CharName).Version;
        }


        public byte[] GetFileByte(int AcctId, int id, bool login, string WorldName, string CharName)
        {
            Log.Info("FileManager", "GetFileByte Id=" + id);

            ConfigFile Conf = GetFileClient(AcctId).GetConf(GetFileName(id, login), WorldName, CharName, false);
            if (Conf != null)
                return Conf.TotalFile;
            else
                return new byte[0];
        }

        public void SaveInfo(int AcctId, int id, byte[] Info)
        {
            Log.Info("FileManager", "SaveInfo : " + AcctId);
            ConfigFile Conf = GetFile(AcctId, id, true, "", "");
            if (Conf != null)
                Conf.Write(Info);
        }

        public void SaveInfo(int AcctId, int id, string WorldName, string CharName, byte[] Info)
        {
            if (WorldName.Length <= 0 || CharName.Length <= 0)
                SaveInfo(AcctId, id, Info);
            else
            {
                Log.Info("FileManager", "SaveInfo : " + AcctId + "," + WorldName + "," + CharName);
                ConfigFile Conf = GetFile(AcctId, id, false, WorldName, CharName);
                if (Conf != null)
                    Conf.Write(Info);
            }
        }
    }
}
