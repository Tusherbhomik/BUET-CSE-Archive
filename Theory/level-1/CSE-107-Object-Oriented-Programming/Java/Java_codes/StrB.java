import java.util.StringTokenizer;

public class StrB {
    public static void main(String[] args) {
        StringBuilder sb = new StringBuilder();
        System.out.println(sb.capacity()); //16
        sb = new StringBuilder(100);
        System.out.println(sb.capacity());
        sb = new StringBuilder("hello world!");
        sb.setCharAt(0, Character.toUpperCase(sb.charAt(0)));
        System.out.println(sb);
        sb.replace(0, 5,"Hallo");
        System.out.println(sb);
        System.out.println(sb.capacity()); //12+16 = 28
        sb.append(123);
        sb.append(true);
        sb.append(' ');
        sb.append("test__");
        sb.insert(0, "__");
        sb.delete(2, 6);
        System.out.println(sb);
        sb.reverse();
        System.out.println(sb);
        sb = new StringBuilder("2022-10-14");
        StringTokenizer s = new StringTokenizer(sb.toString(),"-");
        System.out.println(s.countTokens());
        while(s.hasMoreTokens()) {
            System.out.println(s.nextToken());
        }
        
    }
}