import java.net.ServerSocket;
import java.net.Socket;
import java.io.Serializable;

class Data implements Serializable {
    String name = "";
    String msg = "";    
}
class ReadThread extends Thread {
    SocketWrapper sw;
    ReadThread(Socket cs) throws Exception {
        sw = new SocketWrapper(cs);
    }
    public void run() {
        while(true) {
            try {
                Object o = sw.read();
                if(o instanceof Data) {
                    Data t = (Data)o;
                    Server.broadcast(t);
                }
            }
            catch(Exception e) {
                break;
            }
        }
    }
}
public class Server {
    static ReadThread threads[] = new ReadThread[1000];
    static int id = 0;
    static void broadcast(Data d) throws Exception {
        for(int i=0;i<id;i++) if(threads[i].isAlive()) threads[i].sw.write(d);
    }
    public static void main(String[] args) throws Exception {
        ServerSocket ss = new ServerSocket(5000);
        while (true) {
            Socket cs = ss.accept();
            threads[id] = new ReadThread(cs);
            threads[id].start();
            id++;
        }
    }
}