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
        binding.tvNotStatic.setOnClickListener {
            i++;
            binding.tvNotStatic.text = jni.notStaticJNIString()
        }
        binding.tvStatic.setOnClickListener {
            i++;
            binding.tvStatic.text = NativeJavaJni.stringStaticFromJNI()
        }
        binding.tvTrends.setOnClickListener {
            i++;
            binding.tvTrends.text = NativeJavaJni.jniStaticTrendsString("动态 静态c调用java  "+i)
        }
        binding.tvNoTrends.setOnClickListener {
            i++;
            binding.tvNoTrends.text = jni.jniTrendsString("动态 no c调用java  "+i)
        }
    }

}