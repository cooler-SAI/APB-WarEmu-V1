using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;

using Common;
using FrameWork;

namespace AccountCacher
{
    [ConsoleHandler("create",2,"New Account <Username,Password>")]
    public class CreateAccount : IConsoleHandler
    {
        public bool HandleCommand(string command, List<string> args)
        {
            string Username = args[0];
            string Password = args[1];

            Account Acct = Program.AcctMgr.GetAccount(Username);
            if (Acct != null)
            {
                Log.Error("CreateAccount", "This username is already used");
                return false;
            }

            Acct = new Account();
            Acct.Username = Username.ToLower();
            Acct.Password = Password.ToLower();
            Acct.Ip = "127.0.0.1";
            Acct.Token = "";
            Acct.GmLevel = 0;
            AccountMgr.Database.AddObject(Acct);

            return true;
        }
    }
}
