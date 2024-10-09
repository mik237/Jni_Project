package me.ibrahim.jni_project

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import android.widget.Toast
import me.ibrahim.jni_project.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity(), CalculateSum {

    private lateinit var binding: ActivityMainBinding
    private var bitmap: Bitmap? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        initizlizeNative()
        // Example of a call to a native method
        binding.sampleText.text = "Sum from JNI: ${sumFromJni(10, 20)}"
        calculateSum(10, 40)

        initViews()
    }

    private fun initViews() {

        bitmap = loadImageFromAssets("ibrahim.jpg")

        binding.imageView.setImageBitmap(bitmap)

        binding.btnConvertToGrayscale.setOnClickListener {
            //val bitmap = loadImageFromAssets("ibrahim.jpg")

            bitmap?.let {
                val pixels = bitmapToIntArray(it)
                val processedPixels = convertToGrayscale(pixels, it.width, it.height)
                val processedBitmap = Bitmap.createBitmap(processedPixels, it.width, it.height, Bitmap.Config.ARGB_8888)
                binding.imageView.setImageBitmap(processedBitmap)
            }
        }
    }

    /**
     * A native method that is implemented by the 'jni_project' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
    external fun sumFromJni(a: Int, b: Int): Int
    external fun convertToGrayscale(pixels: IntArray, width: Int, height: Int): IntArray
    external fun calculateSum(a: Int, b: Int)
    external fun initizlizeNative()

    private fun loadImageFromAssets(fileName: String): Bitmap? {
        return try {
            this.assets.open(fileName).use { inputstream ->
                BitmapFactory.decodeStream(inputstream)
            }
        } catch (e: Exception) {
            e.printStackTrace()
            null
        }
    }


    private fun bitmapToIntArray(bitmap: Bitmap): IntArray {
        val width = bitmap.width
        val height = bitmap.height
        val pixelsArray = IntArray(width * height)
        bitmap.getPixels(pixelsArray, 0, width, 0, 0, width, height)
        return pixelsArray
    }

    companion object {
        // Used to load the 'jni_project' library on application startup.
        init {
            System.loadLibrary("jni_project")
        }
    }


    override fun onSumCalculated(result: Int) {
        runOnUiThread {
            Toast.makeText(this@MainActivity, "Sum: $result", Toast.LENGTH_SHORT).show()
        }
    }
}