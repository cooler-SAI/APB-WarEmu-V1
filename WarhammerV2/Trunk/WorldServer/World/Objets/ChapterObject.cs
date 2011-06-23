using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;
namespace WorldServer
{
    public class ChapterObject : Object
    {
        public Chapter_Info Info;

        public ChapterObject()
            : base()
        {

        }

        public ChapterObject(Chapter_Info Info)
            : this()
        {
            this.Info = Info;
            Name = Info.Name;
        }

        public override void OnLoad()
        {
            Log.Success("ChapterObject", "OnLoad");
            X = Info.PinX;
            Y = Info.PinY;
            Z = 16384;
            SetOffset(Info.OffX, Info.OffY);
            Region.UpdateRange(this);

            Log.Success("ChapterObject", "Range =" + _ObjectRanged.Count);

            base.OnLoad();
        }

        public override void SendMeTo(Player Plr)
        {
            Log.Success("SendMeTo", "ChapterObject");
            Plr.TokInterface.AddTok(Info.TokExploreEntry);
            Plr.TokInterface.AddTok(Info.TokEntry);
        }
    }
}
