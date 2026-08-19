class Global {
    final static double pi = Math.PI;
    final static double e = Math.E;
    static int x;
    int p = 0;
    static {
        x = 3;
        System.out.println("Static block is executed");
    }
    static int get() {
        return x;
    }
    static void set(int t) {
        // this cannot be used in static
        x = t;
    }
    static double ex(double x) {
        return Math.pow(e, x);
    }
    void setP(int p) {
        this.p = p;
    }
    int getP() {
        return p;
    }
    int f() {
        int r = 1;
        for(int i=0;i<p;i++) r*=x;
        return r;
    }
}
public class StaticTest {
    public static void main(String[] args) {
        System.out.println("x = " + Global.get());
        Global.set(3);
        Global.x = 2;
        System.out.println("x = " + Global.get());
        Global a = new Global();
        a.setP(5);
        System.out.println(a.f());
        Global b = new Global();
        b.setP(6);
        System.out.println(b.f());
        System.out.println(Global.ex(0));
    }
}