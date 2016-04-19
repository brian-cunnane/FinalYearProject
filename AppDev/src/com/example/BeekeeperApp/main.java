package com.example.BeekeeperApp;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import org.apache.http.client.ClientProtocolException;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.HashMap;

/**
 * Created by brian on 15/03/2016.
 */
public class main extends Activity {

    private Button connectButton;
    private TextView textView1;
    private JSONObject jsonResult;
    private String jsonString;

    public HashMap<String,String> inputMap;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        inputMap = new HashMap<String, String>();
        connectButton = (Button)findViewById(R.id.button);
        textView1 = (TextView)findViewById(R.id.textView1);

        connectButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                ConnectivityManager connManager = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
                NetworkInfo networkInfo = connManager.getActiveNetworkInfo();
                if(networkInfo != null && networkInfo.isConnected()){
                    Toast.makeText(getApplicationContext(),"Connected",Toast.LENGTH_LONG).show();
                    JSONObject test = new JSONObject();
                    try {
                        test.put("test","1");
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }

                    //String URL = "http://10.12.11.185:8080/test";
                    new HttpAsyncTask().execute("http://10.12.11.185:8080/android?test=test",test.toString());
                }
                else
                    Toast.makeText(getApplicationContext(),"Not Connected",Toast.LENGTH_LONG).show();
                //String url = "http://10.12.24.8";
            }
        });
    }
    private class HttpAsyncTask extends AsyncTask<String, Void, String>{

        @Override
        protected String doInBackground(String... params) {
           // String URL = "http://10.12.11.185:8080/test";
            String jsonString = "";
            try {
                jsonString = HttpUtils.urlContentPost(params[0],"test",params[1]);
                Log.d("url",jsonString.toString());
            } catch (IOException e) {
                e.printStackTrace();
            }
            return jsonString;
        }

        protected void onPostExecute(String result){
            JSONArray jsonArray = new JSONArray();
            JSONObject jsonResult = new JSONObject();
            try{
                jsonArray = new JSONArray(result);
                textView1.setText(jsonArray.toString());
                //jsonResult = new JSONObject(result);
            }catch(JSONException JE){
                JE.printStackTrace();
            }
            try{
                String temp = jsonResult.getString("temperature");
                textView1.setText(textView1.getText() + "\n" + temp);
            }catch(JSONException JE){JE.printStackTrace();}
        }
    }
}