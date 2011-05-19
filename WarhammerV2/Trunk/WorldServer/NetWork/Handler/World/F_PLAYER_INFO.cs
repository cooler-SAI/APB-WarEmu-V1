using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_PLAYER_INFO, "onPlayerInfo")]
    public class F_PLAYER_INFO : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            if (cclient.Plr == null)
                return;

            if (!cclient.Plr.IsInWorld())
                return;

            packet.Skip(2);
            UInt16 Oid = packet.GetUint16();
            packet.Skip(1);
            byte Faction = packet.GetUint8();
            Log.Succes("F_PLAYER_INFO", "Target de =" + Oid);
            cclient.Plr.CbtInterface.SetTarget(Oid, (GameData.TargetTypes)Faction);
        }
    }
}
