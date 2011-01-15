using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Logger;
using FrameWork.NetWork;

using Common;

namespace WorldServer
{
    public class WorldClient : BaseClient
    {
        public DBAccount Account = null;
        public CharacterInfo Character = null;

        #region Base

        public WorldClient(TCPManager srv)
            : base(srv)
        {
        }

        public override void OnConnect()
        {

        }

        public override void OnDisconnect()
        {

        }

        #endregion

        #region TCP

        protected override void OnReceive(PacketIn packet)
        {
            lock (this)
            {
                packet.Size = packet.GetUint32Reversed();
                packet = DeCrypt(packet);
                packet.Opcode = packet.GetUint32Reversed();

                Server.HandlePacket(this, packet);
            }
        }

        #endregion
    }
}
