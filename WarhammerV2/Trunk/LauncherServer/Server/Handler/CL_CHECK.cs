using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

using FrameWork.Logger;
using FrameWork.NetWork;

namespace LauncherServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CL_CHECK, "onCheck")]
    public class CL_CHECK : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            Client cclient = client as Client;
            uint Version = packet.GetUint32();

            Log.Debug("CL_CHECK", "Vérification du launcher : " + Version);

            PacketOut Out = new PacketOut((byte)Opcodes.LCR_CHECK);

            if (Version != Program.Version)
            {
                Out.WriteByte((byte)CheckResult.LAUNCHER_VERSION); // Version incorrect + message
                Out.WriteString(Program.Message);
                client.SendTCP(Out);

                cclient.Disconnect();
                return;
            }

            byte File = packet.GetUint8();
            UInt64 Len = 0;

            if (File >= 1)
                Len = packet.GetUint64();

            if ((long)Len != Program.Info.Length)
            {
                Out.WriteByte((byte)CheckResult.LAUNCHER_FILE);
                Out.WriteString(Program.StrInfo);
            }
            else
                Out.WriteByte((byte)CheckResult.LAUNCHER_OK);

            cclient.SendTCP(Out);
        }
    }
}
