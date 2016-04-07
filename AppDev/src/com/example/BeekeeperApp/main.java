package com.example.BeekeeperApp;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

/**
 * Created by brian on 15/03/2016.
 */
public class main extends Activity {

    private Button connectButton;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        connectButton = (Button)findViewById(R.id.button);
        connectButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                ConnectivityManager connManager = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
                NetworkInfo networkInfo = connManager.getActiveNetworkInfo();
                if(networkInfo != null && networkInfo.isConnected()){
                    Toast.makeText(getApplicationContext(),"Connected",Toast.LENGTH_LONG).show();
                }
                else
                    Toast.makeText(getApplicationContext(),"Not Connected",Toast.LENGTH_LONG).show();
                //String url = "http://10.12.24.8";
            }
        });
    }
}