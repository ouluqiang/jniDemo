#include <jni.h>
#include <string>
#include "log.h"
#include <pthread.h>

extern "C" {
extern void Android_JNI_jniTrendsOnEvent(int event, int err);
}

//extern "C" JNIEXPORT jstring JNICALL
//Java_com_tangchengyuzhou_jnidemo_jni_NativeJavaJni_stringFromJNI(
//        JNIEnv* env,
//        jobject /* this */) {
//    std::string hello = "No static Hello world from C++";
////    Android_JNI_jniTrendsOnEvent(0,1);
//    return env->NewStringUTF(hello.c_str());
//}

extern "C" JNIEXPORT jstring JNICALL
Java_com_tangchengyuzhou_jnidemo_jni_NativeJavaJni_stringStaticFromJNI(
        JNIEnv* env,
        jclass clazz) {
    std::string hello = "Static Hello world from C++";

    jmethodID mid = env->GetStaticMethodID( clazz, "stringStatic", "(I)V");
    env->CallStaticVoidMethod(clazz, mid,11);

//    Android_JNI_jniTrendsOnEvent(1,2);
    return env->NewStringUTF(hello.c_str());
}



int getMethodID(JNIEnv* env,
      jobject jobj){
    //非静态方法
    //获取jclass
    jclass jclz = env->GetObjectClass(jobj);

    //三个参数：1：对应类的jclass 2：方法名 3：方法签名
    jmethodID jmid = env->GetMethodID(jclz, "notStaticGetRandom", "(I)I");
    //根据返回值，调用相应方法，我这边返回值是int,第三个参数为可变参数，就是调用方法需要传入的参数
    jint int_random = env->CallIntMethod(jobj, jmid, 500);
    return int_random;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_tangchengyuzhou_jnidemo_jni_NativeJavaJni_notStaticJNIString(
        JNIEnv* env,
        jobject jobj) {
    int int_random=getMethodID(env,jobj);
    //打印
    log_error("int_random #### %d", int_random);
    char buf[10] = {0};
    sprintf(buf, "%d", int_random);
    return env->NewStringUTF(buf);
}


