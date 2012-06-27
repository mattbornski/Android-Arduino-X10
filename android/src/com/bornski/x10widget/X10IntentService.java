package com.bornski.x10widget;

import java.io.IOException;

import javax.jmdns.JmDNS;
import javax.jmdns.ServiceEvent;
import javax.jmdns.ServiceInfo;
import javax.jmdns.ServiceListener;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;

import android.app.IntentService;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.wifi.WifiManager.MulticastLock;
import android.util.Log;

public class X10IntentService extends IntentService {
	/** 
	 * A constructor is required, and must call the super IntentService(String)
     * constructor with a name for the worker thread.
	 */
	public X10IntentService() {
		super("X10IntentService");
	}
	
	private JmDNS jmdns = null;
    private ServiceListener listener = null;
    private ServiceInfo serviceInfo;
	
	/**
	 * The IntentService calls this method from the default worker thread with
	 * the intent that started the service. When this method returns, IntentService
	 * stops the service, as appropriate.
	 */
	@Override
	protected void onHandleIntent(Intent intent) {
		android.net.wifi.WifiManager wifi = (android.net.wifi.WifiManager) getSystemService(android.content.Context.WIFI_SERVICE);
        MulticastLock lock = wifi.createMulticastLock("mylockthereturn");
        lock.setReferenceCounted(true);
        lock.acquire();
        try {
            JmDNS jmdns = JmDNS.create();
            jmdns.getHostName()
            jmdns.addServiceListener(type, listener = new ServiceListener() {
                @Override
                public void serviceResolved(ServiceEvent ev) {
                	this.toggle(ev.getInfo().getQualifiedName());
                }
            });
        } catch (IOException e) {
            e.printStackTrace();
            return;
        }
        // else this.toggle("192.168.10.107");
	}
	
	static final String ON = "on";
    static final String OFF = "off";

    private String mCurStatus;
	
	private void toggle(String host) {
		SharedPreferences mPrefs = getSharedPreferences("x10", MODE_PRIVATE);
        mCurStatus = mPrefs.getString("last_status", OFF);
        
        String nextStatus = mCurStatus.matches(ON) ? OFF : ON;
        String url = "http://" + host + "/N/2/" + nextStatus;
        Log.d("x10", url);
        
		HttpClient httpclient = new DefaultHttpClient();
        HttpPost httppost = new HttpPost(url);
        try {
			@SuppressWarnings("unused")
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
}