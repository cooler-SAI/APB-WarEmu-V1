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

using Common;
using FrameWork;

namespace WorldServer
{
    public class AccountChars
    {
        public int AccountId;
        public GameData.Realms _Realm = GameData.Realms.REALMS_REALM_NEUTRAL;

        public AccountChars(int AccountId)
        {
            this.AccountId = AccountId;
        }

        public Character[] _Chars = new Character[CharMgr.MAX_SLOT];

        public byte GenerateFreeSlot()
        {
            for (byte i = 0; i < _Chars.Length; i++)
                if (_Chars[i] == null)
                    return i;

            return CharMgr.MAX_SLOT;
        }
        public bool AddChar(Character Char)
        {
            if (Char != null)
                _Realm = (GameData.Realms)Char.Realm;

            if (_Chars[Char.SlotId] == null)
                _Chars[Char.SlotId] = Char;
            else
                return false;

            return true;
        }
        public int RemoveCharacter(byte Slot)
        {
            int CharacterId = -1;
            if (_Chars[Slot] != null)
                CharacterId = _Chars[Slot].CharacterId;

            _Chars[Slot] = null;
            _Realm = GameData.Realms.REALMS_REALM_NEUTRAL;

            foreach(Character Char in _Chars)
                if (Char != null)
                {
                    _Realm = (GameData.Realms)Char.Realm;
                    break;
                }

            return CharacterId;
        }
        public Character GetCharacterBySlot(byte Slot)
        {
            if (Slot < 0 || Slot > _Chars.Length)
                return null;

            return _Chars[Slot];
        }
    };

    static public class CharMgr
    {
        static public MySQLObjectDatabase Database = null;

        #region CharacterInfo

        static public Dictionary<byte, CharacterInfo> _Infos = new Dictionary<byte, CharacterInfo>();
        static public Dictionary<byte, List<CharacterInfo_item>> _InfoItems = new Dictionary<byte, List<CharacterInfo_item>>();
        static public Dictionary<byte, List<CharacterInfo_stats>> _InfoStats = new Dictionary<byte, List<CharacterInfo_stats>>();
        static public List<Random_name> _RandomNames = new List<Random_name>();

        [LoadingFunction(true)]
        static public void LoadCharacterInfo()
        {
            IList<CharacterInfo> Chars = WorldMgr.Database.SelectAllObjects<CharacterInfo>();
            foreach (CharacterInfo Info in Chars)
                if (!_Infos.ContainsKey(Info.Career))
                    _Infos.Add(Info.Career, Info);

            _RandomNames = WorldMgr.Database.SelectAllObjects<Random_name>().ToList<Random_name>();

            Log.Success("CharacterMgr", "Loaded " + Chars.Count + " CharacterInfo");
        }

        [LoadingFunction(true)]
        static public void LoadCharacterInfoItems()
        {
            IList<CharacterInfo_item> Chars = WorldMgr.Database.SelectAllObjects<CharacterInfo_item>();
            
            if(Chars != null)
            foreach (CharacterInfo_item Info in Chars)
                if (!_InfoItems.ContainsKey(Info.CareerLine))
                {
                    List<CharacterInfo_item> Items = new List<CharacterInfo_item>(1);
                    Items.Add(Info);
                    _InfoItems.Add(Info.CareerLine, Items);
                }
                else _InfoItems[Info.CareerLine].Add(Info);

            Log.Success("CharacterMgr", "Loaded " + _InfoItems.Count + " CharacterInfo_Item");
        }

        [LoadingFunction(true)]
        static public void LoadCharacterInfoStats()
        {
            IList<CharacterInfo_stats> Chars = WorldMgr.Database.SelectAllObjects<CharacterInfo_stats>();
            foreach (CharacterInfo_stats Info in Chars)
                if (!_InfoStats.ContainsKey(Info.CareerLine))
                {
                    List<CharacterInfo_stats> Stats = new List<CharacterInfo_stats>(1);
                    Stats.Add(Info);
                    _InfoStats.Add(Info.CareerLine, Stats);
                }
                else _InfoStats[Info.CareerLine].Add(Info);

            Log.Success("CharacterMgr", "Loaded " + Chars.Count + " CharacterInfo_Stats");
        }

        static public CharacterInfo GetCharacterInfo(byte Career)
        {
            lock (_Infos)
                if (_Infos.ContainsKey(Career))
                    return _Infos[Career];

            return null;
        }
        static public CharacterInfo_stats[] GetCharacterInfoStats(byte CareerLine, byte Level)
        {
            List<CharacterInfo_stats> _Stats = _InfoStats.ContainsKey(CareerLine) ? _InfoStats[CareerLine] : new List<CharacterInfo_stats>();

            List<CharacterInfo_stats> Stats = new List<CharacterInfo_stats>();
            foreach (CharacterInfo_stats Stat in _Stats)
                if (Stat.CareerLine == CareerLine && Stat.Level == Level)
                    Stats.Add(Stat);

            return Stats.ToArray();
        }
        static public CharacterInfo_item[] GetCharacterInfoItem(byte CareerLine)
        {
            return _InfoItems.ContainsKey(CareerLine) ? _InfoItems[CareerLine].ToArray() : new CharacterInfo_item[0];
        }

