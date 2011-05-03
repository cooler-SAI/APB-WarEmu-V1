using System;
using System.Collections.Generic;
using System.Text;
using System.Timers;
using System.Reflection;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Services;
using System.Runtime.Remoting.Channels.Tcp;

namespace FrameWork
{
    public class RpcServer
    {
        public TcpServerChannel Channel;
        public ServerMgr Mgr;
        public Timer Pinger;
        
        // 0 = Client Class
        // 1 = Server Class
        public List<Type>[] Registered;

        public int StartingPort;
        public string LocalIp;
        public int LocalPort;
        public int AllowedID;

        public RpcServer(int StartingPort, int AllowedID)
        {
            this.StartingPort = StartingPort;
            this.AllowedID = AllowedID;
        }

        public bool Start(string Ip, int Port)
        {
            try
            {
                if (Channel != null)
                    return false;

                LocalIp = Ip;
                LocalPort = Port;

                Log.Debug("RpcServer", "Start on : " + Ip + ":" + Port);

                Channel = new TcpServerChannel("Server" + Port, Port);
                ChannelServices.RegisterChannel(Channel, false);
                Registered = RpcObject.RegisterHandlers(true, AllowedID);

                ServerMgr.Server = this;
                Mgr = GetLocalObject<ServerMgr>();
                Mgr.StartingPort = StartingPort;

                Log.Success("RpcServer", "Listening on : " + Ip + ":" + Port);

                Pinger = new Timer(200);
                Pinger.Elapsed += Ping;
                Pinger.Enabled = true;
            }
            catch (Exception e)
            {
                Log.Error("RpcServer", e.Message);
                Log.Notice("RpcServer", "Can not start RPC : " + Ip + ":" + Port);

                return false;
            }

            return true;
        }

        public bool IsPinging = false;
        public void Ping(object sender, EventArgs Args)
        {
            Pinger.Enabled = false;

            List<ClientInfo> Disconnected = new List<ClientInfo>();

            foreach (ClientInfo Info in Mgr.GetClients())
            {
                if (!Info.Connected)
                    continue;

                try
                {
                    GetObject<ClientMgr>(Info).Ping();
                }
                catch (Exception e)
                {
                    Log.Error("RpcServer", e.ToString());
                    Log.Notice("RpcServer", Info.Description() + " | Disconnected");

                    Disconnected.Add(Info);
                    Mgr.Remove(Info.RpcID);
                }
            }

            if (Disconnected.Count > 0)
            {
                foreach (ClientInfo Info in Mgr.GetClients())
                {
                    try
                    {
                        foreach (ClientInfo ToDisconnect in Disconnected)
                        {
                            foreach (Type type in Registered[1])
                            {
                                RpcServer.GetObject(type, Info.Ip, Info.Port).OnClientDisconnected(ToDisconnect);
                            }
                        }
                    }
                    catch (Exception e)
                    {
                        Log.Error("RpcServer", e.ToString());
                    }
                }
            }

            Pinger.Enabled = true;
        }


        public T GetObject<T>(string Name) where T : RpcObject
        {
            ClientInfo Info = Mgr.GetClient(Name);
            if (Info == null)
            {
                Log.Error("RpcServer", "Can not find client : " + Name);
                return null;
            }

            return GetObject<T>(Info);
        }



        public T GetLocalObject<T>() where T : RpcObject
        {
            return RpcServer.GetObject(typeof(T),LocalIp,LocalPort) as T;
        }

        static public T GetObject<T>(ClientInfo Info) where T : RpcObject
        {
            return GetObject(typeof(T), Info.Ip, Info.Port) as T;
        }

        static public RpcObject GetObject(Type type, string Ip, int Port)
        {
            return Activator.GetObject(type, "tcp://" + Ip + ":" + Port + "/" + type.Name) as RpcObject;
        }
    }
}
