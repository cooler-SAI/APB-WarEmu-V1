/*
 * Copyright (C) 2013 APS
 *	http://AllPrivateServer.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;
using Google.ProtocolBuffers;
using WarProtocol;

using FrameWork;

namespace Common
{
    public enum AuthResult
    {
        AUTH_SUCCESS = 0x00,
        AUTH_ACCT_EXPIRED = 0x07,
        AUTH_ACCT_BAD_USERNAME_PASSWORD = 0x09,
        AUTH_ACCT_TERMINATED = 0x0D,
        AUTH_ACCT_SUSPENDED = 0x0E
    };

    [Rpc(true, System.Runtime.Remoting.WellKnownObjectMode.Singleton,1)]
    public class AccountMgr : RpcObject
    {
        // Account Database
        static public MySQLObjectDatabase Database = null;

        #region Account

        // Account : Username,Account
        public Dictionary<string, Account> _Accounts = new Dictionary<string, Account>();

        public bool LoadAccount(string Username)
        {
            Username = Username.ToLower();

            try
            {
                Account Acct = Database.SelectObject<Account>("Username='" + Database.Escape(Username) + "'");

                if (Acct == null)
                {
                    Log.Error("LoadAccount", "Compte Introuvable : " + Username);
                    return false;
                }

                if (Acct.Password != null && Acct.Password.Length > 0)
                {
                    Acct.CryptPassword = Account.ConvertSHA256(Acct.Username + ":" + Acct.Password);
                    Acct.Password = "";
                    Database.SaveObject(Acct);
                }

                lock (_Accounts)
                    _Accounts[Username] = Acct;
            }
            catch (Exception e)
            {
                Log.Error("LoadAccount", e.ToString());
                return false;
            }

            return true;
        }
        public Account GetAccount(string Username)
        {
            Username = Username.ToLower();

            //Log.Info("GetAccount", Username);

            if (!LoadAccount(Username))
            {
                Log.Error("GetAccount", "Compte Introuvable : " + Username);
                return null;
            }

            lock (_Accounts)
                return _Accounts[Username];
        }
        public bool CheckAccount(string Username, string Password)
        {
            Username = Username.ToLower();

            //Log.Info("CheckAccount", Username + " : " + Password);

            try
            {
                Account Acct = GetAccount(Username);

                if (Acct == null)
                {
                    Log.Error("CheckAccount", "Compte introuvable : " + Username);
                    return false;
                }

                if (Acct.CryptPassword != Password)
                {
                    ++Acct.InvalidPasswordCount;
                    Database.ExecuteNonQuery("UPDATE Accounts SET InvalidPasswordCount = InvalidPasswordCount+1 WHERE Username = '" + Database.Escape(Username) + "'");
                    return false;
                }

            }
            catch (Exception e)
            {
                Log.Error("CheckAccount", e.ToString());
                return false;
            }

            return true;
        }
        public string GenerateToken(string Username)
        {
            Username = Username.ToLower();
            Account Acct = GetAccount(Username);

            if (Acct == null)
            {
                //Log.Error("GenerateToken", "Compte introuvable : " + Username);
                return "ERREUR";
            }

            string GUID = Guid.NewGuid().ToString();
            //Log.Info("GenerateToken", Username + "," + GUID);
            Acct.Token = Convert.ToBase64String(ASCIIEncoding.ASCII.GetBytes(GUID));
            
            Database.SaveObject(Acct);
            return Acct.Token;
        }
        public AuthResult CheckToken(string Username, string Token)
        {
            Account Acct = GetAccount(Username);
            if (Acct == null)
                return AuthResult.AUTH_ACCT_BAD_USERNAME_PASSWORD;

            if (Acct.Token != Convert.ToBase64String(ASCIIEncoding.ASCII.GetBytes(Token)))
                return AuthResult.AUTH_ACCT_BAD_USERNAME_PASSWORD;

            return AuthResult.AUTH_SUCCESS;
        }

        #endregion

        #region Realm

        public Dictionary<byte, Realm> _Realms = new Dictionary<byte, Realm>();
        
        public void LoadRealms()
        {
            foreach (Realm Rm in Database.SelectAllObjects<Realm>())
                AddRealm(Rm);
        }
        public bool AddRealm(Realm Rm)
        {
            lock (_Realms)
            {
                if (_Realms.ContainsKey(Rm.RealmId))
                    return false;

                //Log.Info("AddRealm", "New Realm : " + Rm.Name);

                _Realms.Add(Rm.RealmId, Rm);
            }

            return true;
        }
        public Realm GetRealm(byte RealmId)
        {
            //Log.Info("GetRealm", "RealmId = " + RealmId);
            lock (_Realms)
                if (_Realms.ContainsKey(RealmId))
                    return _Realms[RealmId];

            return null;
        }
        public List<Realm> GetRealms()
        {
            List<Realm> Rms = new List<Realm>();
            Rms.AddRange(_Realms.Values);
            return Rms;
        }
        public Realm GetRealmByRpc(int RpcId)
        {
            lock (_Realms)
                return _Realms.Values.ToList().Find(info => info.Info != null && info.Info.RpcID == RpcId);
        }
        public bool UpdateRealm(RpcClientInfo Info, byte RealmId)
        {
            Realm Rm = GetRealm(RealmId);

            if (Rm != null)
            {
                Log.Success("Realm", "Realm (" + Rm.Name + ") online at " + Info.Ip+":"+Info.Port);
                Rm.Info = Info;
                Rm.Online = 1;
                Rm.OrderCount = 0;
                Rm.DestructionCount = 0;
                Rm.OnlineDate = DateTime.Now;
                Rm.Dirty = true;
                Database.SaveObject(Rm);
            }
            else
            {
                Log.Error("UpdateRealm", "Realm (" + RealmId + ") missing : Please complete the table 'realm'");
                return false;
            }

            return true;
        }
        public void UpdateRealm(byte RealmId, uint OnlinePlayers, uint OrderCount, uint DestructionCount)
        {
            Realm Rm = GetRealm(RealmId);

            if (Rm == null)
                return;

            Log.Info("Realm", RealmId + "- Online : " + OnlinePlayers + ", Order=" + OrderCount + ", Destru=" + DestructionCount);

            Rm.OnlinePlayers = OnlinePlayers;
            Rm.OrderCount = OrderCount;
            Rm.DestructionCount = DestructionCount;
            Rm.Dirty = true;
            Database.SaveObject(Rm);
        }

        public void UpdateRealmCharacters(byte RealmId, uint OrderCharacters, uint DestruCharacters)
        {
            Realm Rm = GetRealm(RealmId);

            if (Rm == null)
                return;

            Rm.OrderCharacters = OrderCharacters;
            Rm.DestruCharacters = DestruCharacters;
            Rm.Dirty = true;
            Database.ExecuteNonQuery("UPDATE realms SET OrderCharacters =" + OrderCharacters + ", DestruCharacters=" + DestruCharacters + " WHERE RealmId = " + RealmId);
        }

        public static ClusterProp setProp(string name, string value)
        {
            return ClusterProp.CreateBuilder().SetPropName(name)
                                              .SetPropValue(value)
                                              .Build();
        }
        public byte[] BuildRealms(string OverrideIp)
        {
            GetClusterListReply.Builder ClusterListReplay = GetClusterListReply.CreateBuilder();
            ClusterListReplay.ResultCode = ResultCode.RES_SUCCESS;

            lock (_Realms)
            {
                //Log.Debug("BuildRealm", "Sending " + _Realms.Count + " realm(s)");

                foreach (Realm Rm in _Realms.Values)
                {
                    ClusterInfo.Builder cluster = ClusterInfo.CreateBuilder();
                    cluster.SetClusterId(Rm.RealmId).SetClusterName(Rm.Name)
                        .SetLobbyHost(OverrideIp != null ? OverrideIp : Rm.Adresse)
                           .SetLobbyPort((uint)Rm.Port)
                           .SetLanguageId(0)
                           .SetMaxClusterPop(500)
                           .SetClusterPopStatus(ClusterPopStatus.POP_HIGH)
                           .SetLanguageId(0)
                           .SetClusterStatus(Rm.Info != null ? ClusterStatus.STATUS_ONLINE : ClusterStatus.STATUS_OFFLINE);

                    cluster.AddServerList(
                        ServerInfo.CreateBuilder().SetServerId(Rm.RealmId)
                                                  .SetServerName(Rm.Name)
                                                  .Build());

                    cluster.AddPropertyList(setProp("setting.allow_trials", Rm.AllowTrials));
                    cluster.AddPropertyList(setProp("setting.charxferavailable", Rm.CharfxerAvailable));
                    cluster.AddPropertyList(setProp("setting.language", Rm.Language));
                    cluster.AddPropertyList(setProp("setting.legacy", Rm.Legacy));
                    cluster.AddPropertyList(setProp("setting.manualbonus.realm.destruction", Rm.BonusDestruction));
                    cluster.AddPropertyList(setProp("setting.manualbonus.realm.order", Rm.BonusOrder));
                    cluster.AddPropertyList(setProp("setting.min_cross_realm_account_level", "0"));
                    cluster.AddPropertyList(setProp("setting.name", Rm.Name));
                    cluster.AddPropertyList(setProp("setting.net.address", OverrideIp != null ? OverrideIp : Rm.Adresse));
                    cluster.AddPropertyList(setProp("setting.net.port", Rm.Port.ToString()));
                    cluster.AddPropertyList(setProp("setting.redirect", Rm.Redirect));
                    cluster.AddPropertyList(setProp("setting.region", Rm.Region));
                    cluster.AddPropertyList(setProp("setting.retired", Rm.Retired));
                    cluster.AddPropertyList(setProp("status.queue.Destruction.waiting", Rm.WaitingDestruction));
                    cluster.AddPropertyList(setProp("status.queue.Order.waiting", Rm.WaitingOrder));
                    cluster.AddPropertyList(setProp("status.realm.destruction.density", Rm.DensityDestruction));
                    cluster.AddPropertyList(setProp("status.realm.order.density",Rm.DensityOrder));
                    cluster.AddPropertyList(setProp("status.servertype.openrvr", Rm.OpenRvr));
                    cluster.AddPropertyList(setProp("status.servertype.rp", Rm.Rp));
                    cluster.AddPropertyList(setProp("status.status", Rm.Status));
                    cluster.Build();
                    ClusterListReplay.AddClusterList(cluster);
                }
            }

            return ClusterListReplay.Build().ToByteArray();
        }
        public override void  OnClientDisconnected(RpcClientInfo Info)
        {
            Realm Rm = GetRealmByRpc(Info.RpcID);
            if (Rm != null && Rm.Info.RpcID == Info.RpcID)
            {
                Log.Error("Realm", "Realm offline : " + Rm.Name);
                Rm.Info = null;
                Rm.Online = 0;
                Rm.Dirty = true;
                Database.SaveObject(Rm);
            }
        }

        #endregion
    }
}
