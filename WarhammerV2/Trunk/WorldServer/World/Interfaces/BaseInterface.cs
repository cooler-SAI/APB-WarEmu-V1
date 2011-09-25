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
    public class BaseInterface
    {
        public Object Obj;
        public bool Loaded = false;

        public BaseInterface()
        {

        }

        public BaseInterface(Object Obj)
        {
            this.Obj = Obj;
        }

        public virtual void Update(long Tick)
        {

        }

        public virtual bool Load()
        {
            Loaded = true;
            return true;
        }

        public bool IsLoad { get { return Loaded; } }

        public virtual bool HasObject()
        {
            return Obj != null;
        }
        public virtual bool HasPlayer()
        {
            if (!HasObject())
                return false;

            return Obj.IsPlayer();
        }
        public virtual bool HasUnit()
        {
            if (!HasObject())
                return false;

            return Obj.IsUnit();

        }
        public virtual Unit GetUnit()
        {
            if (!HasUnit())
                return null;

            return Obj.GetUnit();
        }
        public virtual Player GetPlayer()
        {
            if (!HasPlayer())
                return null;

            return Obj.GetPlayer();
        }

        public virtual void Save()
        {

        }
        public virtual void Stop()
        {
            Obj = null;
        }
    }
}
