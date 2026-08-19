
public class StringTest {
    public static void main(String[] args) {
        String a = new String("hello");
        String b = "world";
        String c = new String(a + " " + b);
        char charArray[] = {'a','b','c','d'};
        String d = new String(charArray);
        String e = new String(charArray, 0, 2);
        
        char ara[] = new char[100];

        System.out.println(e);
        System.out.println(c.length());
        c.getChars(0, 5, ara, 0);
        String s = new String(ara);
        System.out.println(s);

        System.out.println(a.substring(0, 4)); 
        if(a.equals(c.substring(0, 5))) {
            System.out.println("they are equal");
        }   
        if(a.regionMatches(0, c, 0, 5)) {
            System.out.println("Region matched");
        }
        if(c.startsWith(a)) {
            System.out.println("c starts with a");
        }
        if(c.endsWith(b)) {
            System.out.println("c ends with b");
        }
        String A = a.toUpperCase();  
        if(a.equalsIgnoreCase(A)) {
            System.out.println("a = A " + a + " " + A);
        }
        if(b.compareTo(a) > 0) {
            System.out.println("b > a");
        }
        else if(b.compareTo(a) < 0) {
            System.out.println("b < a");
        }

        String t = c.concat(" - concatenation");
        System.out.println(t);
        System.out.println(t.length());
        System.out.println(t.indexOf('e'));
        System.out.println(t.lastIndexOf('o'));
        System.out.println("Splitted[1] = " + t.split("-")[1]);
        System.out.println(t.split("-")[1].trim());
        System.out.println(t.replace('-','~'));
        for(int i=0;i<t.length();i++) {
            System.out.print(t.charAt(i));
        }
        System.out.println();
    }
}