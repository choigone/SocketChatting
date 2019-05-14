package com.top;

import java.io.*;
import java.net.Socket;

public class Client {
    public static void main(String[] arg)
    {
        Socket socket = null;
        DataInputStream in = null;
        BufferedReader in2 = null;

        DataOutputStream out = null;

        try {

            socket = new Socket("127.0.0.1",10002);

            in = new DataInputStream(socket.getInputStream());
            in2 = new BufferedReader(new InputStreamReader(System.in));
            out = new DataOutputStream(socket.getOutputStream());

            System.out.print("닉네임을 입력해주세요 : ");
            String data = in2.readLine();
            out.writeUTF(data);
            Thread th = new Thread(new Send(out));
            th.start();
        }catch(IOException e) {}
        try {
            while(true)
            {
                String str2 = in.readUTF();
                System.out.println(str2);
            }
        }catch(IOException e) {}
    }
}