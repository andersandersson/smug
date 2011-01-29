package se.lolektivet.droidsmug;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import java.util.TimerTask;
import java.util.Timer;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.OrientationEventListener;
import android.view.View;

public class DroidSmugActivity extends Activity
{
    private DroidSmugGLSurfaceView mGLView;
    private boolean _callNativeInitOnGlInit = false;
    private boolean _callWindowRestoredOnGlInit = false;
    private boolean _callWindowActivatedOnGlInit = false;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        if (Printouts.on()) Log.i(Printouts.tag(), "Activity.onCreate.");
        super.onCreate(savedInstanceState);
        mGLView = new DroidSmugGLSurfaceView(this);
        setContentView(mGLView);

        if (mGLView.hasGlContext())
        {
            nativeInit();
        }
        else
        {
            _callNativeInitOnGlInit = true;
        }
    }

    private void nativeInit()
    {
        NativeCallbacks.init(getResources());
        NativeFunctions.nativeInit(Heartbeat.getInstance().getFps());
        initGame();
        NativeFunctions.nativeWindowOpened();
    }

    public void surfaceCreatedCallback()
    {
        if (callNativeInitOnGlInit())
        {
            nativeInit();
        }
        Heartbeat.getInstance().start();
        if (callWindowRestoredOnGlInit())
        {
            NativeFunctions.nativeWindowRestored();
        }
        if (callWindowActivatedOnGlInit())
        {
            NativeFunctions.nativeWindowActivated();
        }
        resetGlInitCalls();
    }

    private boolean callNativeInitOnGlInit()
    {
        return _callNativeInitOnGlInit;
    }

    private boolean callWindowRestoredOnGlInit()
    {
        return _callWindowRestoredOnGlInit;
    }

    private boolean callWindowActivatedOnGlInit()
    {
        return _callWindowActivatedOnGlInit;
    }

    /** Being a bit lazy with this one, since these three are always set together atm. Need
     *  investigation.
     */
    private void resetGlInitCalls()
    {
        _callNativeInitOnGlInit = false;
        _callWindowRestoredOnGlInit = false;
        _callWindowActivatedOnGlInit = false;
    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();
        if (Printouts.on()) Log.i(Printouts.tag(), "Activity.onDestroy.");
        NativeFunctions.nativeWindowClosed();
        NativeFunctions.nativeDeInit();
    }

    @Override
    public void onStart()
    {
        super.onStart();
        if (Printouts.on()) Log.i(Printouts.tag(), "Activity.onStart.");
        if (mGLView.hasGlContext())
        {
            NativeFunctions.nativeWindowRestored();
            Heartbeat.getInstance().start();
        }
        else
        {
            _callWindowRestoredOnGlInit = true;
        }
    }

    @Override
    public void onStop()
    {
        super.onStop();
        if (Printouts.on()) Log.i(Printouts.tag(), "Activity.onStop.");
        Heartbeat.getInstance().stop();
        NativeFunctions.nativeWindowMinimized();
    }

    @Override
    protected void onPause()
    {
        super.onPause();
        if (Printouts.on()) Log.i(Printouts.tag(), "Activity.onPause.");
        mGLView.onPause();
        NativeFunctions.nativeWindowDeactivated();
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        if (Printouts.on()) Log.i(Printouts.tag(), "Activity.onResume.");
        mGLView.onResume();
        if (mGLView.hasGlContext())
        {
            NativeFunctions.nativeWindowActivated();
        }
        else
        {
            _callWindowActivatedOnGlInit = true;
        }
    }

    protected void initGame() {};

    static
    {
        System.loadLibrary("smug");
        System.loadLibrary("apitest");
    }
}

class NativeCallbacks
{
    private static FileOpener mFileOpener;

    private NativeCallbacks() {}

    public static void init(Resources res)
    {
        mFileOpener = new FileOpener(res);
    }

    /* To be called from native code only. */

    private static void openFile(String fileName)
    {
        mFileOpener.openFile(fileName);
    }

