import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class HTTPServer {
    static final int PORT = 5046;
    private static BufferedWriter logWriter;

    public static void main(String[] args) throws IOException {
        File logFile = new File("logs/access_log.txt");
        if (!logFile.exists()) {
            logFile.getParentFile().mkdirs();
            logFile.createNewFile();
        }
        logWriter = new BufferedWriter(new FileWriter(logFile, true));
        ServerSocket serverSocket = new ServerSocket(PORT);
        System.out.println("Server started.\nListening for connections on port : " + PORT + " ...\n");

        while (true) {
            Socket clientSocket = serverSocket.accept();
            System.out.println("New client connected from " + clientSocket.getInetAddress().getHostAddress() + ":" + clientSocket.getPort());
            new Thread(new RequestHandler(clientSocket, logWriter)).start();
        }
    }
}
