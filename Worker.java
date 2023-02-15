package web;

import java.io.*;
import java.net.*;
import java.util.*;

class Worker extends Thread implements HttpConstants {

    /* buffer to use for requests */
    byte[] buffer;
    /* Socket to client we're handling */
    private Socket socket;
    /* WebServer the worker works for */
    WebServer webServer;

    Worker(WebServer webServer, String name) {
        super(name);
        this.webServer = webServer;
        this.start();
    }

    synchronized void setSocket(Socket socket) {
        this.socket = socket;
        notify();
    }

    public synchronized void run() {

        webServer.workerHasStarted();

        while (true) {
            /* nothing to do */
            try {
                wait();
            } catch (InterruptedException e) {
                /* should not happen */
                continue;
            }

            try {
                handleClient();
            } catch (Exception e) {
                e.printStackTrace();
            }
            /* go back in wait queue if there's fewer
             * than numHandler connections.
             */
            Vector pool = webServer.threads;
            synchronized (pool) {
                if (pool.size() >= webServer.workers) {
                    /* too many threads, exit this one */
                    return;
                } else {
                    pool.addElement(this);
                }
            }
        }
    }

    void handleClient() throws IOException {
        InputStream is = new BufferedInputStream(socket.getInputStream());
        PrintStream ps = new PrintStream(socket.getOutputStream());
        int inputNum, numMoves;

        try {
            inputNum = (int) is.read();
            numMoves = collatzConjecture(inputNum);
            ps.write((byte)numMoves);
        } catch(IOException e) {
            System.err.println("Error reading in integer from client.");
        }
        
        socket.close();
    }
    
    int collatzConjecture(int startNumber) {
        int totalMoves = 0;
        
        while(startNumber != 1) {
            if(startNumber % 2 == 0) {
                startNumber /= 2;
            } else {
                startNumber = (startNumber * 3) + 1;
            }
            totalMoves++;
        }
        
        return totalMoves;
    }
}
