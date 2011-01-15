using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Logger;
using FrameWork.NetWork;

namespace LobbyServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.ASK_LOGIN, "onAskLogin")]
    public class ASK_LOGIN : IPacketHandler
    {
        public int HandlePacket(BaseClient client, PacketIn packet)
        {
            LobbyClient cclient = client as LobbyClient;
            packet.Skip(24);
            string Email = packet.GetParsedString().ToUpper();

            Log.Notice("ASK_LOGIN", "Authentification de : " + Email);

            SendLoginResult(cclient, Email,Program.CharMgr.LoadAccount(Email));
            return 0;
        }

        static public void SendLoginResult(LobbyClient client,string Email,bool result)
        {
            if(!result)
            {
                Log.Error("ASK_LOGN","Compte introuvable !");
                ANS_LOGIN_FAILED.Send(client, 10010);
                client.Disconnect();
            }
            else
            {
                Log.Succes("ASK_LOGN","Authentification en cours.");
                client.Account = Program.CharMgr.GetAccount(Email);
                if (client.Account == null)
                    SendLoginResult(client, Email, false);
                else
                    LOGIN_SALT.Send(client);
            }
        }
    }
}
