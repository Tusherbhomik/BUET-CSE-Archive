import java.util.Scanner;

public class FileUploadClient {

    public static void main(String[] args) {
        while (true){
            Scanner scanner = new Scanner(System.in);
            String command = scanner.nextLine();

            String[] commandParts = command.split(" ");

            for(String s: commandParts){
                new Thread(new ClientHandler(s)).start();
            }
        }


    }
}