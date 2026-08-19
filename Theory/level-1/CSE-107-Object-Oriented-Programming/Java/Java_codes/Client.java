import java.util.Scanner;
import java.net.Socket;
import java.io.Serializable;

class Data implements Serializable {
    String name = "";
    String msg = "";    
}

class ClientReadThread extends Thread {
    SocketWrapper sw;
    ClientReadThread(Socket s) throws Exception {
        System.out.println("Wrapping socket");
        sw = new SocketWrapper(s);
        System.out.println("Socket wrapped");
    }
    public void run() {
        while(true) {
            try {
                Object o = sw.read();
                if(o instanceof Data) {
                    Data d = (Data)o;
                    System.out.println(d.name + ": " + d.msg);
                }
            }
            catch(Exception e) {
                break;
            }
        }
    }
}
public class Client {
    static Scanner sc = new Scanner(System.in);
    static Socket socket;
    public static void main(String[] args) throws Exception {
        socket = new Socket("127.0.0.1", 5000);
        System.out.println("Connected");
        var t = new ClientReadThread(socket);
        t.start();
        String name = sc.nextLine();
        while(true) {
            String msg = sc.nextLine();
            Data d = new Data();
            d.name = name;
            d.msg = msg;
            t.sw.write(d);
        }
    }
}