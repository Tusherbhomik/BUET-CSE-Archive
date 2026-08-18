import java.util.ArrayList;
import java.util.List;

public class Folder implements FileSystemComponent{
    private String name;
    private List<FileSystemComponent> components;
    private String directory;
    private String creationTime;

    public Folder(String name, String directory, String creationTime) {
        this.name = name;
        this.directory = directory;
        this.creationTime = creationTime;
        components = new ArrayList<>();
    }
    public void addComponent(FileSystemComponent component) {
        components.add(component);
    }
    public void removeComponent(FileSystemComponent component) {
        components.remove(component);
    }
    @Override
    public String getName() {
        return name;
    }

    @Override
    public String getType() {
        return "Folder";
    }
    private int calculateTotalSize() {
        int totalSize = 0;
        for (FileSystemComponent component : components) {
            totalSize += component.getSize();
        }
        return totalSize;
    }

    @Override
    public int getSize() {
        return calculateTotalSize();
    }

    public void setComponents(List<FileSystemComponent> components) {
        this.components = components;
    }

    @Override
    public String getDirectory() {
        return directory;
    }

    @Override
    public int getComponentCount() {
        return components.size();
    }

    @Override
    public String getCreationTime() {
        return null;
    }
    public List<FileSystemComponent> getComponents() {
        return components;
    }

    @Override
    public void displayDetails() {
        System.out.println("Name: " + name);
        System.out.println("Type: Folder");
        int totalSize = calculateTotalSize();
        System.out.println("Size: " + components.size() + " kB");
        System.out.println("Directory: \"" + directory + "\"");
        System.out.println("Component Count: " + components.size());
        System.out.println("Creation time: " + creationTime);
    }
}
