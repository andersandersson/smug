package se.lolektivet.droidsmug;

class NativeFunctions
{
    public static native void nativeInit(float fps);
    public static native void nativeResize(int w, int h);
    public static native void nativeRender();
    public static native void nativeDeInit();
    public static native boolean nativeTouchDown();
    public static native boolean nativeTouchUp();
    public static native boolean nativeKeyDown(int keyCode);
    public static native boolean nativeKeyUp(int keyCode);
    public static native void nativeOrientationChange(int orientation);
    public static native void nativeHeartbeat();

    public static native void nativeOpenFile(byte[] data);

    public static native void nativeWindowOpened();
    public static native void nativeWindowRestored();
    public static native void nativeWindowActivated();
    public static native void nativeWindowDeactivated();
    public static native void nativeWindowMinimized();
    public static native void nativeWindowClosed();
}
