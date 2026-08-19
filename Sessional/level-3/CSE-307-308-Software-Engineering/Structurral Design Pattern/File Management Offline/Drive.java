import java.util.ArrayList;
import java.util.List;

public class Drive {
    private String name;
    private List<Folder> folders;
    private List<File> files;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public List<Folder> getFolders() {
        return folders;
    }

    public void setFolders(List<Folder> folders) {
        this.folders = folders;
    }

    public List<File> getFiles() {
        return files;
    }

    public void setFiles(List<File> files) {
        this.files = files;
    }

    public Drive(String name) {
        this.name = name;
        folders = new ArrayList<>();
        files = new ArrayList<>();
    }

}
