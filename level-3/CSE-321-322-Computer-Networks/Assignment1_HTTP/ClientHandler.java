import java.io.*;
import java.net.Socket;

public class ClientHandler implements Runnable {

    private static final String SERVER_ADDRESS = "localhost";
    private static final int SERVER_PORT = 5046;
    private final String filename;

    public ClientHandler(String filename) {
        this.filename = filename;
    }

    public void run() {
        File file = new File(filename);
        if (!file.exists() || !file.isFile()) {
            System.out.println("File does not exist or is not a valid file.");
            return;
        }

        try (Socket socket = new Socket(SERVER_ADDRESS, SERVER_PORT);
             OutputStream outputStream = socket.getOutputStream();
             InputStream fileInputStream = new FileInputStream(file);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {
            PrintWriter out = new PrintWriter(outputStream, true);
            out.println("UPLOAD " + filename);


            byte[] buffer = new byte[1024];
            int bytesRead;
            while ((bytesRead = fileInputStream.read(buffer)) != -1) {
                    outputStream.write(buffer, 0, bytesRead);
            }
            outputStream.flush();
            socket.shutdownOutput();
            String responseLine;
            while ((responseLine = in.readLine()) != null) {
                System.out.println(responseLine);
                if (responseLine.isEmpty()) {
                    break;
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}