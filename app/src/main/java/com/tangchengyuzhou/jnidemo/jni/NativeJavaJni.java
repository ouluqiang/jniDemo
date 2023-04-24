package com.tangchengyuzhou.jnidemo.jni;

import android.util.Log;
import android.widget.Toast;

import com.tangchengyuzhou.jnidemo.App;

import java.util.Random;
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
     public native String notStaticJNIString();


    //非静态函数被c++调用
    public int notStaticGetRandom(int bound) {
        return new Random().nextInt(bound);
    }


    /**
     *  静态函数
     *  静态java调用c
     * @return
     */
    public static native String stringStaticFromJNI();
    public static void  stringStatic(int code){
        String s="静态 c调用 java s:"+code;
        Toast.makeText(App.application,s,Toast.LENGTH_SHORT).show();
    }






    // 动态 java 调用c
    public static native  String jniTrendsString(String s);

    /**
     *
     * 动态 c调用 java
     * @param event
     * @param code
     */
    public static void jniTrendsOnEvent(int event,int code){
        String s="动态 c调用 java event:"+event+"  code:"+code;
        Toast.makeText(App.application,s,Toast.LENGTH_SHORT).show();
        Log.e("JNI",s);
    }




}
