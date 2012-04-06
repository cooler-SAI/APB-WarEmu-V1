/*
 * Copyright (C) 2011 APS
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

using Common;
using FrameWork;

namespace WorldServer
{
    public class CellMgr
    {
        public RegionMgr Region;
        public UInt16 X;
        public UInt16 Y;
        public CellSpawns Spawns;

        public CellMgr(RegionMgr Mgr, UInt16 OffX, UInt16 OffY)
        {
            Region = Mgr;
            X = OffX;
            Y = OffY;
            Spawns = Mgr.GetCellSpawn(OffX,OffY);
        }

        #region Objects

        public List<Object> _Objects = new List<Object>();
        public List<Player> _Players = new List<Player>();

        public void AddObject(Object Obj)
        {
            //Log.Success("AddObject", "[" + X + "," + Y + "] Cell Add " + Obj.Name);

            if (Obj.IsPlayer())
            {
                _Players.Add(Obj.GetPlayer());
                Region.LoadCells(X, Y, 1); // Si un joueur entre, alors on charge les cells autours sur 1 range
            }

           _Objects.Add(Obj);
           Obj._Cell = this;
        }
        public void RemoveObject(Object Obj)
        {
            //Log.Success("RemoveObject", "[" + X + "," + Y + "] Cell Remove " + Obj.Name);

            if (Obj.IsPlayer())
                _Players.Remove(Obj.GetPlayer());

            _Objects.Remove(Obj);
            Obj._Cell = null;
        }

        #endregion

        #region Spawns

        public bool _Loaded = false;
        public void Load()
        {
            if (_Loaded)
                return;

            Log.Success(ToString(), "Loading... ");
            foreach (Creature_spawn Spawn in Spawns.CreatureSpawns)
                Region.CreateCreature(Spawn);

            foreach (GameObject_spawn Spawn in Spawns.GameObjectSpawns)
                Region.CreateGameObject(Spawn);

            foreach (Chapter_Info Spawn in Spawns.ChapterSpawns)
                Region.CreateChapter(Spawn);

            foreach (PQuest_Info Quest in Spawns.PublicQuests)
                Region.CreatePQuest(Quest);

            _Loaded = true;
        }

        public override string ToString()
        {
            return "CellMgr["+X+","+Y+"]";
        }

        #endregion
    }
}
