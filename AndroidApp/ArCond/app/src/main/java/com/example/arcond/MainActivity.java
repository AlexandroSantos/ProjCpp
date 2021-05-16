package com.example.arcond;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.example.arcond.databinding.ActivityMainBinding;

public class MainActivity<activity_relatorio> extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private ActivityMainBinding binding;

    private CheckBox cond;
    private Button btnrlt;
    private Intent rltInt;

    private static void onClick(View view) {

    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.titulo;
        tv.setText(stringFromJNI());

        /*btnrlt =  (Button) findViewById(R.id.relat);

        btnrlt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                btnrltRelatorio();
            }
        });*/

        cond= findViewById(R.id.cond);
    }

    /*private void btnrltRelatorio() {
        startActivity(new Intent(MainActivity.class, activity_relatorio.this));
    }*/

    public void onButtonLigarClicked(View view) {
        cond.setText("Ligado");
        cond.setChecked(true);
    }

    public void onButtonDesligadoClicked(View view){
        cond.setText("Desligado");
        cond.setChecked(false);
    }

    public void onButtonSairClicked (View view) {
        // TODO Auto-generated method stub
        finish();
        System.exit(0);
    }

    public void onButtonRelatorioClicked(View view) {
        /*rltInt = Intent(getApplicationContext(), activity_relatorio)
        startActivity(rltInt);*/
        startActivity(new Intent(MainActivity.this, Relatorio.class));
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}