

public class Main2 {
    public static void main(String[] args) {
        FileSystemManager fileSystemManager = new FileSystemManager();

        fileSystemManager.mkDrive("C");
        fileSystemManager.cd("C:\\");
        fileSystemManager.mkDir("Music");
        fileSystemManager.cd("Music");
        fileSystemManager.mkDir("mp3");
        fileSystemManager.cd("mp3");
        fileSystemManager.touch("rainbow.mp3", 10);
       // fileSystemManager.touch("rainbow1.mp3", 10);
        //fileSystemManager.touch("rainbow2.mp3", 10);
        //fileSystemManager.touch("rainbow3.mp3", 10);
        fileSystemManager.list();
        fileSystemManager.jumpToRoot();
        fileSystemManager.cd("C:\\");
        fileSystemManager.ls("Music");
//        fileSystemManager.cd("Music");
//        fileSystemManager.ls("mp3");
//        fileSystemManager.cd("mp3");
//        fileSystemManager.ls("rainbow.mp3");
        fileSystemManager.delete("Music");
        fileSystemManager.deleteRecursive("Music");

    }
}