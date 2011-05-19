using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Logger;
using FrameWork.NetWork;
using FrameWork.Database;

namespace LauncherServer
{
    public class Client : BaseClient
    {
        public Client(TCPManager srv)
            : base(srv)
        {

        }

        public override void OnConnect()
        {
            Log.Debug("Client", "Connexion " + GetIp);
        }

        public override void OnDisconnect()
        {
            Log.Debug("Client", "Deconnexion " + GetIp);
        }

        protected override void OnReceive(PacketIn pack)
        {
            lock (this)
            {
                pack.Size = pack.GetUint32();
                pack.Opcode = pack.GetUint8();

                if (!Enum.IsDefined(typeof(Opcodes), (byte)pack.Opcode))
                {
                    Log.Error("OnReceive", "Opcode invalide : " + pack.Opcode);
                    return;
                }

                Server.HandlePacket((BaseClient)this, pack);
            }
        }
    }
}
