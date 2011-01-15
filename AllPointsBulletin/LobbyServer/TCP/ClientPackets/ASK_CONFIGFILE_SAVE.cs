using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Logger;
using FrameWork.NetWork;
using FrameWork.zlib;

using Common;

namespace LobbyServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.ASK_CONFIGFILE_SAVE, "onAskConfigfileSave")]
    public class ASK_CONFIGFILE_SAVE : IPacketHandler
    {
        public int HandlePacket(BaseClient client, PacketIn packet)
        {
            LobbyClient cclient = (LobbyClient)client;

            byte FileId = packet.GetUint8();
            UInt32 Version = packet.GetUint32Reversed();

            byte[] File = new byte[packet.Length - packet.Position];
            packet.Read(File, 0, File.Length);

            File = ZlibMgr.Decompress(File);

            Log.Debug("ConfigSave", "Config saved : fileid=" + FileId + ", Version=" + Version + ",Size=" + File.Length);

            Program.FileMgr.SaveInfo(cclient.Account.Id, FileId, File);

            PacketOut Out = new PacketOut((UInt32)Opcodes.ANS_CONFIGFILE_SAVE);
            Out.WriteUInt32Reverse(0);
            Out.WriteByte(FileId);
            Out.Write(File, 0, File.Length);
            cclient.SendTCP(Out);

            return 0;
        }
    }
}
