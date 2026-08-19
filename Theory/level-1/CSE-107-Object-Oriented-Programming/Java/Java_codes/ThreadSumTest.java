class SumThread extends Thread {
    int l,r,sum = 0;
    SumThread(int l,int r) {
        this.l = l;
        this.r = r;
    }
    public void run() {
        for(int i=l;i<=r;i++) sum+=i;
    }
}
public class ThreadSumTest {
    public static void main(String[] args) {
        // sum from 1 to 10000
        SumThread t[] = new SumThread[10];
        for(int i=0;i<10;i++) {
            t[i] = new SumThread(i*100 + 1, (i+1) * 100);
            t[i].start();
        }
        for(int i=0;i<10;i++) {
            try {
                t[i].join();
            }
            catch(Exception e) {

            }
        }
        int sum = 0;
        for(int i=0;i<10;i++) sum += t[i].sum;
        System.out.println(sum);
    }
}