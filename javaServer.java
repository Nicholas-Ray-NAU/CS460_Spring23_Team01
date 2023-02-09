import java.io.DataInputStream;
import java.io.DataOutputStream;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.InetAddress;

import java.util.logging.Level;
import java.util.logging.Logger;

public class EchoServer extends Thread implements javaServerConstants {

    private static ServerSocket serverSocket;
    private static int port;
    Socket clientSocket = null;

    public EchoServer(Socket clientSocket) {
        this.clientSocket = clientSocket;
    }

    public EchoServer(int port) {
        try {
            EchoServer.port = port;
            EchoServer.serverSocket = new ServerSocket(port);
        } catch (IOException ex) {
            Logger.getLogger(EchoServer.class.getName()).log(Level.SEVERE, null, ex);
            System.err.println("Error starting server on port " + port);
            System.exit(1);
        }

        EchoServer.port = port;
    }

    public void runServerLoop() throws IOException {

        System.out.println("Daytime server is listening on port "+LISTEN_PORT);

        while (true) {

            try {
                (new EchoServer(serverSocket.accept())).start();
            } catch (IOException exception) {
                System.err.println("Error handing client to a new thread");
            }
        }
    }

    public static void main(String args[]) throws Exception {
        // create instance of echo server
        // note that hardcoding the port is bad, here we do it just for simplicity reasons
        EchoServer echoServer = new EchoServer(LISTEN_PORT);

        // fire up server loop
        echoServer.runServerLoop();
    }

    @Override
    public void run() {

        // Handle every client in parallel
        DataInputStream fromClient = null;
        DataOutputStream toClient = null;
        DataInputStream fromNISTServer = null;
        Socket socketToNIST = null;

        int charFromNISTServer = 0;
        boolean keepGoing = true;

        // show that we are connected to client
        System.out.println("\nClient connected ...");

        // Get streams to and from the NIST server (time.nist.gov)
        try {
            // Connect to the NIST server
            socketToNIST = connectToNISTServer();

            // Set up input/output streams to/from the NIST server
            toClient = new DataOutputStream(clientSocket.getOutputStream());
            fromNISTServer = new DataInputStream(socketToNIST.getInputStream());
        } catch (IOException e) {
            System.err.println("Error opening stream to NIST server");
            return;
        }

        // Now transfer bytes given by NIST to the client
        while (keepGoing) {
            try {
                // Read the next byte given to us by NIST
                charFromNISTServer = fromNISTServer.readByte();
                System.out.print((char)charFromNISTServer);
            } catch (IOException e) {
                System.err.println("Error reading character from NIST");
                return;
            }

            try {
                // Send the current byte of information to the client
                toClient.writeByte((char)charFromNISTServer);
            } catch (IOException e) {
                System.err.println("Error writing character to client");
                return;
            }

            if (charFromNISTServer == END_MESSAGE_CHAR) {
                keepGoing = false;
            }
        }

        try {
            socketToNIST.close();
            clientSocket.close();
            System.out.println("\nTime information sent, booting client...");
        } catch (IOException e) {
            System.err.println("Error closing socket to client");
        }
    }

    public Socket connectToNISTServer() {
        String NISTserverAddressString;
        InetAddress NISTserverAddress;
        Socket clientSocketToNIST = null;

        try {
            NISTserverAddress = InetAddress.getByName(NIST_HOST_NAME);
            NISTserverAddressString = NISTserverAddress.getHostAddress();

            // Create new client socket and connect to the server
            clientSocketToNIST = new Socket(NISTserverAddressString, NIST_PORT);
        } catch(IOException e) {
            System.err.println("Error connecting to NIST server");
        }

        return clientSocketToNIST;
    }
}
