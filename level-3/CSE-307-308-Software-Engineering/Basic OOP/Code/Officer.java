

public class Officer implements Employee {
    private String name;
    public Officer(String name) {
        this.name = name;
    }

    @Override
    public String getName() {
        return name;
    }
}
