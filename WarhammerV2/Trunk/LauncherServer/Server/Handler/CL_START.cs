using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;

using FrameWork.Logger;
using FrameWork.NetWork;

namespace LauncherServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CL_START, "onStart")]
    public class CL_START : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            Client cclient = client as Client;

            string Username = packet.GetString();
            UInt32 Len = packet.GetUint32();
            byte[] Password = packet.Read((int)Len);

            bool result = Program.AcctMgr.CheckAccount(Username, Password);
            Log.Debug("CL_START", "Lancement du client : " + Username + " " + result);

            PacketOut Out = new PacketOut((byte)Opcodes.LCR_START);

            if (result == true)
            {
                Out.WriteByte(0);
                Out.WriteString(Program.AcctMgr.GenerateToken(Username));
            }
            else 
                Out.WriteByte(1);

            cclient.SendTCP(Out);
        }
    }
}
