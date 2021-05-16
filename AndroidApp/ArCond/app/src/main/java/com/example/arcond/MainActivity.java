package com.example.arcond;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.CheckBox;
import android.widget.TextView;

import com.example.arcond.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private ActivityMainBinding binding;

    private CheckBox cond;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.titulo;
        tv.setText(stringFromJNI());

        cond= findViewById(R.id.cond);
    }

    public void onButtonLigarClicked(View view) {
        cond.setText("Ligado");
        cond.setChecked(true);
    }

    public void onButtonDesligadoClicked(View view){
        cond.setText("Desligado");
        cond.setChecked(false);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}