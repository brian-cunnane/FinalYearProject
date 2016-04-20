package com.example.BeekeeperApp;

import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.MarkerView;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.highlight.Highlight;
import com.github.mikephil.charting.listener.OnChartValueSelectedListener;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

/**
 * Created by brian on 15/03/2016.
 */
public class main extends Activity implements OnChartValueSelectedListener {

    private Button connectButton;
    private TextView textView1;
    private LineChart graph;

    private final int[] tempColour = {
            Color.rgb(255, 0, 0)
    };
    private final int[] weightColour = {
            Color.rgb(255, 151, 0)
    };
    private final int[] humidityColour = {
            Color.rgb(0, 255, 255)
    };

    public HashMap<String,String> inputMap;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        inputMap = new HashMap<String, String>();
        connectButton = (Button)findViewById(R.id.button);
        textView1 = (TextView)findViewById(R.id.textView1);

        graph = (LineChart)findViewById(R.id.graph);
        graph.setOnChartValueSelectedListener(this);
        //graph.setBackgroundColor(255);
        graph.setDrawGridBackground(true);
        graph.setDescription("Test");
        graph.setData(new LineData());

        CustomMarkerView marker = new CustomMarkerView(this, R.layout.marker);
        graph.setMarkerView(marker);
        graph.invalidate();

        connectButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                ConnectivityManager connManager = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
                NetworkInfo networkInfo = connManager.getActiveNetworkInfo();
                if(networkInfo != null && networkInfo.isConnected()){
                    Toast.makeText(getApplicationContext(),"Connected",Toast.LENGTH_LONG).show();

                    //college id http://10.12.11.185:8080/android;
                    //home id http://192.168.0.12:8080/android;
                    new HttpAsyncTask().execute("http://192.168.0.22:8080/beekeeper/android?test=test");
                }
                else
                    Toast.makeText(getApplicationContext(),"Not Connected",Toast.LENGTH_LONG).show();
            }
        });
    }

    @Override
    public void onValueSelected (Entry e, int dataSetIndex, Highlight h) {

    }

    @Override
    public void onNothingSelected() {

    }

    private class HttpAsyncTask extends AsyncTask<String, Void, String>{

        @Override
        protected String doInBackground(String... params) {
            String jsonString = "";
            try {
                jsonString = HttpUtils.urlContentPost(params[0],"test");
                Log.d("url",jsonString.toString());
            } catch (IOException e) {
                e.printStackTrace();
            }
            return jsonString;
        }

        protected void onPostExecute(String result){
            ArrayList temps = new ArrayList();
            ArrayList hums = new ArrayList();
            ArrayList weights = new ArrayList();
            ArrayList times = new ArrayList();
            ArrayList dates = new ArrayList();

            JSONArray jsonArray;
            JSONObject jsonObject = new JSONObject();
            try {
                jsonArray = new JSONArray(result);
                for(int i = 0; i < jsonArray.length(); i ++){
                    jsonObject = jsonArray.getJSONObject(i);
                    dates.add(jsonObject.getString("date"));
                    temps.add(jsonObject.getInt("temperature"));
                    hums.add(jsonObject.getInt("humidity"));
                    weights.add(jsonObject.getInt("weight"));
                    times.add(jsonObject.getString("time"));
                }
               // for(int i = 0; i < dates.size();i ++)
                //textView1.setText(textView1.getText() + dates.get(i).toString() + " ");
                Log.d("values",temps.toString());

               /* XAxis xAxis = graph.getXAxis();
                xAxis.setTextSize(10f);
                for(int i = 0; i < times.size(); i ++)
                    xAxis.setValues(times.get(i).toString());
              */
                addDataSet(temps,"Temperature", tempColour);
                addDataSet(hums, "Humidity", humidityColour);
                addDataSet(weights,"Weight", weightColour);

            } catch (JSONException e) {
                e.printStackTrace();
            }
        }
    }

    private void addDataSet(ArrayList data, String name, int[] colour) {
        LineData values = graph.getData();
        Log.d("values",data.toString());
        if(values != null){
            ArrayList<Entry> y = new ArrayList<Entry>(); // array list for y values
            if(values.getXValCount() == 0){
                for(int i = 0; i < 10; i ++)
                    values.addXValue("" + (i+1));
            }
            try {
                for (int i = 0; i < data.size(); i++) {
                    y.add(new Entry(Integer.parseInt(data.get(i).toString()), i));
                    Log.d("y",y.toString());
                }
            }catch(NumberFormatException NFE){NFE.printStackTrace();}
            LineDataSet set = new LineDataSet(y,name);

            set.setColors(colour);
            set.setLineWidth(2.0f);
            set.setCircleRadius(4.0f);
            set.setCircleColors(colour);
            set.setValueTextSize(10f);

            values.addDataSet(set);
            graph.notifyDataSetChanged();
            graph.invalidate();
        }


    }
    public class CustomMarkerView extends MarkerView{
        private TextView textViewMarker;
        public CustomMarkerView(Context context,int layoutResource){
            super(context,layoutResource);
            textViewMarker = (TextView)findViewById(R.id.textViewMarker);
        }
        @Override
        public void refreshContent(Entry e, Highlight highlight) {
            textViewMarker.setText("" + e.getVal());
        }

        @Override
        public int getXOffset(float xpos) {
            return -(getWidth() / 2);
        }

        @Override
        public int getYOffset(float ypos) {
            return -getHeight();
        }
    }
}