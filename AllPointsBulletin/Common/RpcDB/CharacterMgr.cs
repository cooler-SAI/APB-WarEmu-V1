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
using System.Net;

using FrameWork;

namespace Common
{
    [Rpc(true, System.Runtime.Remoting.WellKnownObjectMode.Singleton,0)]
    public class CharacterMgr : RpcObject
    {
        static private MySQLObjectDatabase _Database;
        static public MySQLObjectDatabase Database
        {
            get { return _Database; }
            set
            {
                _Database = value;
                LoadWorlds();
            }
        }

        #region Loader

        // Sauvegarde de la Configuration des comptes
        static private Dictionary<int, DBAccount> _Accounts = new Dictionary<int, DBAccount>();
        static private Dictionary<string, DBAccount> _AcctName = new Dictionary<string, DBAccount>();

        // Classement par compte (int) et par slot (byte) des personnages
        static private Dictionary<int, Dictionary<byte, CharacterInfo>> _AcctChars = new Dictionary<int, Dictionary<byte, CharacterInfo>>();

        // Classement Par Nom des personnages
        static private Dictionary<string, CharacterInfo> _NameChars = new Dictionary<string, CharacterInfo>();

        // Classement Par ID des personnages
        static private Dictionary<UInt32, CharacterInfo> _IdChars = new Dictionary<uint, CharacterInfo>();

        // Ajout d'un Character dans toutes les liste
        public void AddCharacter(CharacterInfo Info)
        {
            int AcctId = Info.Character.AcctId;

            if (!_Accounts.ContainsKey(AcctId))
                return;

            Log.Info("AddCharacter", "Ajout du personnage : " + Info.Character.Name);
            lock (_AcctChars)
            {
                Dictionary<byte, CharacterInfo> Dic = null;

                if (_AcctChars.ContainsKey(AcctId))
                    Dic = _AcctChars[AcctId];
                else
                {
                    Dic = new Dictionary<byte, CharacterInfo>();
                    _AcctChars.Add(AcctId, Dic);
                }

                Dic.Add(Info.Character.SlotId, Info);
            }

            if (!_IdChars.ContainsKey((UInt32)Info.Character.Id))
                _IdChars.Add((UInt32)Info.Character.Id, Info);

            string Name = Info.Character.Name.ToUpper();

            lock (_NameChars)
                if (!_NameChars.ContainsKey(Name))
                    _NameChars.Add(Name, Info);
        }

        public void CreateCharacter(DBCharacter Char)
        {
            Log.Info("CreateCharacter", "Creation du personnage : " + Char.Name);

            Database.AddObject(Char);
            AddCharacter(new CharacterInfo(Char));
        }

        public void DeleteCharacter(int AcctId, byte slotid)
        {
            CharacterInfo Info = GetInfoBySlotId(AcctId, slotid);

            if (Info == null)
                return;

            lock (_AcctChars)
                if (_AcctChars.ContainsKey(Info.Character.AcctId))
                    if (_AcctChars[Info.Character.AcctId].ContainsKey(slotid))
                        _AcctChars[Info.Character.AcctId].Remove(slotid);

            lock (_IdChars)
                if (_IdChars.ContainsKey((uint)Info.Character.Id))
                    _IdChars.Remove((uint)Info.Character.Id);

            lock (_NameChars)
                if (_NameChars.ContainsKey(Info.Character.Name))
                    _NameChars.Remove(Info.Character.Name);

            Database.DeleteObject(Info.Character);

            Info.Character = null;
            Info = null;
        }

        public bool LoadAccount(int Id)
        {
            Log.Info("LoadAccount", "Chargement du compte : " + Id);

            if (_Accounts.ContainsKey(Id))
                return true;

            DBAccount Acct = Database.SelectObject<DBAccount>("Id='" + Id + "'");
            if (Acct == null)
                return false;

            lock(_Accounts)
                _Accounts.Add(Id, Acct);

            lock (_AcctName)
                _AcctName.Add(Acct.Email.ToUpper(), Acct);

            return true;
        }

        public bool LoadAccount(string Email)
        {
            Log.Info("LoadAccount", "Chargement du compte : " + Email);

            Email = Email.ToUpper();

            if (_AcctName.ContainsKey(Email))
                return true;

            DBAccount Acct = Database.SelectObject<DBAccount>("Email='" + Database.Escape(Email) + "'");

            if (Acct == null)
                return false;

            lock (_Accounts)
                _Accounts.Add(Acct.Id, Acct);

            lock (_AcctName)
                _AcctName.Add(Acct.Email.ToUpper(), Acct);

            return true;
        }

        // Chargement d'un personnage par son ID
        public bool LoadCharacter(UInt32 Id)
        {
            Log.Info("LoadCharacter", "Chargement du personnage : " + Id);

            if (_IdChars.ContainsKey(Id))
                return true;

            DBCharacter Char = Database.SelectObject<DBCharacter>("ID='" + Id + "'");

            if (Char == null)
                return false;

            AddCharacter(new CharacterInfo(Char));
            return true;
        }

        // Chargement des personnage d'un compte
        public bool LoadCharacters(int AcctId)
        {
            Log.Info("LoadCharacters", "Chargement des personnages de : " + AcctId);

            if (_AcctChars.ContainsKey((int)AcctId))
                return true;

            if (!LoadAccount(AcctId))
                return false;

            IList<DBCharacter> Chars = Database.SelectObjects<DBCharacter>("AcctId='"+AcctId+"'");

            foreach(DBCharacter Char in Chars)
                 AddCharacter(new CharacterInfo(Char));

            return true;
        }

        #endregion

        #region Accessor

