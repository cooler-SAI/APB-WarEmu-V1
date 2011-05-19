using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_DELETE_CHARACTER, "onDeleteCharacter")]
    public class F_DELETE_CHARACTER : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            byte Slot = packet.GetUint8();

            if (cclient._Account == null)
            {
                cclient.Disconnect();
                return;
            }

            CharMgr.RemoveCharacter(Slot, cclient._Account.AccountId);

            PacketOut Out = new PacketOut((byte)Opcodes.F_SEND_CHARACTER_RESPONSE);
            Out.WriteString(cclient._Account.Username, 24);
            cclient.SendTCP(Out);
        }
    }
}
