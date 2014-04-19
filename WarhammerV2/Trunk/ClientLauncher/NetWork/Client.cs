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
using System.Net.Sockets;
using System.IO;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;
using nsHashDictionary;
using MYPHandler;
using WarEmu;

namespace Launcher
{
    static public class Client
    {
        static public int Version = 1;
        static public string IP = "127.0.0.1";
        static public int Port = 8000;
        static public bool Started = false;

        static public string User;
        static public string Auth;
        static public string Language = "";

        // TCP
        static public Socket _Socket;

        static public bool Connect()
        {
            try
            {
                //IP = "127.0.0.1";

                if (_Socket != null)
                    _Socket.Close();

                _Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                _Socket.Connect(IP, Port);

                BeginReceive();

                SendCheck();
            }
            catch (Exception e)
            {
                MessageBox.Show("Can not connect to server.\n" + e.Message, "Server offline");
                return false;
            }

            return true;
        }

        static public void Close()
        {
            try
            {
                if (_Socket != null)
                    _Socket.Close();
              
            }
            catch(Exception)
            {

            }
        }

        static public void UpdateLanguage()
        {
            /*
            if (Language.Length <= 0)
                return;

            int LangueId = 1;
            switch (Language)
            {
                case "French":
                    LangueId = 2;
                    break;
                case "English":
                    LangueId = 1;
                    break;
                case "Deutch":
                    LangueId = 3;
                    break;
                case "Italian":
                    LangueId = 4; 
                    break;
                case "Spanish":
                    LangueId = 5;
                    break;
                case "Korean":
                    LangueId = 6;
                    break;
                case "Chinese":
                    LangueId = 7;
                    break;
                case "Japanese":
                    LangueId = 9;
                    break;
                case "Russian":
                    LangueId = 10;
                    break;
            };

            string CurDir = Directory.GetCurrentDirectory();

            try
            {
                Directory.SetCurrentDirectory(CurDir + "\\..\\user\\");

                StreamReader Reader = new StreamReader("UserSettings.xml");
                string line = "";
                string TotalStream = "";
                while ( (line = Reader.ReadLine()) != null)
                {
                    Print(line);
                    int Pos = line.IndexOf("Language id=");
                    if (Pos > 0)
                    {
                        Pos = line.IndexOf("\"")+1;
                        int Pos2 = line.LastIndexOf("\"");
                        line = line.Remove(Pos, Pos2-Pos);
                        line = line.Insert(Pos, "" + LangueId);
                    }

                    TotalStream += line +"\n";
                }
                Reader.Close();

                StreamWriter Writer = new StreamWriter("UserSettings.xml", false);
                Writer.Write(TotalStream);
                Writer.Flush();
                Writer.Close();
            }
            catch (Exception e)
            {
                Print("Ecriture : " + e.ToString());
            }*/
        }

        static public void UpdateRealms()
        {
            PacketOut Out = new PacketOut((byte)Opcodes.CL_INFO);
            SendTCP(Out);
        }

        #region Sender

        // Buffer en train d'être envoyé
        static byte[] m_tcpSendBuffer = new byte[65000];

        // Liste des packets a sender
        static readonly Queue<byte[]> m_tcpQueue = new Queue<byte[]>(256);

        // True si un send est en cours
        static bool m_sendingTcp = false;

        // Envoi un packet
        static public void SendTCP(PacketOut packet)
        {
            //Fix the packet size
            packet.WritePacketLength();

            //Get the packet buffer
            byte[] buf = packet.GetBuffer(); //packet.WritePacketLength sets the Capacity

            //Send the buffer
            SendTCP(buf);
        }

        static public void SendTCP(byte[] buf)
        {
            if (m_tcpSendBuffer == null)
                return;

            //Check if client is connected
            if (_Socket.Connected)
            {

                try
                {
                    lock (m_tcpQueue)
                    {
                        if (m_sendingTcp)
                        {
                            m_tcpQueue.Enqueue(buf);
                            return;
                        }

                        m_sendingTcp = true;
                    }

                    Buffer.BlockCopy(buf, 0, m_tcpSendBuffer, 0, buf.Length);

                    _Socket.BeginSend(m_tcpSendBuffer, 0, buf.Length, SocketFlags.None, m_asyncTcpCallback, null);
                }
                catch
                {
                    Close();
                }
            }
        }

        static readonly AsyncCallback m_asyncTcpCallback = AsyncTcpSendCallback;

        static void AsyncTcpSendCallback(IAsyncResult ar)
        {
            try
            {
                Queue<byte[]> q = m_tcpQueue;

                int sent = _Socket.EndSend(ar);

                int count = 0;
                byte[] data = m_tcpSendBuffer;

                if (data == null)
                    return;

                lock (q)
                {
                    if (q.Count > 0)
                    {
                        //						Log.WarnFormat("async sent {0} bytes, sending queued packets count: {1}", sent, q.Count);
                        count = CombinePackets(data, q, data.Length);
                    }
                    if (count <= 0)
                    {
                        //						Log.WarnFormat("async sent {0} bytes", sent);
                        m_sendingTcp = false;
                        return;
                    }
                }

                _Socket.BeginSend(data, 0, count, SocketFlags.None, m_asyncTcpCallback, null);

            }
            catch (Exception)
            {
                    Close();
            }
        }