        public DBAccount GetAccount(int Id)
        {
            lock (_Accounts)
                if (_Accounts.ContainsKey(Id))
                    return _Accounts[Id];

            return null;
        }

        public DBAccount GetAccount(string Email)
        {
            lock (_AcctName)
                if (_AcctName.ContainsKey(Email))
                    return _AcctName[Email];
            return null;
        }

        public CharacterInfo GetInfoById(UInt32 CharacterId)
        {
            lock (_IdChars)
                if (_IdChars.ContainsKey(CharacterId))
                    return _IdChars[CharacterId];
                else return null;
        }

        public CharacterInfo GetInfoBySlotId(int AcctId, byte SlotId)
        {
            lock (_AcctChars)
                if (_AcctChars.ContainsKey(AcctId))
                    if (_AcctChars[AcctId] != null && _AcctChars[AcctId].ContainsKey(SlotId))
                        return _AcctChars[AcctId][SlotId];

            return null;
        }

        public CharacterInfo GetInfoByName(string Name)
        {
            Name = Name.ToUpper();

            lock (_NameChars)
                if (_NameChars.ContainsKey(Name))
                    return _NameChars[Name];

            return null;
        }

        public CharacterInfo[] GetInfos(int AcctId)
        {
            lock (_AcctChars)
                if (_AcctChars.ContainsKey(AcctId) && _AcctChars[AcctId] != null)
                    return _AcctChars[AcctId].Values.ToArray();

            return new CharacterInfo[0];
        }

        public CharacterInfo GetInfoForEnter(int AcctId)
        {
            CharacterInfo[] Infos = GetInfos(AcctId);
            foreach (CharacterInfo Info in Infos)
                if (Info.Enter)
                    return Info;

            return null;
        }

        public byte GetFreeSlot(int AcctId)
        {
            List<byte> FSlots = new List<byte>(8);
            for (byte i = 1; i < 9; ++i)
                FSlots.Add(i);

            CharacterInfo[] Chars = GetInfos(AcctId);
            if (Chars.Length >= 8)
                return 0;

            foreach (CharacterInfo Info in Chars)
                FSlots.Remove(Info.Character.SlotId);

            if (FSlots.Count <= 0)
                return 0;

            return FSlots[0];
        }

        #endregion

        #region Function

        public void SetEnter(int AcctId,byte slotid)
        {
            try
            {
                CharacterInfo Info = GetInfoBySlotId(AcctId, slotid);

                CharacterInfo[] Infos = GetInfos(Info.Character.AcctId);
                foreach (CharacterInfo Inf in Infos)
                    Inf.Enter = false;

                if (Info != null)
                {
                    Log.Info("Enter", Info.Character.Name + " entre en jeu");
                    Info.Enter = true;
                }
            }
            catch (Exception e)
            {
                Log.Error("SetEnter", e.ToString());
            }
        }

        public void SetAccountWorld(int AcctId,int WorldId)
        {
            try
            {
                DBAccount Acct = GetAccount(AcctId);

                if (Acct != null)
                {
                    Acct.WorldId = WorldId;
                    _Database.SaveObject(Acct);
                }
            }
            catch (Exception e)
            {
                Log.Error("SetAccountWorld", e.ToString());
            }
        }

        #endregion

        #region World

        // Liste des worlds, WorldID,DBWorld
        static public List<WorldInfo> _Worlds = new List<WorldInfo>();

        static public void LoadWorlds()
        {
            IList<DBWorldInfo> Worlds = Database.SelectAllObjects<DBWorldInfo>();
            foreach (DBWorldInfo World in Worlds)
            {
                Log.Info("LoadWorlds", "Ajout du world : " + World.Name);
                _Worlds.Add(new WorldInfo(World));
            }
        }

        public bool RegisterWorld(int RpcId,int WorldId, string Name, int port,string Ip)
        {
            Log.Info("RegisterWorld", "Enregistrement du world : " + Name + ", RpcID = " + RpcId + ",Ip=" + Ip + ":" + port);

            int iIp = BitConverter.ToInt32(IPAddress.Parse(Ip).GetAddressBytes(), 0);

            WorldInfo Info = null;
            foreach (WorldInfo World in _Worlds)
            {
                if (World._Info.Id == WorldId)
                    Info = World;
            }

            if (Info == null)
            {
                Log.Debug("RegisterWorld", "Le World n'existe pas, création");

                DBWorldInfo World = new DBWorldInfo();
                World.Name = Name;
                World.Id = WorldId;
                World.Population = 1;
                World.Status = 1;
                World.Crim = 1;
                World.Enf = 1;

                Info = new WorldInfo(World);
                Info.Port = port;
                Info.Ip = iIp;

                _Worlds.Add(Info);
                Database.AddObject(World);
            }
            else
            {
                Info.RpcID = RpcId;
                Info.Port = port;
                Info.Ip = iIp;
            }

            return true;
        }

        public WorldInfo GetWorldInfo(int Id)
        {
            foreach (WorldInfo Info in _Worlds)
                if (Info._Info.Id == Id)
                    return Info;

            return null;
        }

        public List<WorldInfo> GetWorlds()
        {
            return _Worlds;
        }

        #endregion

        public override void  OnClientDisconnected(RpcClientInfo Rpc)
        {
            Log.Info("CharMgr", "Déconnection de l'user : " + Rpc.RpcID);
            foreach (WorldInfo Info in _Worlds)
                if (Info.RpcID == Rpc.RpcID)
                {
                    Log.Debug("CharMgr", "Déconnection du world : " + Info._Info.Name);
                    Info.RpcID = 0;
                }
        }
    }
}
