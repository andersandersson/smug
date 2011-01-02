package se.lolektivet.apitest;

import android.view.MotionEvent;
import se.lolektivet.droidsmug.DroidSmugActivity;

public class DroidSmugGame extends DroidSmugActivity
{
    public void initGame()
    {
        System.out.println("DroidSmugGame init!");
        nativeGameInit();
    }

    static int mainPointerId = 0;
    static boolean isInTouchSequence = false;

    public boolean onTouchEvent(final MotionEvent event)
    {
        switch (event.getActionMasked())
        {
            case MotionEvent.ACTION_DOWN:
            {
                final int index = event.getActionIndex();
                isInTouchSequence = true;
                mainPointerId = event.getPointerId(event.getActionIndex());
                nativeTouchDown(event.getX(index), event.getY(index));
                break;
            }
            case MotionEvent.ACTION_MOVE:
            {
                if (!isInTouchSequence)
                {
                    return false;
                }
                final int index = event.findPointerIndex(mainPointerId);
                if (index != -1)
                {
                    nativeTouchMove(event.getX(index), event.getY(index));
                }
                break;
            }
            case MotionEvent.ACTION_UP:
                // Fall through
            case MotionEvent.ACTION_CANCEL:
            {
                final int index = event.findPointerIndex(mainPointerId);
                nativeTouchUp(event.getX(index), event.getY(index));
                isInTouchSequence = false;
                mainPointerId = -1;
                break;
            }
            case MotionEvent.ACTION_POINTER_DOWN:
            {
                return false;
            }
            case MotionEvent.ACTION_POINTER_UP:
            {
                return false;
            }
            default:
        }
        return true;
    }

    private native void nativeTouchDown(float x, float y);
    private native void nativeTouchMove(float x, float y);
    private native void nativeTouchUp(float x, float y);
    private native void nativeGameInit();
}
