using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Reflection;

namespace FrameWork
{
    public class LoaderMgr
    {
        #region Static

        static public int LoaderCount = 0;
        static public int MaxThread = 0;

        static public void Start()
        {
            long Start = TCPManager.GetTimeStampMS();

            List<LoadFunction> WaitEnd = new List<LoadFunction>();

            foreach (Assembly assembly in AppDomain.CurrentDomain.GetAssemblies())
                foreach (Type type in assembly.GetTypes())
                {
                    // Pick up a class
                    if (type.IsClass != true)
                        continue;

                    foreach (MethodInfo m in type.GetMethods())
                        foreach (LoadingFunctionAttribute at in m.GetCustomAttributes(typeof(LoadingFunctionAttribute), false))
                        {
                            LoadFunction loadfunction = (LoadFunction)Delegate.CreateDelegate(typeof(LoadFunction), m);

                            if (at.CreateNewThread)
                                InitLoad(loadfunction);
                            else
                                WaitEnd.Add(loadfunction);
                        }
                }

            Wait();

            foreach (LoadFunction Function in WaitEnd)
                Function.Invoke();

            long End = TCPManager.GetTimeStampMS();

            Log.Success("LoaderMgr", "Loading complete in : " + (End - Start) + "ms");
        }

        static public void InitLoad(LoadFunction Func)
        {
            if (MaxThread == 0 || LoaderCount < MaxThread)
            {
                new LoaderMgr(Func);
                Thread.Sleep(50);
            }
            else
                Func.Invoke();
        }
        static public void InitMultiLoad(MultiLoadFunction Func,int Count)
        {
            for (int i = 0; i < Count; ++i)
                new LoaderMgr(Func, Count,i);
        }

        static public void Wait()
        {
            while (LoaderCount > 0)
                Thread.Sleep(50);
        }

        #endregion

        public delegate void LoadFunction();
        public delegate void MultiLoadFunction(int ThreadCount, int Id);

        private int Id;
        private int Count;
        private LoadFunction _Function;
        private MultiLoadFunction _MultiFunction;

        public LoaderMgr(LoadFunction Function)
        {
            _Function = Function;
            ThreadStart Start = new ThreadStart(Load);
            Thread LoadThread = new Thread(Start);
            LoadThread.Start();
        }

        public LoaderMgr(MultiLoadFunction Function, int Count, int Id)
        {
            _MultiFunction = Function;
            this.Count = Count;
            this.Id = Id;
            ThreadStart Start = new ThreadStart(MultiLoad);
            Thread LoadThread = new Thread(Start);
            LoadThread.Start();
        }

        public void Load()
        {
            System.Threading.Interlocked.Increment(ref LoaderCount);
            try
            {
                if (_Function != null)
                {
                    Log.Debug("Load", "Loading : " + _Function.Method.Name);
                    _Function.Invoke();
                }
            }
            catch (Exception e)
            {
                Log.Error(_Function.Method.Name, e.ToString());
            }
            finally
            {
                System.Threading.Interlocked.Decrement(ref LoaderCount);
            }
        }

        public void MultiLoad()
        {
            System.Threading.Interlocked.Increment(ref LoaderCount);
            try
            {
                if (_MultiFunction != null)
                {
                    Log.Debug("Load", "Loading : " + _MultiFunction.Method.Name +", Id="+Id);
                    _MultiFunction.Invoke(Count,Id);
                }
            }
            catch (Exception e)
            {
                Log.Error(_MultiFunction.Method.Name, e.ToString());
            }
            finally
            {
                System.Threading.Interlocked.Decrement(ref LoaderCount);
            }
        }
    }
}
