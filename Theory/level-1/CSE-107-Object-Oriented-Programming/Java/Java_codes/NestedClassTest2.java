class First {
    int x = 3;
    Second s;
    int sum() {
        return x + s.y + s.t.z;
    }
    static class Second {
        int y = 4;
        Third t;
        int sum(First outer) {
            return outer.x + y + outer.s.t.z;
        }
        class Third {
            int z = 5;
            int sum(First outer) {
                return outer.x + Second.this.y + z;
            }
        }
    }
}

public class NestedClassTest2 {
	public static void main(String[] args) {
        First a = new First();
        a.s = new First.Second();
        a.s.t = a.s.new Third();
        System.out.println(a.sum());
        System.out.println(a.s.sum(a));
        System.out.println(a.s.t.sum(a));   
    }
}

