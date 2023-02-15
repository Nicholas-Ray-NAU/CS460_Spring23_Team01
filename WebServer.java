package web;

import java.io.*;
import java.net.*;
import java.util.*;

import java.util.logging.Level;
import java.util.logging.Logger;


class WebServer {

    // takes idle worker threads
    static Vector threads = new Vector();

    // web servers port it is listening on
    static int port = 23657;

    // max # worker threads
    static int workers = 5;
    
    // Server socket
    private static ServerSocket serverSocket;
    Socket clientSocket = null;

    // default constructor
    WebServer() {}

    public WebServer(int port) {
        try {
            WebServer.port = port;
            WebServer.serverSocket = new ServerSocket(port);
        } catch (IOException ex) {
            Logger.getLogger(WebServer.class.getName()).log(Level.SEVERE, null, ex);
            System.err.println("Error starting server on port " + port);
            System.exit(1);
        }

        WebServer.port = port;
    }
     
    synchronized void workerHasStarted() {
        notify();
    }

    synchronized void run() throws Exception {

        /* start worker threads */
        for (int i = 0; i < workers; ++i) {
            Worker worker = new Worker(this, "worker #" + i);
            wait();
            threads.addElement(worker);
        }

        Worker worker = null;

        System.out.println("Start server loop");
        while (true) {
            clientSocket = serverSocket.accept();

            synchronized (threads) {
                if (threads.isEmpty()) {
                    worker = new Worker(this, "additional worker");
                    wait();
                    worker.setSocket(clientSocket);
                } else {
                    worker = (Worker) threads.elementAt(0);
                    threads.removeElementAt(0);
                    worker.setSocket(clientSocket);
                }
            }
        }

    }

    public static void main(String[] args) throws Exception {

        (new WebServer(port)).run();

    }
}
