using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace CharacterServer
{
    [aConfigAttributes("Configs/CharacterServerConfig.xml")]
    public class CharacterServerConfig : aConfig
    {
        public RpcServerConfig RpcInfo = new RpcServerConfig("127.0.0.1", 2000, 2150);
        public LogInfo LogLevel = new LogInfo();
        public DatabaseInfo CharacterDatabase = new DatabaseInfo();
    }
}
