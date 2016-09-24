package cn.itcast.driver;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class LoginActivity extends AppCompatActivity {

    protected static final String tag = "LoginActivityTag";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        Button btn = (Button)findViewById(R.id.login);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Login();
                Log.e(tag, "login end xxxxxxxxxx");
            }
        });
    }

    protected void Login(){

        EditText userEdit = (EditText)findViewById(R.id.username);
        EditText passEdit = (EditText)findViewById(R.id.password);

        String username = userEdit.getText().toString();
        String password = passEdit.getText().toString();

        if(username.length() == 0 || password.length() == 0) {
            Log.e(tag, "username or password is null");
            return ;
        }

        boolean ret = Jni.getInstance().Login(username, password);
        if(ret )
        {
            Log.e(tag, "login ok");
        }
        else
            Log.e(tag, "login error");

    }
}
