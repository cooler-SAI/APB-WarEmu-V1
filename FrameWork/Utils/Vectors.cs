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

namespace FrameWork.Utils
{
    [Serializable]
    public class Vector2
    {
        public float x;
        public float y;

        public Vector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }
    }

    [Serializable]
    public class Vector3 : Vector2
    {
        public float z;

        public Vector3(float x, float y, float z)
            : base(x, y)
        {
            this.z = z;
        }
    }

    [Serializable]
    public class Quaternion : Vector3
    {
        public float w;

        public Quaternion(float x, float y, float z, float w)
            : base(x, y, z)
        {
            this.w = w;
        }

    }
}
