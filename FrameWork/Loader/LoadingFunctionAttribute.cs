using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace FrameWork
{
    [AttributeUsage(AttributeTargets.Method)]
    public class LoadingFunctionAttribute : Attribute
    {
        public bool CreateNewThread;

        public LoadingFunctionAttribute(bool CreateNewThread)
        {
            this.CreateNewThread = CreateNewThread;
        }
    }
}
