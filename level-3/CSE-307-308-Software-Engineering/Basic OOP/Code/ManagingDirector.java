
public class ManagingDirector implements Employee {
    String name;

    public ManagingDirector(String name) {
        this.name = name;
    }
    @Override
    public String getName() {
        return name;
    }
}
