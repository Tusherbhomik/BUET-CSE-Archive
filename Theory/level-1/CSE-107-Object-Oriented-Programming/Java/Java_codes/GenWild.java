import java.util.Collections;
import java.util.List;
import java.util.ArrayList;

public class GenWild<S> {
    public <T> void cube(S x) {
        System.out.println(x);
    }
    static <T extends Number> void square(T x) {
        Number t = x;
        int a = t.intValue();
        System.out.println(a*a);
    }
    static <T> void square(T x) {
        System.out.println("Please provide a valid number");
    }
    static void f(List<S> o) {
        for(var x:o) 
        System.out.println(x);
    }

    static void f(List<? extends Number> o) {
        for(var x:o)
        System.out.println(x);
    }
    public static void main(String[] args) {
        GenWild.<Integer>square(100);
        GenWild<Double> g = new GenWild();
        g.<Integer>cube(10.0);
        List<Integer> a = new ArrayList();
        a.add(10);
        a.add(1);
        f(a);
    }

}