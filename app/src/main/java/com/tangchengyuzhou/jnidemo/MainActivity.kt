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
        // Example of a call to a native method
//        binding.sampleText.text = NativeJni.stringFromJNI()
        lifecycleScope.launch {
            NativeJavaJni.nativeSetupJNI()
//            binding.sampleText.text = jni.stringFromJNI()
            binding.sampleText.text = NativeJavaJni.stringStaticFromJNI()
        }
        binding.sampleText.setOnClickListener {
            i++;
            binding.sampleText.text = NativeJavaJni.stringJni("点击"+i)
        }
    }

}