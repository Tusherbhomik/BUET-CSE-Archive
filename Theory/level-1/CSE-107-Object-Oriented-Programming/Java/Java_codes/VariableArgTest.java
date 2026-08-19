public class VariableArgTest {
    public static void f(int ... v) {
        System.out.println("int ... v");
        for(var x:v) 
        System.out.println(x);    
    }
    public static void f(double ... v) {
        System.out.println("double ... v");
        for(var x:v)
        System.out.println(x);
    }
    public static void f(int a, double ... v) {
        System.out.println("int a, double ... v");
        System.out.println(a);
        for(var x:v)
        System.out.println(x);
    }
    /*
    ambiguous
    f();
    public static void f(boolean ... v) {

    }
    */
    /*
    ambiguous
    f(1,2,3);
    public static void f(int a,int ... v) {
        System.out.println(a);
        for(var x:v)
        System.out.println(x);
    }
    */
    public static void main(String[] args) {
        f();
        f(1,2,3);
        f(1,2.0,3);
        f(0,1,0.1);
        f(1.0,2,3);
    }
}