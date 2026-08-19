public class MultidimensionalArrayTest {
    public static void main(String[] args) {
        int a[][];
        a = new int[3][];
        a[0] = new int[1];
        a[1] = new int[2];
        a[2] = new int[3];
        for(var x:a) {
            for(var y:x) 
                System.out.print(y);
            System.out.println();
        }
    }
}