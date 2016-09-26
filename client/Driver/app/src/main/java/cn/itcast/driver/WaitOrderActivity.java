package cn.itcast.driver;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.amap.api.location.AMapLocation;
import com.amap.api.location.AMapLocationClient;
import com.amap.api.location.AMapLocationClientOption;
import com.amap.api.location.AMapLocationListener;
import com.amap.api.maps.AMap;
import com.amap.api.maps.LocationSource;
import com.amap.api.maps.MapView;

public class WaitOrderActivity extends AppCompatActivity {

    protected MapView mapView;
    protected AMap aMap;
    protected LocationSource source;

    // for MyLocation
    protected AMapLocationClient locationClient = null;
    protected LocationSource.OnLocationChangedListener locationSourceListener = null;

    protected Button button;

    protected boolean isStarted = false;

    protected final int STARTORDER = 1;
    protected final int STOPORDER = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_wait_order);

        mapView = (MapView)findViewById(R.id.map);
        mapView.onCreate(savedInstanceState);
        aMap = mapView.getMap();
        initLocationSource();
        aMap.setLocationSource(source);
        aMap.setMyLocationEnabled(true);

        button = (Button)findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ButtonClicked();
            }
        });
    }

    protected void updateGeoInfo(AMapLocation aMapLocation){
        int ret = Jni.getInstance().UpdateGeoInfo(aMapLocation.getLongitude(),
                aMapLocation.getLatitude());
        Log.e(Jni.getInstance().tag, "update geo info  yyyy");
    }

    protected void LocationSourceActivate(){
        if(locationClient == null)
        {
            locationClient = new AMapLocationClient(getApplicationContext());
            AMapLocationClientOption option = new AMapLocationClientOption();
            option.setInterval(2000);
            option.setOnceLocation(false);

            locationClient.setLocationListener(new AMapLocationListener() {
                @Override
                public void onLocationChanged(AMapLocation aMapLocation) {
                    locationSourceListener.onLocationChanged(aMapLocation);
                    if(isStarted)
                        updateGeoInfo(aMapLocation);
                }
            });
        }
        locationClient.startLocation();
    }

    protected void LocationSourceDeactivate(){
        if(locationClient != null)
        {
            locationClient.stopLocation();
            locationClient = null;
        }
        locationSourceListener = null;
    }

    protected void initLocationSource(){
        source = new LocationSource() {
            @Override
            public void activate(OnLocationChangedListener onLocationChangedListener) {
                // start Location
                locationSourceListener = onLocationChangedListener;
                LocationSourceActivate();
            }

            @Override
            public void deactivate() {
                LocationSourceDeactivate();
            }
        };
    }

    protected void start(){
        // start get order
        button.setText("Stop");
        isStarted = true;

        Jni.getInstance().SetStatus(STARTORDER);
    }

    protected void stop(){
        // stop get order
        button.setText("Start");
        isStarted = false;

        Jni.getInstance().SetStatus(STOPORDER);
    }

    protected void ButtonClicked(){
        String text = button.getText().toString();
        if(!isStarted){
            start();
        }
        else{
            stop();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        mapView.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mapView.onDestroy();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mapView.onPause();
    }
}
