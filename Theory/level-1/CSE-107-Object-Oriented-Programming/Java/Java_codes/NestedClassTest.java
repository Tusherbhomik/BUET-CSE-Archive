// inner classes can directly access outer class
// since inner classes imply outer class must exist
// outer class can access inner class through an object
class first {
	int x = 100;
    second s;
    int sum() {
        return x + s.y + s.t.z;
    }
	class second {
		int y = 200;
        int sum() {
            return first.this.x + y + t.z;
        }
        third t;
		class third {
            int z = 200;
			void show() {
				System.out.println("(" + x + "," + y + "," + z + ")");
			}
            int sum() {
                return first.this.x + second.this.y + z;
            }
		}
	}
}
public class NestedClassTest {
	public static void main(String[] args) {
		first a = new first();
        a.s = a.new second();
        a.s.t = a.s.new third();
        System.out.println(a.sum());
        System.out.println(a.s.sum());
        System.out.println(a.s.t.sum());
	}
}
