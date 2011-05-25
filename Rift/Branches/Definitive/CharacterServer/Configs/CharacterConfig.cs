using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace CharacterServer
{
    [aConfigAttributes("Configs/Characters.xml")]
    public class CharacterConfig : aConfig
    {
        public int CharacterServerPort = 6900;

        public string RpcIP = "127.0.0.1";
        public int RpcPort = 6800;
        public int RpcClientStartingPort = 6000;

        public bool UseCertificate = false;

        public DatabaseInfo AccountDB = new DatabaseInfo();
        public LogInfo LogLevel = new LogInfo();
    }
}
