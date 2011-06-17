﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Logger;
using FrameWork.NetWork;

namespace LobbyServer
{
    public class TCPServer : TCPManager
    {
        public TCPServer()
            : base()
        {
            PacketOut.SizeLen = sizeof(UInt16);
            PacketOut.OpcodeInLen = false;
            PacketOut.SizeInLen = false;
            PacketOut.OpcodeReverse = false;
            PacketOut.SizeReverse = false;
            PacketOut.Struct = PackStruct.SizeAndOpcode;
            DisposePackets = false;
        }

        protected override BaseClient GetNewClient()
        {
            Client client = new Client(this);

            return client;
        }
    }
}