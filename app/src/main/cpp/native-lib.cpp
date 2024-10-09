#include <jni.h>
#include <string>
#include <android/log.h>
#include <cmath>

//global reference to the interface
static jobject gCalculateSum = nullptr;
extern "C" JNIEXPORT void JNICALL
Java_me_ibrahim_jni_1project_MainActivity_initizlizeNative(
        JNIEnv *env,
        jobject instance) {
    gCalculateSum = env->NewGlobalRef(instance);
}

extern "C" JNIEXPORT void JNICALL
Java_me_ibrahim_jni_1project_MainActivity_calculateSum(
        JNIEnv *env,
        jobject,
        jint a,
        jint b
) {
    jint result = a + b;
    jclass listenerClass = env->GetObjectClass(gCalculateSum);
    jmethodID onSumCalculated = env->GetMethodID(listenerClass, "onSumCalculated", "(I)V");
    env->CallVoidMethod(gCalculateSum, onSumCalculated, result);
}


extern "C" JNIEXPORT jstring JNICALL
Java_me_ibrahim_jni_1project_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C" JNIEXPORT jint JNICALL
Java_me_ibrahim_jni_1project_MainActivity_sumFromJni(
        JNIEnv *env,
        jobject,
        jint a,
        jint b
) {
    jint sq_a = sqrt(a);
    jint sq_b = sqrt(b);

    __android_log_print(ANDROID_LOG_DEBUG, "Sq_A", "Sample result");
    return a + b;
}

extern "C" JNIEXPORT jintArray JNICALL
Java_me_ibrahim_jni_1project_MainActivity_convertToGrayscale(
        JNIEnv *env,
        jobject,
        jintArray pixels,
        jint width,
        jint height
) {
    jint *pixelsData = env->GetIntArrayElements(pixels, NULL);

    for (int i = 0; i < width * height; i++) {
        jint alpha = (pixelsData[i] & 0xFF000000);
        jint red = (pixelsData[i] & 0x00FF0000) >> 16;
        jint green = (pixelsData[i] & 0x0000FF00) >> 8;
        jint blue = (pixelsData[i] & 0x000000FF);

        //Convert to grayscale using luminosity method.
        //luminosity method decreases the contribution of red
        //increases the contribution of green & put blue color contribution in between these two.
        //formula: (R*0.21 + G*0.72 + B*0.07)
        jint gray = (red * 0.21 + green * 0.72 + blue * 0.07);
        gray = alpha | (gray << 16) | (gray << 8) | gray;
        pixelsData[i] = gray;
    }

    jintArray results = env->NewIntArray(width * height);
    env->SetIntArrayRegion(results, 0, width * height, pixelsData);

    env->ReleaseIntArrayElements(pixels, pixelsData, 0);
    return results;
}