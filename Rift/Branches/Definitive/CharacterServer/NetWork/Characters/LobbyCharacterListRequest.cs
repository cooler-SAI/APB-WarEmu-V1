using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace CharacterServer
{
    [ISerializableAttribute((long)Opcodes.LobbyCharacterListRequest)]
    public class LobbyCharacterListRequest : ISerializablePacket
    {
        public override void OnRead(RiftClient From)
        {
            Log.Success("CharacterListRequest", "Characters Requested For : " + From.GetIp);

            if (From.Acct == null || From.Rm == null)
                return;

            Character[] Chars = RpcServer.GetObject<CharacterMgr>(From.Rm.RpcInfo).GetCharacters(From.Acct.Id);

            LobbyCharacterListResponse ListRp = new LobbyCharacterListResponse();

            foreach (Character Char in Chars)
            {
                LobbyCharacterEntry Entry = new LobbyCharacterEntry();
                Entry.AccountId = Char.AccountId;
                Entry.Email = From.Acct.Email;
                Entry.CharacterId = Char.Id;
                Entry.CharacterName = Char.Name;
                ListRp.Characters.Add(Entry);
            }

            From.SendSerialized(ListRp);
        }
    }
}
