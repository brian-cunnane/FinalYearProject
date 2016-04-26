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
import android.widget.*;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.MarkerView;
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
    private Button clearButton;
    private TextView textView1;
    private LineChart graph;
    private CheckBox temperatureBox;
    private CheckBox weightBox;
    private CheckBox humidityBox;
    private Spinner spinner;
    private ArrayList times = new ArrayList();
    private ArrayList dates = new ArrayList();
    private String hive;

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
        clearButton = (Button)findViewById(R.id.Clear);
        temperatureBox = (CheckBox)findViewById(R.id.TemperatureBox);
        weightBox = (CheckBox)findViewById(R.id.WeightBox);
        humidityBox = (CheckBox)findViewById(R.id.HumidityBox);
        textView1 = (TextView)findViewById(R.id.textView1);
        spinner = (Spinner)findViewById(R.id.spinner);
        spinner.setOnItemSelectedListener(new CustomOnItemSelectedListener());


        graph = (LineChart)findViewById(R.id.graph);
        graph.setOnChartValueSelectedListener(this);
        graph.setDrawGridBackground(true);
        graph.setDescription("Beekeeper App Demo");
        graph.setData(new LineData());


        CustomMarkerView marker = new CustomMarkerView(this, R.layout.marker);
        graph.setMarkerView(marker);
        graph.invalidate(); //refresh graph

            connectButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                ConnectivityManager connManager = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
                NetworkInfo networkInfo = connManager.getActiveNetworkInfo();
                if(networkInfo != null && networkInfo.isConnected()){
                    Toast.makeText(getApplicationContext(),"Connected",Toast.LENGTH_LONG).show();

                    //college ip http://10.12.11.185:8080/beekeeper/android;
                    //home ip http://192.168.0.12:8080/android;
                    if(hive.compareTo("Hive1")==0)
                        new HttpAsyncTask().execute("http://192.168.0.13:8080/android?Hive1");
                    else if(hive.compareTo("Hive2")==0)
                        new HttpAsyncTask().execute("http://192.168.0.13:8080/android?Hive2");
                }
                else
                    Toast.makeText(getApplicationContext(),"Not Connected",Toast.LENGTH_LONG).show();
            }
        });
        clearButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                removeDataSets();
            }
        });
    }

    @Override
    public void onValueSelected (Entry e, int dataSetIndex, Highlight h) {

    }

    @Override
    public void onNothingSelected() {

    }
    public class CustomOnItemSelectedListener implements AdapterView.OnItemSelectedListener{

        @Override
        public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
            hive = parent.getItemAtPosition(position).toString();
        }

        @Override
        public void onNothingSelected(AdapterView<?> parent) {

        }
    }
    private class HttpAsyncTask extends AsyncTask<String, Void, String>{

        @Override
        protected String doInBackground(String... params) {
            String jsonString = "";
            try {
                jsonString = HttpUtils.urlContentPost(params[0]);
                Log.d("url",jsonString.toString());
            } catch (IOException e) {
                e.printStackTrace();
            }
            return jsonString;
        }

        protected void onPostExecute(String result){
            ArrayList <Integer>temps = new ArrayList<Integer>();
            ArrayList <Integer>hums = new ArrayList<Integer>();
            ArrayList <Double>weights = new ArrayList<Double>();

            JSONArray jsonArray;
            JSONObject jsonObject = new JSONObject();

            try {
                jsonArray = new JSONArray(result);
                for(int i = 0; i < jsonArray.length(); i ++){
                    jsonObject = jsonArray.getJSONObject(i);
                    dates.add(jsonObject.getString("date"));
                    temps.add(jsonObject.getInt("temperature"));
                    hums.add(jsonObject.getInt("humidity"));
                    weights.add(jsonObject.getDouble("weight"));
                    times.add(jsonObject.getString("time"));
                }
            } catch (JSONException e) {
                e.printStackTrace();
            }
            Log.d("values",temps.toString());
            if(temperatureBox.isChecked()) {
                addDataSet(temps, "Temperature " + hive,dates, tempColour);
            }

            if(humidityBox.isChecked()) {
                addDataSet(hums, "Humidity " + hive,dates, humidityColour);
            }

            if(weightBox.isChecked()) {
                addDataSet(weights, "Weight " + hive,dates, weightColour);
            }

        }
    }

    private void addDataSet(ArrayList data, String name, ArrayList dates, int[] colour) {
        LineData values = graph.getData();

        Log.d("values",data.toString());
        if(values != null){
            ArrayList<Entry> y = new ArrayList<Entry>(); // array list for y values
            if(values.getXValCount() == 0){
                for(int i = 0; i< data.size(); i ++)
                    values.addXValue("" + dates.get(i).toString()); // add divisions to x axis
            }
            try {
                if(data.get(0).toString().contains(".")){
                    for (int i = 0; i < data.size(); i++) {
                        y.add(new Entry((float) Double.parseDouble(data.get(i).toString()), i));
                        Log.d("y", y.toString());
                    }
                }
                else {
                    for (int i = 0; i < data.size(); i++) {
                        y.add(new Entry(Integer.parseInt(data.get(i).toString()), i));
                        Log.d("y", y.toString());
                    }
                }
            }catch(NumberFormatException NFE){NFE.printStackTrace();}
            LineDataSet set = new LineDataSet(y,name);
            set.setColors(colour);
            set.setLineWidth(2.0f);
            set.setCircleRadius(4.0f);
            set.setCircleColors(colour);
            set.setValueTextSize(10f);
            set.setDrawValues(false);
            values.addDataSet(set);

            graph.fitScreen();
            graph.notifyDataSetChanged();
            graph.animateX(1500);
            graph.invalidate();
        }
    }
    public String getTime(int index){
        return times.get(index).toString();
    }

    public String getDate(int index){
        return dates.get(index).toString();
    }
    private void removeDataSets(){
        LineData values = graph.getData();
        if(values != null){
            //values.removeDataSet(values.getDataSetByIndex(values.getDataSetCount()-1));
            values.clearValues();
        }
        graph.notifyDataSetChanged();
        graph.invalidate();
    }
    public class CustomMarkerView extends MarkerView{
        private TextView textViewMarker;
        public CustomMarkerView(Context context,int layoutResource){
            super(context,layoutResource);
            textViewMarker = (TextView)findViewById(R.id.textViewMarker);
        }
        @Override
        public void refreshContent(Entry e, Highlight highlight) {
            int index = e.getXIndex();
            textViewMarker.setText("" + e.getVal()+"\n\n" + ""+ getTime(index)+ "\n\n" + "" +getDate(index));
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