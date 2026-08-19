interface printable {
    void print();
}
class Counter implements printable {
    final Object lock = new Object();
    int count = 0;
    public void print() {
        System.out.println("Count = " + count);
    }
    Counter() {

    }
    Counter(int initial) {
        count = initial;
    }
    void add() {
        synchronized(lock) {
            count++;
        }
    }
    synchronized void add(boolean f) {
        count++;
    }
}
class CountThread extends Thread {
    Counter counter;
    CountThread(Counter c) {
        counter = c;
    }
    public void run() {
        for(int i=0;i<100;i++) counter.add();
    }
}
public class ThreadSyncTest {
    public static void main(String[] args) {
        Counter c = new Counter();
        Thread[] t = new Thread[10];
        for(int i=0;i<10;i++) {
            t[i] = new CountThread(c);
            t[i].start();
        }
        for(int i=0;i<10;i++) {
            try {
                t[i].join();
            }
            catch(Exception e) {

            }
        }
        c.print();
    }
}