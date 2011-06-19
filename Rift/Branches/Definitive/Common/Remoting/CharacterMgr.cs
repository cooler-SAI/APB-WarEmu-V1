using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [Rpc(false, System.Runtime.Remoting.WellKnownObjectMode.Singleton, 1)]
    public class CharacterMgr : RpcObject
    {
        static public MySQLObjectDatabase CharactersDB;

        #region Realm

        static public Realm MyRealm;
        static public RpcClient Client;

        public override void OnServerConnected()
        {
            if(MyRealm != null && Client != null)
                Client.GetServerObject<AccountMgr>().RegisterRealm(MyRealm, Client.Info);
        }


        #endregion

        #region Characters

        public Character[] GetCharacters(long AccountId)
        {
            return CharactersDB.SelectObjects<Character>("AccountId=" + AccountId).ToArray();
        }

        public int GetCharactersCount(long AccountId)
        {
            return CharactersDB.GetObjectCount<Character>("AccountId=" + AccountId);
        }

        public LobbyCharacterListResponse GetCharactersList(long AccountId)
        {
            Character[] Chars = GetCharacters(AccountId);

            LobbyCharacterListResponse Rp = new LobbyCharacterListResponse();
            foreach (Character Char in Chars)
            {
                LobbyCharacterEntry Entry = new LobbyCharacterEntry();
                Entry.AccountId = Char.AccountId;
                Entry.CharacterId = Char.Id;
                Entry.CharacterName = Char.Name;
                Rp.Characters.Add(Entry);
            }

            return Rp;
        }

        public List<ISerializablePacket> GetCreationCache()
        {
            List<ISerializablePacket> Caches = new List<ISerializablePacket>();

            return Caches;

        }

        #endregion
    }
}
