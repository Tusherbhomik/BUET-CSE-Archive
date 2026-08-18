public interface FileSystemComponent {
    String getName();
    String getType();
    int getSize();
    String getDirectory();
    int getComponentCount();
    String getCreationTime();
    void displayDetails();
}
