using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace MapServer
{
    public class GameRiftServer : RiftServer
    {
        protected override BaseClient GetNewClient()
        {
            return new GameRiftClient(this);
        }
    }
}
