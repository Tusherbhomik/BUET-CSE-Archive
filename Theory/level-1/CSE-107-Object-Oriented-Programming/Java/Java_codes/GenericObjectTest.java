interface printable {
    void print();
}
class Tuple<A,B,C> implements printable {
    public void print() {
        System.out.println(x + " " + y + " " + z);
    }
    A x;
    B y;
    C z;
    void setA(A t) {x=t;}
    void setB(B t) {y=t;}
    void setC(C t) {z=t;}
    A getA() {return x;}
    B getB() {return y;}
    C getC() {return z;} 
}
public class GenericObjectTest {
    public static void main(String[] args) {
        Tuple<Integer,Double,String> t = new Tuple<>();
        t.setA(10);
        t.setB(1.1);
        t.setC("hello world!");
        t.print();
    }  
}