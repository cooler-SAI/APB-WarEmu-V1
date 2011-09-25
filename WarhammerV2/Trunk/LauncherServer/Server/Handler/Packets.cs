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

using Common;
using FrameWork;

namespace LauncherServer
{
    public class LauncherPackets : IPacketHandler
    {
        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CL_START, 0, "OnStart")]
        static public void CL_START(BaseClient client, PacketIn packet)
        {
            Client cclient = client as Client;

            string Username = packet.GetString();
            UInt32 Len = packet.GetUint32();
            byte[] Password = packet.Read((int)Len);

            bool result = Program.AcctMgr.CheckAccount(Username, Password);
            Log.Debug("CL_START", "Lancement du client : " + Username + " " + result);

            PacketOut Out = new PacketOut((byte)Opcodes.LCR_START);

            if (result == true)
            {
                Out.WriteByte(0);
                Out.WriteString(Program.AcctMgr.GenerateToken(Username));
            }
            else 
                Out.WriteByte(1);

            cclient.SendTCP(Out);
        }

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CL_CHECK, 0, "OnCheck")]
        static public void CL_CHECK(BaseClient client, PacketIn packet)
        {
            Client cclient = client as Client;
            uint Version = packet.GetUint32();

            Log.Debug("CL_CHECK", "Launcher Version : " + Version);

            PacketOut Out = new PacketOut((byte)Opcodes.LCR_CHECK);

            if (Version != Program.Version)
            {
                Out.WriteByte((byte)CheckResult.LAUNCHER_VERSION); // Version incorrect + message
                Out.WriteString(Program.Message);
                client.SendTCP(Out);

                cclient.Disconnect();
                return;
            }

            byte File = packet.GetUint8();
            UInt64 Len = 0;

            if (File >= 1)
                Len = packet.GetUint64();

            if ((long)Len != Program.Info.Length)
            {
                Out.WriteByte((byte)CheckResult.LAUNCHER_FILE);
                Out.WriteString(Program.StrInfo);
            }
            else
                Out.WriteByte((byte)CheckResult.LAUNCHER_OK);

            cclient.SendTCP(Out);
        }

        [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.CL_INFO, 0, "OnInfo")]
        static public void CL_INFO(BaseClient client, PacketIn packet)
        {
            Client cclient = client as Client;

            if (cclient.LastInfoRequest == 0 || cclient.LastInfoRequest <= TCPServer.GetTimeStampMS()+10000)
            {
                cclient.LastInfoRequest = TCPServer.GetTimeStampMS();

                List<Realm> Rms = Program.AcctMgr.GetRealms();

                PacketOut Out = new PacketOut((byte)Opcodes.LCR_INFO);
                Out.WriteByte((byte)Rms.Count);
                foreach (Realm Rm in Rms)
                {
                    Out.WriteByte(Convert.ToByte(Rm.Info != null));
                    Out.WriteString(Rm.Name);
                    Out.WriteUInt32(Rm.OnlinePlayers);
                    Out.WriteUInt32(Rm.OrderCount);
                    Out.WriteUInt32(Rm.DestructionCount);
                }

                cclient.SendTCP(Out);
            }
        }
    }
}
