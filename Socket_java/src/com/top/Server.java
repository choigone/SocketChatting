package com.top;

import java.net.ServerSocket;
import java.net.Socket;

public class Server {
    public static void main(String[] args) throws Exception {
        Socket socket = null;
        User user = new User();
        ServerSocket serverSocket = null;

        int count = 0;
        Thread thread[] = new Thread[10];

        try{
            serverSocket = new ServerSocket(10002);
            while(true){
                socket = serverSocket.accept();
                thread[count] = new Thread(new Receiver(user,socket));
                thread[count].start();
                count++;
            }
        }
        catch (Exception e){

        }
    }
}