        private static int CombinePackets(byte[] buf, Queue<byte[]> q, int length)
        {
            int i = 0;
            do
            {
                var pak = q.Peek();
                if (i + pak.Length > buf.Length)
                {
                    if (i == 0)
                    {
                        q.Dequeue();
                        continue;
                    }
                    break;
                }

                Buffer.BlockCopy(pak, 0, buf, i, pak.Length);
                i += pak.Length;

                q.Dequeue();
            } while (q.Count > 0);

            return i;
        }

        static public void SendTCPRaw(PacketOut packet)
        {
            SendTCP((byte[])packet.GetBuffer().Clone());
        }

        #endregion

        #region Receiver

        static private readonly AsyncCallback ReceiveCallback = OnReceiveHandler;
        static byte[] _pBuf = new byte[2048];


        private static void OnReceiveHandler(IAsyncResult ar)
        {
            try
            {
                int numBytes = _Socket.EndReceive(ar);

                if (numBytes > 0)
                {
                    byte[] buffer = _pBuf;
                    int bufferSize = numBytes;

                    PacketIn pack = new PacketIn(buffer, 0, bufferSize);
                    OnReceive(pack);
                    BeginReceive();

                }
                else
                {
                    Close();
                }
            }
            catch
            {

            }
        }

        static public void BeginReceive()
        {
            if (_Socket != null && _Socket.Connected)
            {
                int bufSize = _pBuf.Length;

                if (0 >= bufSize) //Do we have space to receive?
                {
                    Close();
                }
                else
                {
                    _Socket.BeginReceive(_pBuf, 0, bufSize, SocketFlags.None, ReceiveCallback, null);
                }
            }
        }

        #endregion

        static public void OnReceive(PacketIn packet)
        {
            lock (packet)
            {
                packet.Size = packet.GetUint32();
                packet.Opcode = packet.GetUint8();

                Handle(packet);
            }
        }

        #region Packet

        static public void Handle(PacketIn packet)
        {
            if(!Enum.IsDefined(typeof(Opcodes),(byte)packet.Opcode))
            {
                MessageBox.Show("Invalid opcode : " + packet.Opcode.ToString("X02"));
                return;
            }

            switch((Opcodes)packet.Opcode)
            {
                case Opcodes.LCR_CHECK:
                    
                    byte Result = packet.GetUint8();

                    switch((CheckResult)Result)
                    {
                        case CheckResult.LAUNCHER_VERSION:
                            string Message = packet.GetString();
                            MessageBox.Show(Message);
                            Close();
                            break;
                        case CheckResult.LAUNCHER_FILE:
                            {
                                string File = packet.GetString();
                                byte[] Bt = ASCIIEncoding.ASCII.GetBytes(File);

                                FileInfo Info = new FileInfo("PortalSettings.xml");

                                try
                                {
                                    using (FileStream Str = Info.Create())
                                    {
                                        Str.Write(Bt, 0, Bt.Length);
                                        Str.Flush();
                                        Str.Position = 0;

                                        Form1.Instance.updateMYP(Str);
                                    }

                                   /* {
                                        Info = new FileInfo("D94BB637_5181D982E63A3766.dds");

                                        using (FileStream Str = Info.OpenRead())
                                        {
                                            Form1.Instance.updateWarhammerName(Str);
                                        }
                                    }*/
                                }
                                catch (Exception e)
                                {
                                    MessageBox.Show(e.ToString());
                                    return;
                                }

                                Start();
                            } break;
                    }
                    break;

                case Opcodes.LCR_START:

                    byte Res = packet.GetUint8();

                    if (Res >= 1)
                    {
                        MessageBox.Show("Incorrect Username or password. Please try again. Be carefull, they are case sensitive.\n\rVisit http://www.warhammer-server.com/forum/  for more information.");
                        return;
                    }
                    else
                    {
                        Auth = packet.GetString();

                        try
                        {
                            Form1.Instance.StartClient(Auth);
                        }
                        catch (Exception e)
                        {
                            MessageBox.Show(e.ToString());
                        }
                    }

                    break;

                case Opcodes.LCR_INFO:
                    {
                        //Accueil.Acc.ClearRealms();
                        byte RealmsCount = packet.GetUint8();
                        for (byte i = 0; i < RealmsCount; ++i)
                        {
                            bool Online = packet.GetUint8() > 0;
                            string Name = packet.GetString();
                            UInt32 OnlinePlayers = packet.GetUint32();
                            UInt32 OrderCount = packet.GetUint32();
                            UInt32 DestructionCount = packet.GetUint32();

                            //Accueil.Acc.AddRealm(Name, Online, OnlinePlayers, DestructionCount, DestructionCount);

                        }
                    }break;
            }
        }

        static public void Start()
        {
            PacketOut Out = new PacketOut((byte)Opcodes.CL_START);
            Out.WriteString(User);
            Out.WriteString(Auth);
            Client.SendTCP(Out);
        }

        static public void SendCheck()
        {
            PacketOut Out = new PacketOut((byte)Opcodes.CL_CHECK);
            Out.WriteUInt32((uint)Version);
            SendTCP(Out);
        }

        #endregion
    }
}
