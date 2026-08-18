import java.util.*;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class FileSystemManager {

    private List<Drive> drives;
    private String currentDirectory;
    private Folder currentFolder;
    private Drive currentDrive;
    private static final String ROOT_PATH = "/";

    public FileSystemManager() {
        drives = new ArrayList<>();
        currentDirectory = ROOT_PATH;
    }

    public void mkDrive(String name) {
        if (currentDirectory.equalsIgnoreCase(ROOT_PATH)) {
            drives.add(new Drive(name));
            System.out.println("Drive '" + name + "' created.");
        } else {
            System.out.println("You can not create Rive from here .Navigate to the root path Using Cd ~");
        }
    }

    public void mkDir(String name) {
        LocalDateTime currentDateTime = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("dd-MM-yyyy HH:mm:ss");
        String formattedDateTime = currentDateTime.format(formatter);
        if (currentFolder != null) {
            currentFolder.getComponents().add(new Folder(name, currentDirectory, formattedDateTime));

        } else {
            currentDrive.getFolders().add(new Folder(name, currentDirectory, formattedDateTime));
        }
        System.out.println("Folder '" + name + "' created in the current directory.");
    }

   /* public void cd(String path) {
        if(path.equalsIgnoreCase("~"))
        {
            currentDirectory=ROOT_PATH;
        }
        else if(path.contains("\\")){//back to drive
            String temp = path.substring(0, 1);
            for (Drive drive : drives) {
                if (drive.getName().equalsIgnoreCase(temp)) {
                    currentDirectory=temp+":\\";
                    currentDrive=drive;
                    currentFolder=null;

                    break;
                }
                System.out.println("Not found any Directory in the same name");
            }
        }
        else {
            String[] parts = currentDirectory.split("\\\\");
            String currentFolderName = parts[parts.length - 1];
            if(currentFolder!=null)
            {
                for (FileSystemComponent fileSystemComponent:currentFolder.getComponents()) {
                    if(fileSystemComponent.getName().equalsIgnoreCase(currentFolderName))
                    {
                        currentFolder= (Folder) fileSystemComponent;
                        break;
                    }

                }
            }
        }
        System.out.println(path);

    }*/

    public void cd(String path) {
        if (path.equalsIgnoreCase("~")) {
            currentDirectory = ROOT_PATH;
        } else if (path.contains("\\")) { // Navigate to drive
            String temp = path.substring(0, 1);
            for (Drive drive : drives) {
                if (drive.getName().equalsIgnoreCase(temp)) {
                    currentDirectory = temp + ":\\";
                    currentDrive = drive;
                    currentFolder = null;
                    break;
                }
            }
        } else { // Navigate within folders
            String newPath = currentDirectory + "\\" + path;
            boolean found = false;

            if (currentFolder != null) {
                for (FileSystemComponent fileSystemComponent : currentFolder.getComponents()) {
                    if (fileSystemComponent.getName().equalsIgnoreCase(path)) {
                        currentDirectory = newPath;
                        currentFolder = (Folder) fileSystemComponent;
                        found = true;
                        break;
                    }
                }
            }

            if (!found) {
                for (Folder folder : currentDrive.getFolders()) {
                    if (folder.getName().equalsIgnoreCase(path)) {
                        currentDirectory = newPath;
                        currentFolder = folder;
                        found = true;
                        break;
                    }
                }
            }

            if (!found) {
                System.out.println("Error: Folder '" + path + "' doesn't exist in the current directory or drive.");
            }
        }
        System.out.println("Current directory set to: " + currentDirectory);
    }


    public void touch(String name, int size) {
        LocalDateTime currentDateTime = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("dd-MM-yyyy HH:mm:ss");
        String formattedDateTime = currentDateTime.format(formatter);
        if (currentFolder != null) {
            File newFile = new File(name, size, "File", currentDirectory + "\\" + name, formattedDateTime);
            currentFolder.getComponents().add(newFile);
            System.out.println("File '" + name + "' created in the current directory.");
        }

    }

    public void list() {
        if (currentFolder != null) {
            for (FileSystemComponent item : currentFolder.getComponents()) {
                System.out.println(item.getName());
            }
        } else {
            //under a drive
            for (FileSystemComponent item : currentDrive.getFolders()) {
                System.out.println(item.getName());
            }
            for (FileSystemComponent item : currentDrive.getFiles()) {
                System.out.println(item.getName());
            }


        }
    }

    public void jumpToRoot() {
        currentDrive = null;
        currentFolder = null;
        currentDirectory = ROOT_PATH;
        System.out.println("Now in root");
    }
    public void ls(String name)
    {
        if(currentFolder!=null)
        {
            for (FileSystemComponent item : currentFolder.getComponents()) {
                if(item.getName().equalsIgnoreCase(name))
                {
                    item.displayDetails();
                }
            }
        }
        else {
            for (FileSystemComponent item : currentDrive.getFolders()) {
                if(item.getName().equalsIgnoreCase(name))
                {
                    item.displayDetails();
                }
            }
            for (FileSystemComponent item : currentDrive.getFiles()) {
                if(item.getName().equalsIgnoreCase(name))
                {
                    item.displayDetails();
                }
            }

        }
    }
    public void delete(String name) {

        if (currentFolder != null) {
            for (FileSystemComponent item:currentFolder.getComponents()) {
                if(item.getName().equalsIgnoreCase(name))
                {
                    if(item instanceof File)
                    {
                        currentFolder.getComponents().remove(item);
                        System.out.println("Deleted the file "+name);
                    }
                    if(item instanceof Folder)
                    {
                        if(item.getComponentCount()==0){
                            currentFolder.getComponents().remove(item);
                            System.out.println("Deleted the empty Folder");
                        }
                    }
                }

            }

        } else {
            for (FileSystemComponent item: currentDrive.getFolders()) {
                if(item.getName().equalsIgnoreCase(name)&&item.getComponentCount()!=0)
                {
                    System.out.println("Can't Delete the folder is not empty");
                }
                if(item.getName().equalsIgnoreCase(name)&&item.getComponentCount()==0)
                {
                    currentDrive.getFolders().remove(item);
                    System.out.println("Removed The empty Folder");
                }

            }

        }//in drive
    }
    public void deleteRecursive(String name) {

            if (currentFolder != null) {
                Iterator<FileSystemComponent> iterator = currentFolder.getComponents().iterator();
                while (iterator.hasNext()) {
                    FileSystemComponent item = iterator.next();
                    if (item.getName().equalsIgnoreCase(name)) {
                        if (item instanceof File) {
                            iterator.remove();
                            System.out.println("Deleted the file " + name);
                        } else if (item instanceof Folder) {
                            deleteFolderRecursive((Folder) item);
                        }
                    }
                }
            } else {
                Iterator<File> fileIterator = currentDrive.getFiles().iterator();
                while (fileIterator.hasNext()) {
                    FileSystemComponent item = fileIterator.next();
                    if (item.getName().equalsIgnoreCase(name)) {
                        fileIterator.remove();
                        System.out.println("Deleted the file " + name);
                    }
                }
                Iterator<Folder> folderIterator = currentDrive.getFolders().iterator();
                while (folderIterator.hasNext()) {
                    Folder item = folderIterator.next();
                    if (item.getName().equalsIgnoreCase(name)) {
                        deleteFolderRecursive(item);
                        folderIterator.remove();
                        System.out.println("Deleted folder: " + name);
                    }
                }
            }
        }



    private void deleteFolderRecursive(Folder folder) {

        List<FileSystemComponent> components = new ArrayList<>(folder.getComponents());
        for (FileSystemComponent component : components) {
            if (component instanceof File) {
                folder.removeComponent(component);
                System.out.println("Deleted file: " + component.getName());
            } else if (component instanceof Folder) {
                deleteFolderRecursive((Folder) component);
                folder.removeComponent(component);
                System.out.println("Deleted folder: " + component.getName());
            }
        }
    }



}





