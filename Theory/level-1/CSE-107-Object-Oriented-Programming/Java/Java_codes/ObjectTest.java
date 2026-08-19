import java.util.Objects;

class vector {
    int x,y,z;
    vector(int x,int y,int z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }
    @Override
    public String toString() {
        return "<" + x + "," + y + "," + z + ">";
    }
    @Override
    public boolean equals(Object o) {
        if(o == this) return true;
        if(!(o instanceof vector)) return false;
        vector v = (vector) o;
        if(v.x == x && v.y == y && v.z == z) return true;
        return false;
    }
    @Override
    public int hashCode() {
        return Objects.hash(x,y,z);
    }
}
public class ObjectTest {
    public static void main(String[] args) {
        vector v = new vector(1,2,2);
        System.out.println(v);
        System.out.println(v.hashCode());
    }
}