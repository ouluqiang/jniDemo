package com.tangchengyuzhou.jnidemo.jni;

import android.util.Log;
import android.widget.Toast;

import java.util.UUID;

public class NativeJavaJni {

    static {
        System.loadLibrary("jnidemo");
    }
    //初始化
     public static native  int nativeSetupJNI();

    /**
     * 非静态函数
     * 静态java调用c
     */
     public native String stringFromJNI();

    //非静态函数被c++调用
    public int getRandom(int bound) {
        return bound;
    }


    /**
     *  静态函数
     *  静态java调用c
     * @return
     */
    public static native String stringStaticFromJNI();


    //静态函数 动态 java 调用c
    public static native  String stringJni(String s);

    /**
     * 静态函数
     * 动态 c调用 java
     * @param event
     * @param code
     */
    public static void jniOnEvent(int event,int code){
        Log.e("JNI","jniOnEvent event:"+event+"  code:"+code);
    }




}
