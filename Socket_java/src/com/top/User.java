package com.top;

import java.io.DataOutputStream;
import java.net.Socket;
import java.util.HashMap;
import java.util.Iterator;

public class User {
    HashMap<String, DataOutputStream> clientMap = new HashMap<String,DataOutputStream>();
    public synchronized void addClient(String name, Socket socket){
        try{
            sendMessage(name+"님이 입장하셨습니다.","Server");
            clientMap.put(name, new DataOutputStream(socket.getOutputStream()));
            System.out.println("채팅 참여 인원 : " + clientMap.size());
        }catch (Exception e){ }
    }
    public synchronized void removeClient(String name){
        try{
            clientMap.remove(name);
            sendMessage(name + " 퇴장하셨습니다.","Server");
            System.out.println("채팅 참여 인원 : " + clientMap.size());
        }catch (Exception e){}
    }
    public synchronized void sendMessage(String msg, String name) throws Exception {
        Iterator iterator = clientMap.keySet().iterator();
        while(iterator.hasNext()){
            String clientname = (String)iterator.next();
            clientMap.get(clientname).writeUTF("[ " + name + " ] : " + msg);
        }
    }
}
