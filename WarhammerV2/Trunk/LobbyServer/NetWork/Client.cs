using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;

using FrameWork.Logger;
using FrameWork.NetWork;

namespace LobbyServer
{
    public class Client : BaseClient
    {
        public string Username = "";
        public string Token = "";

        public Client(TCPManager srv)
            : base(srv)
        {
            RSACryptoServiceProvider Prov = new RSACryptoServiceProvider();
        }

        public override void OnConnect()
        {
            Log.Debug("Client", "Connexion " + GetIp);
        }

        public override void OnDisconnect()
        {
            Log.Debug("Client", "Deconnexion " + GetIp);
        }

        private ushort Opcode = 0;
        private int m_expectSize = 0;
        public bool m_expectData = false;

        protected override void OnReceive(PacketIn packet)
        {
            lock (this)
            {
                long byteLeft = packet.Length;

                while (byteLeft > 0)
                {
                    if (!m_expectData)
                    {
                        long StartPos = packet.Position;
                        m_expectSize = packet.DecodeMythicSize();
                        long EndPos = packet.Position;

                        long Diff = EndPos - StartPos;
                        byteLeft -= Diff;
                        if (m_expectSize <= 0)
                            return;

                        if (byteLeft <= 0)
                            return;

                        Opcode = packet.GetUint8();
                        byteLeft -= 1;

                        m_expectData = true;
                    }
                    else
                    {
                        m_expectData = false;
                        if (byteLeft >= m_expectSize)
                        {
                            long Pos = packet.Position;

                            packet.Opcode = Opcode;
                            packet.Size = (ulong)m_expectSize;

                            _srvr.HandlePacket(this, packet);

                            byteLeft -= m_expectSize;
                            packet.Position = Pos;
                            packet.Skip(m_expectSize);
                        }
                        else
                        {
                            Log.Error("OnReceive", "Data count incorrect :" + byteLeft + " != " + m_expectSize);
                        }
                    }
                }

                packet.Dispose();
            }
        }

        public void SendTCPCuted(PacketOut Out)
        {

            long PSize = Out.Length - Out.OpcodeLen - PacketOut.SizeLen; // Size = Size-len-opcode

            byte[] Packet = new byte[PSize];
            Out.Position = Out.OpcodeLen + PacketOut.SizeLen;
            Out.Read(Packet, 0, (int)(PSize));

            List<byte> Header = new List<byte>(5);
            int itemcount = 1;
            while (PSize > 0x7f)
            {
                Header.Add((byte)((byte)(PSize) | 0x80));
                PSize >>= 7;
                itemcount++;
                if (itemcount >= Header.Capacity + 10)
                    Header.Capacity += 10;
            }

            Header.Add((byte)(PSize));
            Header.Add((byte)(Out.Opcode));

            Log.Tcp("Header", Header.ToArray(), 0, Header.Count);
            Log.Tcp("Packet", Packet, 0, Packet.Length);

            SendTCP(Header.ToArray());
            SendTCP(Packet);

            Out.Dispose();
        }
    }
}
