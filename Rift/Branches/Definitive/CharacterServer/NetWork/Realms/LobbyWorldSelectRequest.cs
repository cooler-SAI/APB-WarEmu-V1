using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace CharacterServer
{
    [ISerializableAttribute((long)Opcodes.LobbyWorldSelectRequest)]
    public class LobbyWorldSelectRequest : ISerializablePacket
    {
        [Unsigned7Bit(0)]
        public long RealmId;

        public override void OnRead(RiftClient From)
        {
            Realm Rm = Program.AcctMgr.GetRealm((int)RealmId);
            if (Rm != null)
                From.Rm = Rm;

            if (From.Rm == null)
            {
                Log.Error("WorldSelectRequest", "Realm null : " + From.GetIp);
                From.Disconnect();
                return;
            }

            LobbyWorldSelectResponse Rp = new LobbyWorldSelectResponse();
            From.SendSerialized(Rp);
        }
    }
}
