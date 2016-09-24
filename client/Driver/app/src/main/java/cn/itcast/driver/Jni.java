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

    public native boolean Login(String username, String password);

    static  {
        System.loadLibrary("myjni");
    }
}
