class SumThread extends Thread {
    long sum = 0;
    long at = 0;
    public void run() {
        while(true) {
            at++;
            sum += at;
        }
    }
}
public class ThreadLifeCycleTest {
   public static void main(String[] args) throws Exception {
        SumThread t = new SumThread();
        t.start();
        Thread.sleep(500);
        t.suspend();
        System.out.println(t.sum + " - " + t.at);
        Thread.sleep(500);
        t.resume();
        Thread.sleep(500);
        t.suspend();
        System.out.println(t.sum + " - " + t.at);
        t.stop();
   }
}