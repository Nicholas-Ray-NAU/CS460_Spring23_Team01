import java.net.Socket;
import java.io.IOException;

import java.io.DataInputStream;
import java.io.DataOutputStream;

import java.util.Scanner;

public class javaClient {

    public static void main(String args[]) {
        Socket clientSocket = null;
        DataInputStream fromServer = null;
        DataOutputStream toServer = null;
        Boolean keepGoing = true;
        int resultInt = 0;
        Scanner input = new Scanner(System.in);
        int inputNumber;

        try {
            clientSocket = new Socket("127.0.0.1", 23657);

            fromServer = new DataInputStream(clientSocket.getInputStream());
            toServer = new DataOutputStream(clientSocket.getOutputStream());
        } catch(IOException e) {
            System.err.println("Unable to connect to server");
            keepGoing = false;
        }

        System.out.print("Input number: ");
        inputNumber = input.nextInt();

        try {
            toServer.writeByte((byte)inputNumber);
        } catch(IOException e) {
            System.err.println("Error reading byte from server");
        }

        try {
            resultInt = (int)fromServer.readByte();
            System.out.println("Number of moves to get to 1: " + resultInt);
        } catch(IOException e) {
            System.err.println("Error reading byte from server");
        }

        System.out.println(); // Print new line for better spacing

        try{
            clientSocket.close();
        } catch(IOException e) {
            System.err.println("Error closing socket");
        }
    }
}