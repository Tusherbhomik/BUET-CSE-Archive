import java.util.Scanner;
import java.util.StringTokenizer;
public class ScannerTest {
    static Scanner sc = new Scanner(System.in);
    public static void main(String[] args) {
        int T = Integer.parseInt(sc.nextLine());
        while(T-- > 0) {
            int sum = 0;
            String list = sc.nextLine();
            StringTokenizer st = new StringTokenizer(list,",");
            while(st.hasMoreTokens()) {
                String token = st.nextToken();
                sum += Integer.valueOf(token);
            }   
            System.out.println("Sum = " + sum);
        }
    }
}