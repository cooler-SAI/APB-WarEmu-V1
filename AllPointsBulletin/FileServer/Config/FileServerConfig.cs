using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace FileServer
{
    [aConfigAttributes("Configs/FileServerConfig.xml")]
    public class FileServerConfig : aConfig
    {
        public LogInfo LogLevel = new LogInfo();
        public RpcServerConfig RpcInfo = new RpcServerConfig("127.0.0.1", 2100, 2201);
    }
}
