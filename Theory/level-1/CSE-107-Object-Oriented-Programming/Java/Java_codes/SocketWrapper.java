import java.net.Socket;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

public class SocketWrapper {
    private Socket socket;
    private ObjectInputStream ois;
    private ObjectOutputStream oos;
    public SocketWrapper(Socket s) throws IOException {
        this.socket = s;
        oos = new ObjectOutputStream(socket.getOutputStream());
        ois = new ObjectInputStream(socket.getInputStream());
    }
    public void write(Object o) throws IOException {
        oos.writeUnshared(o);
    }
    public Object read() throws IOException,ClassNotFoundException {
        return ois.readUnshared();
    }
}