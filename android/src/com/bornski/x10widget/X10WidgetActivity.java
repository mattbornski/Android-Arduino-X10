package com.bornski.x10widget;

import java.io.IOException;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;

import com.bornski.x10widget.R;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;

public class X10WidgetActivity extends Activity {
	static final String ON = "on";
    static final String OFF = "off";

    private SharedPreferences mPrefs;
    private String mCurStatus;
	
	private void on() {
		SharedPreferences mPrefs = getSharedPreferences("x10", MODE_PRIVATE);
        mCurStatus = mPrefs.getString("last_status", OFF);
        
        String nextStatus = mCurStatus.matches(ON) ? OFF : ON;
        String url = "http://192.168.10.107/N/2/" + nextStatus;
        Log.d("x10", url);
        
		HttpClient httpclient = new DefaultHttpClient();
        HttpPost httppost = new HttpPost(url);
        try {
			HttpResponse response = httpclient.execute(httppost);
		} catch (ClientProtocolException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			Log.d("x10", e.getMessage());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			Log.d("x10", e.getMessage());
		}
        
        SharedPreferences.Editor ed = mPrefs.edit();
        ed.putString("last_status", nextStatus);
        ed.commit();
	}
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        this.on();
    }
    
    /** Called when the activity is brought to the foreground. */
    public void onResume(Bundle savedInstanceState) {
    	this.on();
    }
}