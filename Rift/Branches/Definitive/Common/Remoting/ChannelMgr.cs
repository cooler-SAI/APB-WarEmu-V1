using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    public class Channel
    {
        public delegate void OnPlayerJoinOrLeave(Channel Chan, string PlayerName, int TickCount);
        public delegate void OnMessage(Channel Chan, string PlayerName, string Message, int TickCount);

        public Channel(string ChannelName, string ChannelPassword)
        {
            this.ChannelName = ChannelName;
            this.ChannelPassword = ChannelPassword;
        }
        
        public string ChannelName;
        public string ChannelPassword;

        #region Messages

        public List<OnMessage> OnMessages = new List<OnMessage>();
        public void AddMessageEvent(OnMessage MsgEvent)
        {
            OnMessages.Add(MsgEvent);
        }
        public void RemoveMessageEvent(OnMessage MsgEvent)
        {
            OnMessages.Add(MsgEvent);
        }
        public bool DispatchMessage(string PlayerName, string Message, bool CheckExist)
        {
            if(CheckExist && !HasPlayer(PlayerName))
                return false;

            foreach (OnMessage MsgEvent in OnMessages.ToArray())
                MsgEvent.Invoke(this, PlayerName, Message, Environment.TickCount);

            return true;
        }

        #endregion

        #region Players

        public List<string> Players = new List<string>();

        public int PlayerCount
        {
            get
            {
                return Players.ToArray().Length;
            }
        }

        public bool HasPlayer(string PlayerName)
        {
            return Players.Contains(PlayerName);
        }
        public bool CanJoin(string PlayerName, string Password)
        {
            if (Password != ChannelPassword)
                return false;

            return true;
        }
        public bool Join(string PlayerName, string Password , bool Check)
        {
            if (Check && !CanJoin(PlayerName, Password))
                return false;

            OnPlayerJoin(PlayerName);
            Players.Add(PlayerName);

            return true;
        }
        public bool Remove(string PlayerName)
        {
            Players.Remove(PlayerName);
            OnPlayerLeave(PlayerName);
            return true;
        }

        public List<OnPlayerJoinOrLeave> PlayerJoinEvents = new List<OnPlayerJoinOrLeave>();
        public List<OnPlayerJoinOrLeave> PlayerLeaveEvents = new List<OnPlayerJoinOrLeave>();

        public void AddPlayerJoinEvent(OnPlayerJoinOrLeave PlayerEvent)
        {
            PlayerJoinEvents.Add(PlayerEvent);
        }
        public void AddPlayerLeaveEvent(OnPlayerJoinOrLeave PlayerEvent)
        {
            PlayerLeaveEvents.Add(PlayerEvent);
        }
        public void OnPlayerJoin(string PlayerName)
        {
            foreach (OnPlayerJoinOrLeave Event in PlayerJoinEvents.ToArray())
                Event.Invoke(this, PlayerName, Environment.TickCount);
        }
        public void OnPlayerLeave(string PlayerName)
        {
            foreach (OnPlayerJoinOrLeave Event in PlayerLeaveEvents.ToArray())
                Event.Invoke(this, PlayerName, Environment.TickCount);
        }

        #endregion
    }

    [Rpc(true, System.Runtime.Remoting.WellKnownObjectMode.Singleton, 1)]
    public class ChannelMgr : RpcObject
    {
        public Dictionary<string, Channel> Channels = new Dictionary<string, Channel>();

        public bool HasChannel(string ChannelName)
        {
            return Channels.ContainsKey(ChannelName);
        }
        public Channel GetChannel(string ChannelName)
        {
            Channel Chan;
            Channels.TryGetValue(ChannelName, out Chan);
            return Chan;
        }

        public Channel CreateChannel(string ChannelName, string ChannelPassword)
        {
            if (HasChannel(ChannelName))
                return null;

            Channel Chan = new Channel(ChannelName, ChannelPassword);
            Channels.Add(ChannelName, Chan);

            return Chan;
        }
        public Channel RemoveChannel(string ChannelName)
        {
            Channel Chan = GetChannel(ChannelName);
            if (Chan == null)
                return null;

            Channels.Remove(ChannelName);

            return Chan;
        }

        public Channel JoinChannel(string ChannelName, string ChannelPassword, string PlayerName, bool Create)
        {
            Channel Chan = GetChannel(ChannelName);
            if (Chan == null && Create)
                Chan = CreateChannel(ChannelName, ChannelPassword);

            if (Chan == null)
                return null;

            if (Chan.Join(PlayerName, ChannelPassword, true))
                return Chan;
            else
                return null;
        }
        public List<Channel> JoinChannels(string PlayerName, bool CheckPassword)
        {
            List<Channel> Chans = new List<Channel>();

            foreach (Channel Chan in Channels.Values.ToArray())
            {
                if (Chan.Join(PlayerName, "", CheckPassword))
                    Chans.Add(Chan);
            }

            return Chans;
        }

        public Channel LeaveChannel(string ChannelName, string PlayerName)
        {
            Channel Chan = GetChannel(ChannelName);
            if (Chan == null)
                return null;

            Chan.Remove(PlayerName);
            return Chan;
        }
        public void LeaveChannels(string PlayerName)
        {
            foreach (Channel Chan in Channels.Values.ToArray())
                Chan.Remove(PlayerName);
        }

        public Channel SendMessage(string ChannelName, string PlayerName, string Message, bool CheckNameExist)
        {
            Channel Chan = GetChannel(ChannelName);
            if (Chan == null || !Chan.DispatchMessage(PlayerName, Message, CheckNameExist))
                return null;

            return Chan;
        }
        public List<Channel> SendMessages(string PlayerName, string Message, bool CheckNameExist)
        {
            List<Channel> Chans = new List<Channel>();

            foreach (Channel Chan in Channels.Values.ToArray())
                if (Chan.DispatchMessage(PlayerName, Message, CheckNameExist))
                    Chans.Add(Chan);

            return Chans;
        }
    }
}
