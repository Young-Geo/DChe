package cn.itcast.driver;

/**
 * Created by xueguoliang on 16-9-24.
 */
public class Jni {
    private static Jni theOne = null;
    public static Jni getInstance()
    {
        if(theOne == null)
            theOne = new Jni();
        return theOne;
    }

    public String tag = "DirverDebugTag";

    public native boolean Login(String username, String password);
    public native boolean SetStatus(int status);
    public native int UpdateGeoInfo(double lng, double lat);

    static  {
        System.loadLibrary("myjni");
    }
}
