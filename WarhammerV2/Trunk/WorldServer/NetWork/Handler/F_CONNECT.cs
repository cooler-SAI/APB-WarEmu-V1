using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_CONNECT, "onConnect")]
    public class F_CONNECT : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            packet.Skip(8);
            UInt32 Tag = packet.GetUint32();
            string Token = packet.GetString(80);
            packet.Skip(21);
            string Username = packet.GetString(23);

            Log.Debug("Connect", "Connexion avec : " + Token + ",User=" + Username);

            AuthResult Result = Program.AcctMgr.CheckToken(Username, Token);
            if (Result != AuthResult.AUTH_SUCCESS)
            {
                cclient.Disconnect();
            }
            else
            {
                cclient._Account = Program.AcctMgr.GetAccount(Username);
                if (cclient._Account == null)
                    cclient.Disconnect();
                else
                {
                    Log.Success("F_CONNECT", "MeId=" + cclient.Id);

                    GameClient Other = (cclient.Server as TCPServer).GetClientByAccount(cclient,cclient._Account.AccountId);
                    if (Other != null)
                        Other.Disconnect();

                    PacketOut Out = new PacketOut((byte)Opcodes.S_CONNECTED);
                    Out.WriteUInt32(0);
                    Out.WriteUInt32(Tag);
                    Out.WriteByte(Program.Rm.RealmId);
                    Out.WriteUInt32(1);
                    Out.WritePascalString(Username);
                    Out.WritePascalString(Program.Rm.Name);
                    Out.WriteByte(0);
                    Out.WriteUInt16(0);
                    cclient.SendTCP(Out);
                }
            }
        }
    }
}
