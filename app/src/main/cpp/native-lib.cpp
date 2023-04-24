#include <jni.h>
#include <string>
#include "log.h"
#include <pthread.h>

extern "C" {
extern void Android_JNI_jniOnEvent(int event, int err);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_tangchengyuzhou_jnidemo_jni_NativeJavaJni_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "No Hello world from C++";
    Android_JNI_jniOnEvent(0,1);
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_tangchengyuzhou_jnidemo_jni_NativeJavaJni_stringStaticFromJNI(
        JNIEnv* env,
        jclass clazz) {
    std::string hello = "Static Hello world from C++";
    Android_JNI_jniOnEvent(1,2);
    return env->NewStringUTF(hello.c_str());
}



extern "C" JNIEXPORT jstring JNICALL
Java_com_tangchengyuzhou_jnidemo_jni_NativeJni_notStaticJNI(
        JNIEnv* env,
        jobject jobj) {
    //非静态方法
    //获取jclass
    jclass jclz = env->GetObjectClass(jobj);

    //三个参数：1：对应类的jclass 2：方法名 3：方法签名
    jmethodID jmid = env->GetMethodID(jclz, "getRandom", "(I)I");
    //根据返回值，调用相应方法，我这边返回值是int,第三个参数为可变参数，就是调用方法需要传入的参数
    jint int_random = env->CallIntMethod(jobj, jmid, 500);

    //打印
    log_error("int_random #### %d", int_random);
    char buf[10] = {0};
    sprintf(buf, "%d", int_random);
    return env->NewStringUTF(buf);
}







//extern "C" JNIEXPORT jstring JNICALL
//Java_com_tangchengyuzhou_jnidemo_jni_NativeJni_staticJNI(
//        JNIEnv* env,
//        jobject jobj,jstring  ject) {
//
//    //获取java传递的参数
////    const char *strCont = env->GetStringUTFChars(str, JNI_FALSE);
//    //通过反射获取java类
////    jclass ccallj = env->FindClass("com/niubashaoye/ndk/jni/CCallJavaTools");
//    //得到方法id
//    //参数列表：反射类，方法名称，方法签名
////    jmethodID methodId = env->GetStaticMethodID(ccallj, "getUUID",
////                                                "(Ljava/lang/String;)Ljava/lang/String;");
//
//
//    //调用方法,CallStaticObjectMethod第一个参数是反射类名
////    jstring result = (jstring) env->CallStaticObjectMethod(ccallj, methodId,
////                                                           env->NewStringUTF(strCont));
//
//
////    return result;
//
////    //1 . 获取 Java 对应的 Class 对象
////    jclass jclz = env->GetObjectClass(jobj);
//    jclass jclz = env->FindClass("com/tangchengyuzhou/jnidemo/jni/NativeJni");
////    jmethodID method_logInfo = env->GetStaticMethodID(jclz, "getUUID" , "(Ljava/lang/String;)V");
////
////    //获取静态方法的jmethodID
//    jmethodID jmid = env->GetStaticMethodID(jclz, "getUUIDA", "()Ljava/lang/String;");
////    //调用静态Java方法
//    jstring jstr = (jstring)env->CallStaticObjectMethod(jclz, jmid);
////
////    //将Java方法返回值转换为c++string对象
////    string uuid = env->GetStringUTFChars(jstr, NULL);
//
////打印
//    log_error("int_random #### %d",jstr);
//    char buf[10] = {0};
//    sprintf(buf,"%d",jstr);
//    return env->NewStringUTF(buf);
//
//}