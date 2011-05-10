using System;
using System.Collections.Generic;
using System.Text;
using System.Timers;
using System.Reflection;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Services;
using System.Runtime.Remoting.Channels.Tcp;
using System.Timers;
using System.Diagnostics;

namespace FrameWork
{
    public class RpcClient
    {
        public TcpClientChannel Channel;
        public TcpServerChannel ServerChannel;
        public ServerMgr Mgr;
        public Timer Pinger;

        public RpcClientInfo Info;
        public List<Type>[] RegisteredTypes;

        public string ServerName;
        public string ServerIp;

        public string RpcServerIp;
        public int RpcServerPort;
        public int AllowedID;

        public RpcClient(string Name, string Ip, int AllowedID)
        {
            this.ServerName = Name;
            this.ServerIp = Ip;
            this.AllowedID = AllowedID;
        }

        public bool Start(string Ip, int Port)
        {
            return Connect(Ip, Port);
        }

        public bool Connect()
        {
            try
            {
                if (Channel != null)
                    ChannelServices.UnregisterChannel(Channel);

                if (ServerChannel != null)
                    ChannelServices.UnregisterChannel(ServerChannel);

                Channel = null;
                ServerChannel = null;
            }
            catch (Exception e)
            {

            }

            return Connect(RpcServerIp, RpcServerPort);
        }
        public bool Connect(string Ip, int Port)
        {
            Log.Debug("RpcClient", "Connect : " + Ip + ":" + Port);

            RpcServerIp = Ip;
            RpcServerPort = Port;

            try
            {
                if (Channel != null)
                    return false;

                Log.Debug("RpcClient", "Connecting to : " + Ip);

                Channel = new TcpClientChannel(ServerName, null);
                ChannelServices.RegisterChannel(Channel, false);

                Mgr = Activator.GetObject(typeof(ServerMgr), "tcp://" + Ip + ":" + Port + "/" + typeof(ServerMgr).Name) as ServerMgr;
                Info = Mgr.Connect(ServerName, ServerIp);

                if (Info == null)
                    return false;

                Log.Debug("RpcClient", "Listening on : " + ServerIp + ":" + Info.Port);

                ServerChannel = new TcpServerChannel("Server" + Info.RpcID, Info.Port);
                ChannelServices.RegisterChannel(ServerChannel, false);

                RegisteredTypes = RpcObject.RegisterHandlers(false, AllowedID);
                    foreach (Type t in RegisteredTypes[1])
                        RpcServer.GetObject(t, Info.Ip, Info.Port).MyInfo = Info;

                Mgr.Connected(Info.RpcID);

                Log.Success("RpcClient", "Client/Server started : " + Ip + ":" + Port);

                if (Pinger != null)
                    Pinger.Close();

                Pinger = new Timer();
                Pinger.Interval = 100;
                Pinger.Elapsed += CheckPing;
                Pinger.Start();

            }
            catch (Exception e)
            {
                Log.Error("RpcClient", e.ToString());
                Log.Notice("RpcClient", "Can not start RPC : " + Ip + ":" + Port);

                Mgr = null;
                Info = null;

                return false;
            }

            return true;
        }

        public void CheckPing(object Sender, EventArgs Args)
        {
            Pinger.Enabled = false;

            try
            {
                if (Mgr != null)
                    Mgr.Ping();
                else
                    Connect();
            }
            catch (Exception e)
            {
                GetLocalObject<ClientMgr>().OnServerDisconnected();

                Connect();
            }
            Pinger.Enabled = true;
        }

        public T GetServerObject<T>() where T : RpcObject
        {
            return Activator.GetObject(typeof(T), "tcp://" + RpcServerIp + ":" + RpcServerPort + "/" + typeof(T).Name) as T;
        }

        public T GetLocalObject<T>() where T : RpcObject
        {
            return Activator.GetObject(typeof(T), "tcp://" + Info.Ip + ":" + Info.Port + "/" + typeof(T).Name) as T;
        }

        public T GetClientObject<T>(string Name) where T : RpcObject
        {
            RpcClientInfo Info = GetServerObject<ServerMgr>().GetClient(Name);
            if (Info == null)
                return GetLocalObject<T>();
            else
                return RpcServer.GetObject<T>(Info);
        }

        public T GetClientObject<T>(int RpcID) where T : RpcObject
        {
            RpcClientInfo Info = GetServerObject<ServerMgr>().GetClient(RpcID);
            if (Info == null)
                return GetLocalObject<T>();
            else
                return RpcServer.GetObject<T>(Info);
        }
    }
}
