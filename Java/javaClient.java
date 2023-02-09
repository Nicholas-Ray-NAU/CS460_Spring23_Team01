import java.net.ServerSocket;
import java.net.Socket;
import java.io.IOException;
import java.net.InetAddress;

import java.io.DataInputStream;
import java.io.DataOutputStream;

public class javaClient implements javaClientConstants {

    public static void main(String args[]) {
        Socket clientSocket = null;
        DataInputStream fromServer = null;
        DataOutputStream toServer = null;
        Boolean keepGoing = true;
        int currChar = 0;
        InetAddress hostAddress = null;
        String serverIP = "";

        try {
            hostAddress = InetAddress.getByName(HOST_NAME);
            serverIP = hostAddress.getHostAddress();

            clientSocket = new Socket(serverIP, CONNECTION_PORT);

            fromServer = new DataInputStream(clientSocket.getInputStream());
            toServer = new DataOutputStream(clientSocket.getOutputStream());
        } catch(IOException e) {
            System.err.println("Unable to connect to server");
            keepGoing = false;
        }

        while(keepGoing) {
            try {
                currChar = fromServer.readByte();
                System.out.print((char)currChar);
            } catch(IOException e) {
                System.err.println("Error reading byte from server");
            }

            if((char)currChar == END_MESSAGE_CHAR) {
                keepGoing = false;
            }
        }
        System.out.println(); // Print new line for better spacing

        try{
            clientSocket.close();
        } catch(IOException e) {
            System.err.println("Error closing socket");
        }
    }
}