        static public Random_name[] GetRandomNames()
        {
            return _RandomNames.ToArray();
        }


        #endregion

        #region Characters

        static public byte MAX_SLOT = 10;
        static public int MAX_CHARACTERS = 1000000;
        static public int MAX_CHAR_GUID = 1;
        static public Character[] _Chars = new Character[MAX_CHARACTERS];
        static public Dictionary<int, AccountChars> _AcctChars = new Dictionary<int, AccountChars>();

        [LoadingFunction(true)]
        static public void LoadCharacters()
        {
            Character[] Chars = Database.SelectAllObjects<Character>().ToArray();

            foreach (Character Char in Chars)
                AddChar(Char);

            Log.Success("LoadCharacters", Chars.Length + "  : Character(s) loaded");
        }

        static public int GenerateMaxCharId()
        {
            return System.Threading.Interlocked.Increment(ref MAX_CHAR_GUID);
        }
        static public bool CreateChar(Character Char)
        {
            AccountChars Chars = GetAccountChar(Char.AccountId);
            Char.SlotId = Chars.GenerateFreeSlot();
            if (Char.SlotId < 0)
                return false;

            lock (_Chars)
            {
                int Id = GenerateMaxCharId();

                while (_Chars[Id] != null)
                    Id = GenerateMaxCharId();

                if (Id >= MAX_CHARACTERS || Id <= 0)
                {
                    Log.Error("CreateChar", "Maximum number of characters reaches !");
                    return false;
                }

                Char.CharacterId = Id;
                _Chars[Id] = Char;
                Chars.AddChar(Char);
            }
           Database.AddObject(Char);

            return true;
        }
        static public void AddChar(Character Char)
        {
            lock (_Chars)
            {
                _Chars[Char.CharacterId] = Char;
                GetAccountChar(Char.AccountId).AddChar(Char);

                if (Char.CharacterId > MAX_CHAR_GUID)
                    MAX_CHAR_GUID = Char.CharacterId;
            }
        }
        static public Character[] GetAccountCharacters(int AccountId)
        {
            return GetAccountChar(AccountId)._Chars;
        }
        static public AccountChars GetAccountChar(int AccountId)
        {
            lock (_AcctChars)
            {
                if (!_AcctChars.ContainsKey(AccountId))
                    _AcctChars[AccountId] = new AccountChars(AccountId);

                return _AcctChars[AccountId];
            }
        }
        static public bool NameIsUsed(string Name)
        {
            for (int i = 0; i < _Chars.Length; ++i)
            {
                if (_Chars[i] != null)
                {
                    if (_Chars[i].Name.ToLower() == Name.ToLower())
                        return true;
                }
            }

            return false;
        }
        static public byte[] BuildCharacters(int AccountId)
        {
            Log.Debug("BuildCharacters", "AcocuntId = " + AccountId);

            Character[] Chars = GetAccountChar(AccountId)._Chars;
            UInt16 Count = 0;

            // On Compte le nombre de personnages existant du joueur
            for (UInt16 c = 0; c < Chars.Length; ++c)
                if (Chars[c] != null) ++Count;

            PacketOut Out = new PacketOut(0);
            Out.Position = 0;
            Out.WriteUInt16(Count);

            Character Char = null;
            for (int i = 0; i < MAX_SLOT; ++i)
            {
                Char = Chars[i];

                if (Char == null)
                    Out.Write(new byte[280], 0, 280);
                else
                {
                    List<Character_items> Items = CharMgr.GetItemChar(Char.CharacterId);

                    Out.FillString(Char.Name, 48);
                    Out.WriteByte(Char.Value[0].Level);
                    Out.WriteByte(Char.Career);
                    Out.WriteByte(Char.Realm);
                    Out.WriteByte(Char.Sex);
                    Out.WriteByte(Char.ModelId);
                    Out.WriteUInt16(Char.Value[0].ZoneId);
                    Out.Write(new byte[5], 0, 5);

                    Character_items Item = null;
                    for (UInt16 SlotId = 14; SlotId < 30; ++SlotId)
                    {
                        Item = Items.Find(item => item != null && item.SlotId == SlotId);
                        if (Item == null)
                            Out.WriteUInt32(0);
                        else
                            Out.WriteUInt32R(Item.ModelId);

                        Out.Write(new byte[4], 0, 4);
                    }

                    Out.Write(new byte[6], 0, 6);

                    for (int j = 0; j < 5; ++j)
                    {
                        Out.Write(new byte[6], 0, 6);
                        Out.WriteUInt16(0xFF00);
                    }

                    for (UInt16 SlotId = 10; SlotId < 13; ++SlotId)
                    {
                        Item = Items.Find(item => item != null && item.SlotId == SlotId);
                        Out.WriteUInt16(0);
                        if (Item == null)
                            Out.WriteUInt16(0);
                        else
                            Out.WriteUInt16R((ushort)Item.ModelId);
                    }

                    Out.Write(new byte[10], 0, 10);
                    Out.WriteUInt16(0xFF00);
                    Out.WriteByte(0);
                    Out.WriteByte(Char.Race);
                    Out.WriteUInt16(0);
                    Out.Write(Char.bTraits, 0, Char.bTraits.Length);
                    Out.Write(new byte[10], 0, 10);
                }
            }
            return Out.ToArray();
        }
        static public GameData.Realms GetAccountRealm(int AccountId)
        {
            return GetAccountChar(AccountId)._Realm;
        }
        static public void RemoveCharacter(byte Slot, int AccountId)
        {
            int CharacterId = GetAccountChar(AccountId).RemoveCharacter(Slot);

            lock(_Chars)
                if (CharacterId >= 0 &&_Chars[CharacterId] != null)
                {
                    Log.Debug("RemoveCharacter", "Slot=" + Slot + ",Acct=" + AccountId + ",CharId=" + CharacterId);

                    RemoveItemsChar(CharacterId);
                    Database.DeleteObject(_Chars[CharacterId]);
                    _Chars[CharacterId] = null;
                }
        }
        static public Character GetCharacter(string Name)
        {
            Name = Name.ToLower();

            lock(_Chars)
                for (int i = 0; i < _Chars.Length; ++i)
                    if (_Chars[i] != null && _Chars[i].Name.ToLower() == Name)
                        return _Chars[i];

            return null;
        }

