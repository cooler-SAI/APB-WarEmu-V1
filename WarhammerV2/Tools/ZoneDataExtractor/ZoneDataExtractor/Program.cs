using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ZoneDataExtractor
{
    class Program
    {
        public class ZoneInfo
        {
            public UInt16 ZoneId;
            public UInt16 RegionId=0;
            public int Type=0;
            public string Name;
            public int Tier=0;
            public int Pairing=0;
            public int BorderZone=0;
        };

        static public StreamWriter Sw = new StreamWriter("zones.sql");

        static void Main(string[] args)
        {
            Console.WriteLine("ZoneExtractor : Lancement ...");
            Console.WriteLine("Ouverture de Zones.dat ...");

            try
            {
                StreamReader Rd = new StreamReader("zones.dat");

                ZoneInfo Info = null;

                string line = null;
                while (!Rd.EndOfStream)
                {
                    line = Rd.ReadLine();
                    if (line.StartsWith("[zone"))
                    {
                        DumpInfo(Info);

                        Info = new ZoneInfo();
                        Info.ZoneId = UInt16.Parse(line.Substring(5,3));
                    }

                    if (Info != null)
                    {
                        if (line.StartsWith("region="))
                        {
                            Info.RegionId = UInt16.Parse(line.Substring(7));
                        }
                        else if (line.StartsWith("type="))
                        {
                            Info.Type = int.Parse(line.Substring(5));
                        }
                        else if (line.StartsWith("name="))
                        {
                            Info.Name = line.Substring(5);
                        }
                        else if (line.StartsWith("tier="))
                        {
                            Info.Tier = int.Parse(line.Substring(5));
                        }
                        else if (line.StartsWith("pairing="))
                        {
                            Info.Pairing = int.Parse(line.Substring(8));
                        }
                    }
                }

                DumpInfo(Info);

                Console.ReadKey();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
                Console.ReadKey();
            }
        }

        static public void DumpInfo(ZoneInfo Info)
        {
            if(Info == null)
                return;

            Console.WriteLine("Dump de :" + Info.Name + ",ZoneId="+Info.ZoneId);
            Sw.WriteLine("UPDATE zone_info SET " +/*zonename='" + Info.Name + "',*/"`type`='" + Info.Type + "',`tier`='" + Info.Tier + "',`region`='" + Info.RegionId + "' WHERE `zoneid`='" + Info.ZoneId + "';");
            Sw.Flush();
        }
    }
}
