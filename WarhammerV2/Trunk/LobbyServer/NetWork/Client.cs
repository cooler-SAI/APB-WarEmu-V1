/*
 * Copyright (C) 2013 APS
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
using System.Net;
using System.Net.Sockets;
using Google.ProtocolBuffers;
using WarProtocol;
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
                Log.Tcp("Received", Packet, 0, Packet.Length);
                PacketIn packet = new PacketIn(Packet, 0, Packet.Length);
                long byteLeft = packet.Length;
                long StartPos, EndPos, Pos, Diff;

                while (byteLeft > 0)
                {
                    if (!m_expectData)
                    {
                        StartPos = packet.Position;
                        m_expectSize = packet.DecodeMythicSize();
                        EndPos = packet.Position;

                        Diff = EndPos - StartPos;
                        byteLeft -= Diff;
                        if (m_expectSize <= 0)
                        {
                            packet.Opcode = packet.GetUint8();
                            packet.Size = (ulong)m_expectSize;
                            //HandlePacket(packet);
                            _srvr.HandlePacket(this, packet);
                            return;
                        }

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
                            Pos = packet.Position;

                            packet.Opcode = Opcode;
                            packet.Size = (ulong)m_expectSize;

                            //HandlePacket(packet);
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


        public static ClusterProp setProp(string name, string value)
        {
            return ClusterProp.CreateBuilder().SetPropName(name)
                                              .SetPropValue(value)
                                              .Build();
        }
        public static ClusterInfo.Builder BuildCluster()
        {
            ClusterInfo.Builder cluster = ClusterInfo.CreateBuilder();
            cluster.SetClusterId(122)
                   .SetClusterName("Cluster 1")
                   .SetLobbyHost("127.0.0.1")
                   .SetLobbyPort(10622)
                   .SetLanguageId(0)
                   .SetMaxClusterPop(500)
                   .SetClusterPopStatus(ClusterPopStatus.POP_HIGH)
                   .SetLanguageId(0)
                   .SetClusterStatus(ClusterStatus.STATUS_ONLINE);

            cluster.AddServerList(
                ServerInfo.CreateBuilder().SetServerId(122)
                                          .SetServerName("Emulator")
                                          .Build());

            cluster.AddPropertyList(setProp("setting.allow_trials", "1"));
            cluster.AddPropertyList(setProp("setting.charxferavailable", "0"));
            cluster.AddPropertyList(setProp("setting.language", "EN"));
            cluster.AddPropertyList(setProp("setting.legacy", "0"));
            cluster.AddPropertyList(setProp("setting.manualbonus.realm.destruction", "100"));
            cluster.AddPropertyList(setProp("setting.manualbonus.realm.order", "100"));
            cluster.AddPropertyList(setProp("setting.min_cross_realm_account_level", "0"));
            cluster.AddPropertyList(setProp("setting.name", "Emulator"));
            cluster.AddPropertyList(setProp("setting.net.address", "127.0.0.1"));
            cluster.AddPropertyList(setProp("setting.net.port", "10622"));
            cluster.AddPropertyList(setProp("setting.redirect", "0"));
            cluster.AddPropertyList(setProp("setting.region", "STR_REGION_NORTHAMERICA"));
            cluster.AddPropertyList(setProp("setting.retired", "0"));
            cluster.AddPropertyList(setProp("status.queue.Destruction.waiting", "0"));
            cluster.AddPropertyList(setProp("status.queue.Order.waiting", "0"));
            cluster.AddPropertyList(setProp("status.realm.destruction.density", "1"));
            cluster.AddPropertyList(setProp("status.realm.order.density", "1"));
            cluster.AddPropertyList(setProp("status.servertype.openrvr", "0"));
            cluster.AddPropertyList(setProp("status.servertype.rp", "0"));
            cluster.AddPropertyList(setProp("status.status", "0"));

            cluster.Build();
            return cluster;
        }
    }
}
