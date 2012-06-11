using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace FrameWork
{
    public class ProcessInformation
    {
        public ThreadExecutor Executor;
        public int ProcessCount;
        public ThreadExecutor.OnProcessStartDelegate OnStart;
        public ThreadExecutor.OnProcessExecutedDelegate OnExecuted;
        public ThreadExecutor.OnProcessEndDelegate OnEnd;
    }

    public class ThreadExecutor
    {
        public delegate void ProcessDelegate(ThreadExecutor Executor);
        public delegate void OnProcessStartDelegate();
        public delegate void OnProcessExecutedDelegate();
        public delegate void OnProcessEndDelegate();

        static public ThreadExecutor MainExecutor;
        static public int WaitTimeMS = 50; // 50ms check for executing
        static public List<ThreadExecutor> Executors = new List<ThreadExecutor>();

        static public void Start(int StartThreadCount, int WaitTimeMS)
        {
            ThreadExecutor.MainExecutor = new ThreadExecutor(-1);
            ThreadExecutor.WaitTimeMS = WaitTimeMS;

            for (int i = 0; i < StartThreadCount; ++i)
                CreateThread();
        }

        static public ThreadExecutor CreateThread()
        {
            ThreadExecutor Executor = new ThreadExecutor(Executors.Count);
            Executors.Add(Executor);
            return Executor;
        }

        static public ThreadExecutor GetThread(bool Create = false)
        {
            ThreadExecutor Selected = null;
            lock (Executors)
            {
                if (Create == false)
                {
                    foreach (ThreadExecutor Executor in Executors)
                    {
                        if (Executor.Processing)
                            continue;

                        Selected = Executor;
                        break;
                    }
                }
                else
                {
                    Selected = CreateThread();
                }
            }

            return Selected;
        }

        static public void AddProcess(ProcessDelegate Process, bool NewThread=false, int ExecuteCount=1)
        {

        }

        static public void UpdateList()
        {

        }

        public int ThreadID = 0;
        public bool IsRunning = true;
        public Thread CurrentThread;
        public ThreadStart ExecutingThread;
        public bool Processing = false;
        private ProcessDelegate ProcessEvent;
        private int ExecuteCount;

        public ThreadExecutor(int ThreadID)
        {
            Log.Debug("ThreadExecutor", "Creating new Thread : " + ThreadID);

            this.ThreadID = ThreadID;
            ExecutingThread = new ThreadStart(Process);
            CurrentThread = new Thread(ExecutingThread);
            CurrentThread.Start();
        }

        public void Process()
        {
            Log.Debug("ThreadExecutor", "[" + ThreadID + "] Processing ...");

            while (IsRunning)
            {
                if (ProcessEvent != null)
                {
                    Processing = true;
                    long StartTime = TCPManager.GetTimeStampMS();

                    try
                    {
                        ProcessEvent.Invoke(this);
                    }
                    catch (Exception e)
                    {
                        Log.Error("ThreadExecutor", e.ToString());
                    }

                    long Elapsed = TCPManager.GetTimeStampMS() - StartTime;

                    Processing = false;
                    ProcessEvent = null;

                    if(Elapsed < WaitTimeMS && IsRunning)
                        Thread.Sleep((int)(WaitTimeMS - Elapsed));
                }
                else if(IsRunning)
                    Thread.Sleep(WaitTimeMS);
            }

            Log.Debug("ThreadExecutor", "Process Stop : " + ThreadID);
        }

        public void Stop()
        {
            IsRunning = false;
        }

        public void SetProcess(ProcessDelegate Proc, int ExecuteCount)
        {

        }
    }
}
