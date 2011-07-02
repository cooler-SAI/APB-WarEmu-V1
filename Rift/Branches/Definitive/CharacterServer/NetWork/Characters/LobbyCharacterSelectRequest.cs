using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace CharacterServer
{
    [ISerializableAttribute((long)Opcodes.LobbyCharacterSelectResponse)]
    public class LobbyCharacterSelectResponse : ISerializablePacket
    {
        [ListBit(1)]
        public List<string> Ips = new List<string>();
    }

    [ISerializableAttribute((long)Opcodes.LobbyCharacterSelectRequest)]
    public class LobbyCharacterSelectRequest : ISerializablePacket
    {
        [Unsigned7Bit(0)]
        public long GUID;

        public override void OnRead(RiftClient From)
        {
            Log.Success("SelectRequest", "Enter on World : " + From.GetIp + ",GUID=" + GUID);

            if (From.Acct == null || From.Rm == null)
                return;

            Character Char = From.Rm.GetObject<CharactersMgr>().GetCharacter(GUID);
            if (Char == null || Char.AccountId != From.Acct.Id)
            {
                Log.Error("SelectRequest", "Invalid CharacterId = " + GUID);
                From.Disconnect();
                return;
            }


            MapServerInfo Info = From.Rm.GetObject<WorldMgr>().GetMapInfo();

            if (Info == null)
            {
                Log.Error("SelectRequest", "No map loaded ! Run MapServer");
                return;
            }

            Log.Success("SelectRequest", "Entering on Map : " + Info.MapAdress);

            Info.GetObject<MapMgr>().RegisterConnecting(From.Acct.Username,Char.CharacterId);

            LobbyCharacterSelectResponse Rp = new LobbyCharacterSelectResponse();
            Rp.Ips.Add(Info.MapAdress);
            From.SendSerialized(Rp);

        }
    }
}