    private static void changeFps(float fps)
    {
        Heartbeat.getInstance().changeFps(fps);
    }
}

class Printouts
{
    private static boolean mDoPrintouts = true;
    private static final String mTag = "SMUG";
    public static void set(boolean active)
    {
        mDoPrintouts = active;
    }
    public static boolean on()
    {
        return mDoPrintouts;
    }
    public static String tag()
    {
        return mTag;
    }
}

class OrientationHandler extends OrientationEventListener
{
    public OrientationHandler(Context context)
    {
        super(context);
        if (canDetectOrientation())
        {
            enable();
        }
    }

    public void onOrientationChanged(int orientation)
    {
        NativeFunctions.nativeOrientationChange(orientation);
    }
}

class DroidSmugGLSurfaceView extends GLSurfaceView implements View.OnKeyListener
{
    private OrientationHandler mOrientationHandler;
    private DroidSmugRenderer mRenderer;

    public DroidSmugGLSurfaceView(DroidSmugActivity activity /* Context context */)
    {
        super((Context)activity);
        mRenderer = new DroidSmugRenderer(activity);
        setDebugFlags(DEBUG_CHECK_GL_ERROR | DEBUG_LOG_GL_CALLS);
        setRenderer(mRenderer);
        mOrientationHandler = new OrientationHandler((Context)activity);
        setFocusable(true);
        setFocusableInTouchMode(true);
        requestFocus();
    }

    public boolean hasGlContext()
    {
        return mRenderer.hasGlContext();
    }

/*     @Override
    public void onPause()
    {
        super.onPause();
    }

    @Override
    public void onResume()
    {
        super.onResume();
    } */

    public boolean onTouchEvent(final MotionEvent event)
    {
        if (event.getAction() == MotionEvent.ACTION_DOWN)
        {
            if (Printouts.on()) Log.i(Printouts.tag(), "Java received touch down event.");
            return NativeFunctions.nativeTouchDown();
        }
        if (event.getAction() == MotionEvent.ACTION_UP)
        {
            if (Printouts.on()) Log.i(Printouts.tag(), "Java received touch up event.");
            return NativeFunctions.nativeTouchUp();
        }
        return false;
    }

    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        if (NativeFunctions.nativeKeyDown(keyCode))
        {
            return true;
        }
        else
        {
            return super.onKeyDown(keyCode, event);
        }
    }
    public boolean onKeyUp(int keyCode, KeyEvent event)
    {
        if (NativeFunctions.nativeKeyUp(keyCode))
        {
            return true;
        }
        else
        {
            return super.onKeyUp(keyCode, event);
        }
    }

    public boolean onKey(View v, int keyCode, KeyEvent event)
    {
        if (event.getAction() == KeyEvent.ACTION_DOWN)
        {
            if (NativeFunctions.nativeKeyUp(keyCode))
            {
                return true;
            }
            else
            {
                return super.onKeyUp(keyCode, event);
            }
        }
        if (event.getAction() == KeyEvent.ACTION_UP)
        {
            if (NativeFunctions.nativeKeyUp(keyCode))
            {
                return true;
            }
            else
            {
                return super.onKeyUp(keyCode, event);
            }
        }
        return false;
    }
}

class DroidSmugRenderer implements GLSurfaceView.Renderer
{
    boolean _hasGlContext = false;
    DroidSmugActivity activity;

    DroidSmugRenderer(DroidSmugActivity activity)
    {
        super();
        this.activity = activity;
    }

    public boolean hasGlContext()
    {
        return _hasGlContext;
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        if (Printouts.on()) Log.i(Printouts.tag(), "DroidSmugRenderer.onSurfaceCreated");
        activity.surfaceCreatedCallback();
        _hasGlContext = true;
    }

    public void onSurfaceChanged(GL10 gl, int w, int h)
    {
        NativeFunctions.nativeResize(w, h);
    }

    public void onDrawFrame(GL10 gl)
    {
        NativeFunctions.nativeRender();
    }
}
