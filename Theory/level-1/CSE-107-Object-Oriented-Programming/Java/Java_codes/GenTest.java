
class X {
    int x;

    void f1() {
        System.out.println("In f1");
    }
}

interface Y {
    void f2();
}

interface Z {
    void f3();
}

class MyX extends X implements Y, Z {
    public void f2() {
        System.out.println("In f2");
    }

    public void f3() {
        System.out.println("In f3");
    }
}

public class GenTest<T extends X & Y & Z> {
    private T a;

    public void setObj(T a) {
        this.a = a;
    }

    public T getObj() {
        return this.a;
    }

    public static void main(String[] args) {
        MyX objX = new MyX();
        GenTest<MyX> myGenerics = new GenTest<>();
        myGenerics.setObj(objX);
        System.out.println(myGenerics.getObj());
    }
}