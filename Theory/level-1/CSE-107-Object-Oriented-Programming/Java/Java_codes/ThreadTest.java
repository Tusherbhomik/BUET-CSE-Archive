class Thread1 implements Runnable {
    public void run() {
        System.out.println("Thread1 sum = " + ThreadTest.sum());
    }    
}
class Thread2 extends Thread {
    public void run() {
        System.out.println("Thread2 sum = " + ThreadTest.sum());
    }
}
public class ThreadTest {
    static int sum() {
        int s = 0;
        for(int i=0;i<10000000;i++) {
            s += i;
            s %= 69;
        }
        return s;
    }
    public void f() {
        System.out.println("ThreadTest sum = " + ThreadTest.sum());
    }
    public static void main(String[] args) throws Exception {
        new Thread(new Thread1()).start();
        new Thread2().start();    
        new Thread(new ThreadTest()::f).start();
        Thread.sleep(1000);
        Thread t = Thread.currentThread();
        System.out.println(t.getName());
        t.setName("MAIN THREAD");
        System.out.println(t.getName());

    }
}