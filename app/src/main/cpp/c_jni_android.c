//
// Created by 63252 on 2023/4/23.
//

#include <pthread.h>
#include <jni.h>
#include <android/log.h>

//包名
#define SDL_JAVA_PREFIX                                 com_tangchengyuzhou_jnidemo_jni
#define CONCAT1(prefix, class, function)                CONCAT2(prefix, class, function)
#define CONCAT2(prefix, class, function)                Java_ ## prefix ## _ ## class ## _ ## function
#define SDL_JAVA_INTERFACE(function)                    CONCAT1(SDL_JAVA_PREFIX, NativeJavaJni, function)

JNIEXPORT int JNICALL SDL_JAVA_INTERFACE(nativeSetupJNI)(
        JNIEnv *env, jclass cls);
JNIEXPORT jstring JNICALL SDL_JAVA_INTERFACE(stringJni)(
        JNIEnv *env, jclass cls,jstring);

static JNINativeMethod SDLActivity_tab[] = {
        {"nativeSetupJNI",   "()I",                  SDL_JAVA_INTERFACE(nativeSetupJNI)},
        {"stringJni",   "(Ljava/lang/String;)Ljava/lang/String;",                  SDL_JAVA_INTERFACE(stringJni)},
};

static jmethodID jniEvent;
static jmethodID stringJni;
static pthread_key_t mThreadKey;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;
static JavaVM *mJavaVM = NULL;

static jclass mActivityClass;



/* Set local storage value */
static int
Android_JNI_SetEnv(JNIEnv *env) {
    int status = pthread_setspecific(mThreadKey, env);
    if (status < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "SDL", "Failed pthread_setspecific() in Android_JNI_SetEnv() (err=%d)", status);
    }
    return status;
}

/* Get local storage value */
JNIEnv* Android_JNI_GetEnv(void)
{
    /* Get JNIEnv from the Thread local storage */
    JNIEnv *env = pthread_getspecific(mThreadKey);
    if (env == NULL) {
        /* If it fails, try to attach ! (e.g the thread isn't created with SDL_CreateThread() */
        int status;

        /* There should be a JVM */
        if (mJavaVM == NULL) {
            __android_log_print(ANDROID_LOG_ERROR, "SDL", "Failed, there is no JavaVM");
            return NULL;
        }

        /* Attach the current thread to the JVM and get a JNIEnv.
         * It will be detached by pthread_create destructor 'Android_JNI_ThreadDestroyed' */
        status = (*mJavaVM)->AttachCurrentThread(mJavaVM, &env, NULL);
        if (status < 0) {
            __android_log_print(ANDROID_LOG_ERROR, "SDL", "Failed to attach current thread (err=%d)", status);
            return NULL;
        }

        /* Save JNIEnv into the Thread local storage */
        if (Android_JNI_SetEnv(env) < 0) {
            return NULL;
        }
    }

    return env;
}


/* Destructor called for each thread where mThreadKey is not NULL */
static void
Android_JNI_ThreadDestroyed(void *value)
{
    /* The thread is being destroyed, detach it from the Java VM and set the mThreadKey value to NULL as required */
    JNIEnv *env = (JNIEnv *) value;
    if (env != NULL) {
        (*mJavaVM)->DetachCurrentThread(mJavaVM);
        Android_JNI_SetEnv(NULL);
    }
}

/* Creation of local storage mThreadKey */
static void
Android_JNI_CreateKey(void)
{
    int status = pthread_key_create(&mThreadKey, Android_JNI_ThreadDestroyed);
    if (status < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "SDL", "Error initializing mThreadKey with pthread_key_create() (err=%d)", status);
    }
}

static void
Android_JNI_CreateKey_once(void)
{
    int status = pthread_once(&key_once, Android_JNI_CreateKey);
    if (status < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "SDL", "Error initializing mThreadKey with pthread_once() (err=%d)", status);
    }
}



/* Activity initialization -- called before SDL_main() to initialize JNI bindings */
JNIEXPORT jstring JNICALL SDL_JAVA_INTERFACE(stringJni)(JNIEnv *env, jclass cls,jstring s)
{
    return s;
//    return (*env)->NewStringUTF(env,s);
}
JNIEXPORT int JNICALL SDL_JAVA_INTERFACE(nativeSetupJNI)(JNIEnv *env, jclass cls)
{
    __android_log_print(ANDROID_LOG_VERBOSE, "SDL", "nativeSetupJNI()");

    /*
     * Create mThreadKey so we can keep track of the JNIEnv assigned to each thread
     * Refer to http://developer.android.com/guide/practices/design/jni.html for the rationale behind this
     */
    Android_JNI_CreateKey_once();

    /* Save JNIEnv of SDLActivity */
    Android_JNI_SetEnv(env);

    if (mJavaVM == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, "SDL", "failed to found a JavaVM");
    }
    mActivityClass = (jclass)((*env)->NewGlobalRef(env, cls));
    if (mActivityClass == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, "SDL", "failed to found a JavaVM");
    }

    jniEvent = (*env)->GetStaticMethodID(env, mActivityClass, "jniOnEvent","(II)V");
    stringJni = (*env)->GetStaticMethodID(env, mActivityClass, "stringJni","(Ljava/lang/String;)Ljava/lang/String;");

}



void Android_JNI_jniOnEvent(int w, int h)
{
    JNIEnv *env = Android_JNI_GetEnv();

//    jstring jhint = (*env)->NewStringUTF(env, (hint ? hint : ""));
    (*env)->CallStaticVoidMethod(env, mActivityClass, jniEvent, w, h );
//    (*env)->DeleteLocalRef(env, jhint);
}
