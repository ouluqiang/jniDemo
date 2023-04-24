//package com.tangchengyuzhou.jnidemo.jni
//
//import android.util.Log
//import java.util.*
//
//class NativeJni {
//    init {
//        System.loadLibrary("jnidemo")
//    }
//
//    external fun stringFromJNI(): String
//    external fun notStaticJNI():String
//    external fun nativeSetupJNI():Int
////    external fun staticJNI(s:String):String
//
//
//    //非静态函数被c++调用
//    private fun getRandom(bound: Int): Int {
//        return Random().nextInt(bound)
//    }
//
//    companion object{
//
//        @JvmStatic
//        fun jniOnEvent(event:Int,code:Int){
//            Log.e("JNI","jniOnEvent ${event}  ${code}")
//        }
//
//         fun getUUIDA(): String {
//            return UUID.randomUUID().toString()
//        }
//    }
//
//}
