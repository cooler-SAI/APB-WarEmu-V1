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

using Common;
using FrameWork;

namespace WorldServer
{
    public abstract class BaseInterface
    {
        public Object _Owner;
        public bool Loaded = false;
        public bool IsLoad { get { return Loaded; } }

        public BaseInterface()
        {

        }

        public virtual void SetOwner(Object Owner)
        {
            this._Owner = Owner;
        }


        public virtual bool Load()
        {
            Loaded = true;
            return true;
        }

        public virtual void Update(long Tick)
        {

        }

        public virtual void Stop()
        {

        }

        public virtual void Save()
        {

        }

        public virtual bool HasObject()
        {
            return _Owner != null;
        }

        public virtual bool HasPlayer()
        {
            if (!HasObject())
                return false;

            return _Owner.IsPlayer();
        }

        public virtual bool HasUnit()
        {
            if (!HasObject())
                return false;

            return _Owner.IsUnit();

        }

        public virtual Unit GetUnit()
        {
            if (!HasUnit())
                return null;

            return _Owner.GetUnit();
        }

        public virtual Player GetPlayer()
        {
            if (!HasPlayer())
                return null;

            return _Owner.GetPlayer();
        }
    }
}
