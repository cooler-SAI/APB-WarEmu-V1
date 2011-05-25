using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace CharacterServer
{
    [ISerializableAttribute((long)Opcodes.LobbyWorldListRequest)]
    public class LobbyWorldListRequest : ISerializablePacket
    {
        public override void OnRead(RiftClient From)
        {
            Log.Success("WorldList", "Request : In Progress");

            Realm[] Realms = Program.AcctMgr.GetRealms();

            LobbyWorldListResponse Rp = new LobbyWorldListResponse();
            foreach(Realm Rm in Realms)
            {
                LobbyWorldEntry Entry = new LobbyWorldEntry();
                Entry.RealmID = Rm.RiftId;
                Entry.PVP = Rm.PVP == 1;
                Entry.Recommended = Rm.Recommended == 1;
                Entry.Population = 0;
                Entry.RP = Rm.RP == 1;
                Entry.Version = Rm.ClientVersion;
                Entry.CharactersCount = 0;
                Entry.Language = Rm.Language;
                if (Rm.Online > 0)
                    Entry.AddField(16, EPacketFieldType.True, (bool)true);

                Rp.Realms.Add(Entry);
            }

            Log.Success("WorldList", "Count = " + Rp.Realms.Count);

            From.SendSerialized(Rp);
        }
    }
}
