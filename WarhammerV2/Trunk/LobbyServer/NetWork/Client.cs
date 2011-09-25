/*
 * Copyright (C) 2011 APS
 *	http://AllPrivateServer.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;

using FrameWork;

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

        protected override void OnReceive(byte[] Packet)
        {
            lock (this)
            {
                PacketIn packet = new PacketIn(Packet, 0, Packet.Length);
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
