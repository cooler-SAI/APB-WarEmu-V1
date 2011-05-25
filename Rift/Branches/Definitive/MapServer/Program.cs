using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;
using Common;


namespace MapServer
{
    class Program
    {
        static void Main(string[] args)
        {
            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);
            Log.Texte("", ",---.o", ConsoleColor.Cyan);
            Log.Texte("", "`---..,---.,---.,---.,---.", ConsoleColor.Cyan);
            Log.Texte("", "    |||---'|   ||   |,---|", ConsoleColor.Cyan);
            Log.Texte("", "`---'``---'`   '`   '`---^ Core", ConsoleColor.Cyan);
            Log.Texte("", "http://siennacore.com", ConsoleColor.Blue);
            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);


            ConsoleMgr.Start();
        }
    }
}
