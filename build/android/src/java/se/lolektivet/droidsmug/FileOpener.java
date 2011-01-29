package se.lolektivet.droidsmug;

import android.content.res.Resources;
import android.content.res.AssetManager;
import java.io.InputStream;
import java.io.IOException;
import java.io.ByteArrayOutputStream;

public class FileOpener
{
    private Resources mResources;

    public FileOpener(Resources res)
    {
        mResources = res;
    }

    public void openFile(String fileName)
    {
        InputStream stream;
        try
        {
            stream = mResources.getAssets().open(fileName);
        }
        catch (IOException e)
        {
            return;
        }
        ByteArrayOutputStream out = new ByteArrayOutputStream(2048);
        final int chunkSize = 1024;
        byte[] bytes = new byte[chunkSize];
        int read = chunkSize;
        while (read == chunkSize)
        {
            try
            {
                read = stream.read(bytes, 0, chunkSize);
            }
            catch (IOException e)
            {
                return;
            }
            out.write(bytes, 0, read);
        }
        // byte[] fileBytes = out.toByteArray();
        NativeFunctions.nativeOpenFile(out.toByteArray());
    }
}
