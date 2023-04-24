package com.tangchengyuzhou.jnidemo

import android.os.Build.VERSION_CODES.S
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import androidx.lifecycle.lifecycleScope
import com.tangchengyuzhou.jnidemo.databinding.ActivityMainBinding
import com.tangchengyuzhou.jnidemo.jni.NativeJavaJni
import kotlinx.coroutines.launch

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    var i=0
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        var jni=NativeJavaJni()
        NativeJavaJni.nativeSetupJNI()
        // Example of a call to a native method
//        binding.sampleText.text = NativeJni.stringFromJNI()
//        lifecycleScope.launch {
//            binding.tvNotStatic.text = jni.stringFromJNI()
//            binding.tvStatic.text = NativeJavaJni.stringStaticFromJNI()
//        }
        binding.tvNotStatic.setOnClickListener {
            i++;
            binding.tvNotStatic.text = jni.stringFromJNI()
        }
        binding.tvStatic.setOnClickListener {
            i++;
            binding.tvStatic.text = NativeJavaJni.stringStaticFromJNI()
        }
        binding.tvNoStaticC.setOnClickListener {
//            i++;
            binding.tvNoStaticC.text = jni.notStaticJNIString()
        }
        binding.tvStaticC.setOnClickListener {
//            i++;
            binding.tvStaticC.text = NativeJavaJni.stringStaticFromJNI()
        }
        binding.tvTrends.setOnClickListener {
            i++;
            binding.tvTrends.text = NativeJavaJni.jniTrendsString("动态 c调用java  "+i)
        }
    }

}