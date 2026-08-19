class Reader implements Runnable {
    Buff b;
    Reader(Buff t) {
        b = t;
    }
    public void run() {
        for(int i=0;i<10;i++) {
            b.read();
        }
    }
}
class Writer implements Runnable {
    Buff b;
    Writer(Buff t) {
        b = t;
    }
    public void run() {
        for(int i=0;i<10;i++) {
            b.write();
        }
    }
}
class Buff {
    volatile boolean available = false;
    int data = 0;
    public void read() {
        while(!available) {
            try {
                System.out.println("waiting to read");
            }
            catch(Exception e) {

            }
        }
        System.out.println(data);
        available = false;
    }
    public void write() {
        while(available) {
            try {
                System.out.println("waiting to write");
            }
            catch(Exception e) {

            }
        }
        data++;
        available = true;
    }
}
public class ThreadPolling {
    public static void main(String[] args) throws Exception {
        Buff b = new Buff();
        Thread rt = new Thread(new Reader(b));
        rt.start();
        Thread wt = new Thread(new Writer(b));
        wt.start();
        rt.join();
        wt.join();
    }
}