import java.util.*;
class A implements Comparable<A> {
    int x = 0;
    A(int t) {
        x=t;
    }
    @Override
    public int compareTo(A o) {
        if(x < o.x) return -1;
        return 1;
    }
}
public class SortTest {
    public static void main(String[] args) {
        List<Integer> l = new ArrayList<Integer>();
        l.add(10);
        l.add(2);
        int a[] = {4,1,5};
        Collections.sort(l);
        Arrays.sort(a);
        for(int x: a) System.out.println(x);
        for(var x: l) System.out.println(x);
        System.out.println("Increasing order");
        var b = new ArrayList<A>();
        b.add(new A(10));
        b.add(new A(4));
        Collections.sort(b);
        for(var x: b) System.out.println(x.x);
        System.out.println("Decreasing order");
        Collections.sort(b,  new Comparator<A>(){
            public int compare(A o1, A o2) {
                if(o1.x < o2.x) return 1;
                return -1;
            }
        });
        for(var x: b) System.out.println(x.x);
    }
}