package com.example.BeekeeperApp;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import org.apache.http.client.ClientProtocolException;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.HashMap;

/**
 * Created by brian on 15/03/2016.
 */
public class main extends Activity {

    private Button connectButton;
    private JSONObject jsonResult;
    private String jsonString;

    public HashMap<String,String> inputMap;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        inputMap = new HashMap<String, String>();
        connectButton = (Button)findViewById(R.id.button);

        connectButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                ConnectivityManager connManager = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
                NetworkInfo networkInfo = connManager.getActiveNetworkInfo();
                if(networkInfo != null && networkInfo.isConnected()){
                    Toast.makeText(getApplicationContext(),"Connected",Toast.LENGTH_LONG).show();
                    String URL = "http://10.12.16.116:8080/beekeeper/test";

                    JSONObject jsonInput = new JSONObject(inputMap);
                    new Thread(new Runnable(){
                        @Override
                        public void run(){
                            try{
                                String jsonString = HttpUtils.urlContentPost(URL,"Query",jsonInput.toString());
                                jsonResult = new JSONObject(jsonString);
                            }catch(ClientProtocolException CPE){Toast.makeText(getApplicationContext(),"ClientProtocolException",Toast.LENGTH_LONG).show();}
                            catch(IOException IOE){Toast.makeText(getApplicationContext(),"IO Exception",Toast.LENGTH_LONG).show();}
                            catch(JSONException JSE){Toast.makeText(getApplicationContext(),"JSON Exception",Toast.LENGTH_LONG).show();}
                        }
                    }).start();
                }
                else
                    Toast.makeText(getApplicationContext(),"Not Connected",Toast.LENGTH_LONG).show();
                //String url = "http://10.12.24.8";
            }
        });
    }
}