        #endregion

        #region CharacterItems

        static public long MAX_ITEMS = 600000;
        static public Character_items[] _Items;
        static public Dictionary<int,List<Character_items>> _CharItems = new Dictionary<int,List<Character_items>>();

        [LoadingFunction(true)]
        static public void LoadItems()
        {
            _Items = new Character_items[MAX_ITEMS];
            IList<Character_items> Items = Database.SelectAllObjects<Character_items>();

            if(Items != null)
                lock (_Items)
                    foreach (Character_items Itm in Items)
                        LoadItem(Itm);

            Log.Success("LoadItems",Items.Count + " : Characters items loaded");
        }
        static public void LoadItem(Character_items CharItem)
        {
            lock (_Items)
            {
                _Items[CharItem.Guid] = CharItem;

                if (!_CharItems.ContainsKey(CharItem.CharacterId))
                    _CharItems.Add(CharItem.CharacterId, new List<Character_items>() { CharItem });
                else
                    _CharItems[CharItem.CharacterId].Add(CharItem);
            }
        }
        static public void DeleteItem(Character_items Itm)
        {
            Log.Info("DeleteItem", "Guid=" + Itm.Guid + ",CharId=" + Itm.CharacterId);

            lock (_Items)
            {
                if (_CharItems.ContainsKey(Itm.CharacterId))
                    _CharItems[Itm.CharacterId].Remove(Itm);

                _Items[Itm.Guid] = null;
            }

            CharMgr.Database.DeleteObject(Itm);
        }

        static public List<Character_items> GetItemChar(int CharacterId)
        {
            lock (_Items)
            {
                if (_CharItems.ContainsKey(CharacterId))
                    return _CharItems[CharacterId];
                else
                    return new List<Character_items>();
            }
        }

        static public bool CreateItem(Character_items Item)
        {
            lock(_Items)
                for(long i=0;i<_Items.Length;++i)
                    if (_Items[i] == null)
                    {
                        Item.Guid = i;
                        LoadItem(Item);
                        Database.AddObject(Item);
                        return true;
                    }


            Log.Error("CreateItem", "Maximum number of items reaches !");
            return false;
        }
        static public void RemoveItemsChar(int CharacterId)
        {
            lock (_Items)
            {
                for (int i = 0; i < _Items.Length; ++i)
                    if (_Items[i] != null && _Items[i].CharacterId == CharacterId)
                    {
                        CharMgr.Database.DeleteObject(_Items[i]);
                        _Items[i] = null;
                    }

                _CharItems.Remove(CharacterId);
            }
        }
        static public void SaveItems(int CharacterId,Item[] CItems)
        {
            List<Character_items> CItem = new List<Character_items>();
            for (UInt16 i = 0; i < CItems.Length; ++i)
                if(CItems[i] != null)
                    CItem.Add(CItems[i].Save(CharacterId));

            lock (_Items)
            {
                _CharItems.Remove(CharacterId);
                _CharItems.Add(CharacterId, CItem);
            }
        }

        #endregion
    }
}
