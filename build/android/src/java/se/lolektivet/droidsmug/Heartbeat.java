package se.lolektivet.droidsmug;

import java.util.TimerTask;
import java.util.Timer;

class Heartbeat
{
    private static Heartbeat mInstance = null;

    private Timer mUpdateTimer;
    private HeartbeatTask mTask;
    private float mFps = 20.0f;

    private class HeartbeatTask extends TimerTask
    {
        @Override
        public void run()
        {
            NativeFunctions.nativeHeartbeat();
        }
    }

    public static Heartbeat getInstance()
    {
        if (mInstance == null)
        {
            mInstance = new Heartbeat();
        }
        return mInstance;
    }

    public void start()
    {
        if (mUpdateTimer == null)
        {
            mUpdateTimer = new Timer();
            mTask = new HeartbeatTask();
            mUpdateTimer.schedule(mTask, 0, (int)(1000.0f / mFps));
        }
    }

    public void stop()
    {
        if (mUpdateTimer != null)
        {
            mTask.cancel();
            mUpdateTimer.cancel();
            mUpdateTimer.purge();
            mUpdateTimer = null;
            mTask = null;
        }
    }

    public float getFps()
    {
        return mFps;
    }

    protected static void changeFps(float fps)
    {
        Heartbeat instance = getInstance();
        boolean wasStarted = false;
        if (instance.mUpdateTimer != null)
        {
            wasStarted = true;
            instance.stop();
        }
        instance.mFps = fps;
        if (wasStarted)
        {
            instance.start();
        }
    }
}

