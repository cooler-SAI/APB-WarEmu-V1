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
            Log.Success("CharacterListRequest", "Characters For : " + From.GetIp + " RPC : " + From.Rm.RpcInfo.Description());

            if (From.Acct == null || From.Rm == null)
                return;

            LobbyCharacterListResponse ListRp = new LobbyCharacterListResponse();
            Character[] Chars = From.Rm.GetObject<CharactersMgr>().GetCharacters(From.Acct.Id);
            foreach (Character Char in Chars)
                ListRp.Characters.Add(Char);
            From.SendSerialized(ListRp);

            Log.Success("Characters","Count = " + ListRp.Characters.Count);
        }
    }
}
