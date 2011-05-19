using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_INIT_PLAYER, "onInitPlayer")]
    public class F_INIT_PLAYER : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            Log.Succes("F_INIT_PLAYER", "Lancement !");

            Player Plr = cclient.Plr;

            if (!Plr.IsInWorld()) // Si le joueur n'est pas sur une map, alors on l'ajoute a la map
            {
                UInt16 ZoneId = Plr._Info.Value[0].ZoneId;
                ushort RegionId = (ushort)Plr._Info.Value[0].RegionId;
                RegionMgr Region = WorldMgr.GetRegion(RegionId, true);
                Region.AddObject(Plr, ZoneId);
            }
            else
                Plr._Loaded = false;
        }
    }
}
