using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_PLAYER_ENTER_FULL, "onPlayerEnterFull")]
    public class F_PLAYER_ENTER_FULL : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            UInt16 SID;
            byte unk1,serverID,characterSlot;

            SID = packet.GetUint16();
            unk1 = packet.GetUint8();
            serverID = packet.GetUint8();
            string CharName = packet.GetString(24);
            packet.Skip(2);
            string Language = packet.GetString(2);
            packet.Skip(4);
            characterSlot = packet.GetUint8();

            Log.Succes("F_PLAYER_ENTER_FULL", "Entrer en jeu de : " + CharName + ",Slot=" + characterSlot);

            if (Program.Rm.RealmId != serverID)
                cclient.Disconnect();
            else
            {
                PacketOut Out = new PacketOut((byte)Opcodes.S_PID_ASSIGN);
                Out.WriteUInt16R((ushort)cclient.Id);
                cclient.SendTCP(Out);
            }
        }
    }
}
