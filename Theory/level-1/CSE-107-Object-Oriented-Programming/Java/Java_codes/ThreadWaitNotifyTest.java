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
    boolean available = false;
    int data = 0;
    public synchronized void read() {
        while(!available) {
            try {
                wait();
            }
            catch(Exception e) {

            }
        }
        System.out.println(data);
        available = false;
        notifyAll();
    }
    public synchronized void write() {
        while(available) {
            try {
                wait();
            }
            catch(Exception e) {

            }
        }
        data++;
        available = true;
        notifyAll();
    }
}
public class ThreadWaitNotifyTest {
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