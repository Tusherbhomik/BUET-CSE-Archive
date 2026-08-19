public class File implements FileSystemComponent{

    private String name;
    private int size;
    private String type;
    private String directory;
    private String creationTime;

    public File(String name, int size, String type, String directory, String creationTime) {
        this.name = name;
        this.size = size;
        this.type = type;
        this.directory = directory;
        this.creationTime = creationTime;
    }

    @Override
    public String getName() {
        return name;
    }
    @Override
    public String getType() {
        return type;
    }
    @Override
    public int getSize() {
        return size;
    }
    @Override
    public String getDirectory() {
        return directory;
    }
    @Override
    public int getComponentCount() {
        return 0;
    }

    @Override
    public String getCreationTime() {
        return creationTime;
    }

    @Override
    public void displayDetails() {
        System.out.println("Name: " + name);
        System.out.println("Type: " + type);
        System.out.println("Size: " + size + " kB");
        System.out.println("Directory: \"" + directory + "\"");
        //System.out.println("Component Count: 0"); // Files have no components
        System.out.println("Creation time: " + creationTime);
    }
}
