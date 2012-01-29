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

using FrameWork;
using Common;

namespace WorldServer
{
    [AttributeUsage(AttributeTargets.Class)]
    public class IAbilityTypeAttribute : Attribute
    {
        public UInt16 AbilityType;
        public string TypeDescription;

        public IAbilityTypeAttribute(UInt16 AbilityType, string TypeDescription)
        {
            this.AbilityType = AbilityType;
            this.TypeDescription = TypeDescription;
        }
    }

    public abstract class IAbilityTypeHandler
    {
        public abstract void Start(Ability Ab);

        public abstract void Update();

        public abstract GameData.AbilityResult CanCast();

        public abstract void Cast();

        public abstract void Stop();

        public abstract UInt32 GetAbilityDamage();
    }
}
