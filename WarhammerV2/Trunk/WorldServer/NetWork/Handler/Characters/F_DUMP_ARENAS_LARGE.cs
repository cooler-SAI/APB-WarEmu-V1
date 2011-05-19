using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_DUMP_ARENAS_LARGE, "onDumpArenasLarge")]
    public class F_DUMP_ARENAS_LARGE : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            if (!cclient.HasAccount())
            {
                cclient.Disconnect();
                return;
            }

            byte CharacterSlot = packet.GetUint8();
            Character Char = CharMgr.GetAccountChar(cclient._Account.AccountId).GetCharacterBySlot(CharacterSlot);

            if (Char == null)
            {
                Log.Error("F_DUMP_ARENAS_LARGE", "Erreur , Character Introuvable au slot : " + CharacterSlot);
                cclient.Disconnect();
                return;
            }

            if (cclient.Plr == null)
                cclient.Plr = Player.CreatePlayer(cclient, Char);

            PacketOut Out = new PacketOut((byte)Opcodes.F_WORLD_ENTER);
            Out.WriteUInt16(0x0608); // TODO
            Out.Fill(0, 20);
            Out.WriteString("38699",5);
            Out.WriteString("38700",5);
            Out.WriteString("0.0.0.0",20);
            cclient.SendTCP(Out);
        }
    }
}
