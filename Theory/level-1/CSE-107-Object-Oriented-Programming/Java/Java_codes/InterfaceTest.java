interface Printable {
    private String getLineSeperator() {
        return System.lineSeparator();
    }
    static void info() {
        System.out.println("Printable interface");
    }
    default void print() {
        System.out.print("Hello world!" + getLineSeperator());
    }
}
interface sharedContents {
    int YES = 1;
    int NO = 0;
}
interface Initializable {
    void initialize();
}
interface Both extends Printable, Initializable {
    static int get() {
        return 0;
    }
}
class Counter implements Both, sharedContents {
    int x;
    Counter(int t) { x= t; }
    public void print() {
        System.out.println(x);
    }
    public void initialize() {
        x = 0;
    }
}
public class InterfaceTest {
    final static int x = 10;
    void f() {
    }
    public static void main(String[] args) {
        Counter a = new Counter(x);
        Both.get();
        a.print();
        a.initialize();
        a.print();
    }
}