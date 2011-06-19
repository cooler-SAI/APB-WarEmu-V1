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

            LobbyCharacterListResponse ListRp = From.Rm.GetObject<CharacterMgr>().GetCharactersList(From.Acct.Id);

            foreach (LobbyCharacterEntry Entry in ListRp.Characters)
                Entry.Email = From.Acct.Email;

            From.SendSerialized(ListRp);
        }
    }
}
