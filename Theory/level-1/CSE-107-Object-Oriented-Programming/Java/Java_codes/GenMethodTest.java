public class GenMethodTest {
    static <T extends Number> void f(T[] v) {
        System.out.println("Numbers are: ");
        for(T x:v) System.out.print(x + " ");
        System.out.println();
    }
    public static void main(String[] args) {
        Integer a[] = new Integer[10];
        for(int i=0;i<10;i++) a[i] = i;
        GenMethodTest.<Integer>f(a);
        String b[] = new String[10];
        for(int i=0;i<10;i++) b[i] = "hello world!";
        GenMethodTest.<String>f(b);
        Double d[] = new Double[10];
        for(int i=0;i<10;i++) d[i] = Double.valueOf(i);
        GenMethodTest.<Double>f(d);
    }
}
