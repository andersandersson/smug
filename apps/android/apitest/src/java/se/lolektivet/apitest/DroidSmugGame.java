package se.lolektivet.apitest;

import se.lolektivet.droidsmug.DroidSmugActivity;

public class DroidSmugGame extends DroidSmugActivity
{
    public void initGame()
    {
        System.out.println("DroidSmugGame init!");
        nativeGameInit();
    }

    private native void nativeGameInit();
}
