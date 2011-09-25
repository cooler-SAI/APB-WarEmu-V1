using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Windows.Forms;

using FrameWork;
using FrameWork.Logger;

namespace LauncherV2
{
    class Program
    {
        static void Main(string[] args)
        {
            Log.Info("Main", "Lancement du launcher");

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Master());
        }
    }
}
