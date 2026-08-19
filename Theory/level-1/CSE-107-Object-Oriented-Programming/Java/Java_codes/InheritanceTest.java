class print {
    public static void string(String s) {
        System.out.println(s);
    }
}
class point2d {
    int hidden = 0;
    // this cannot be overriden
    final void overrideMe() {

    }
    int x,y;
    point2d(int x, int y) {
        this.x = x;
        this.y = y;
    }
    double abs() {
        return Math.sqrt(x*x + y*y);
    }
    void f() {

    }
}

class point3d extends point2d {
    int hidden = 10;
    /*void overrideMe() {

    }*/
    int z;
    point3d(int x,int y, int z) {
        super(x,y);
        this.z = z;
        this.hidden = super.hidden;
    }
    double abs() {
        double r = super.abs();
        return Math.sqrt(r*r + z*z);
    }
    void g() {

    }
}

class P {
    void call() {
        print.string("this is p");   
    }
}
class Q extends P {
    void call() {
        print.string("this is q");
    }
}
class R extends Q {
    void call() {
        print.string("this is r");
    }
}
final class A {
    // this class cannot be extended
}
/* 
class B extends A {

}
*/
public class InheritanceTest {
    public static void main(String[] args) {
        point3d a = new point3d(0, 0, 0);
        point2d b = new point3d(1,2,2);
        print.string(b.abs() + "");
        //dynamic method dispatch
        P p = new P();
        Q q = new Q();
        R r = new R();
        P x;
        x = p;
        x.call();
        x = q;
        x.call();
        x = r;
        x.call();
    }
}