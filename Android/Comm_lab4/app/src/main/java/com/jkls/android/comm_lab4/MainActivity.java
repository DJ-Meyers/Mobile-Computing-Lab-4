package com.jkls.android.comm_lab4;

import org.json.JSONException;
import org.json.JSONObject;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;
import com.loopj.android.http.RequestParams;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final Button button = (Button) findViewById(R.id.btn1);
        final Button button2 = (Button) findViewById(R.id.btn2);
        final Button button3 = (Button) findViewById(R.id.btn3);
        final Button button4 = (Button) findViewById(R.id.btn4);

        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Perform action on click
                RequestParams params = new RequestParams();
                params.put("region", "1");
                final String num = "1";
                invokeWS(params, num);
            }
        });
        button2.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Perform action on click
                RequestParams params = new RequestParams();
                params.put("region", "2");
                final String num = "2";
                invokeWS(params,num);
            }
        });
        button3.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Perform action on click
                RequestParams params = new RequestParams();
                params.put("region", "3");
                final String num = "3";
                invokeWS(params,num);
            }
        });
        button4.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Perform action on click
                RequestParams params = new RequestParams();
                params.put("region", "4");
                final String num = "4";
                invokeWS(params,num);
            }
        });
    }

    public void invokeWS(RequestParams params, String num){
        // Show Progress Dialog
       // prgDialog.show();
        // Make RESTful webservice call using AsyncHttpClient object
        AsyncHttpClient client = new AsyncHttpClient();
        final String URL = "http://192.168.0.104:8080/incrementPopulation/" + num;


        client.get(URL,params ,new AsyncHttpResponseHandler() {
            // When the response returned by REST has Http response code '200'
            @Override
            public void onSuccess( String response) {
                // Hide Progress Dialog
                //prgDialog.hide();
                Toast.makeText(getApplicationContext(), "population: " + response, Toast.LENGTH_SHORT).show();
            }
            @Override
            public void onFailure(int statusCode, Throwable error,
                                  String content) {
                // Hide Progress Dialog
                //prgDialog.hide();
                // When Http response code is '404'

                if(statusCode == 404){
                    Toast.makeText(getApplicationContext(), "Requested resource not found", Toast.LENGTH_LONG).show();
                }
                // When Http response code is '500'
                else if(statusCode == 500){
                    Toast.makeText(getApplicationContext(), "Something went wrong at server end", Toast.LENGTH_LONG).show();
                }
                // When Http response code other than 404, 500
                else{
                   String s_code =  Integer.toString(statusCode);
                    Toast.makeText(getApplicationContext(),URL , Toast.LENGTH_LONG).show();
                }
            }
        });
    }




}
