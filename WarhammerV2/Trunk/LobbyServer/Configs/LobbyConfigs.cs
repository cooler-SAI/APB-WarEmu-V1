using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Config;
using FrameWork.Database;
using FrameWork.RpcV3;

namespace LobbyServer
{
    [ConfigAttributes("Configs/Lobby.xml")]
    public class LobbyConfigs : Config
    {
        public RpcClientInfo AccountCacher = new RpcClientInfo("127.0.0.1","Pass",2100);

        public int ClientPort = 8040;
        public string ClientVersion = "1.3.5";
    }
}
