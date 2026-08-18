package Update;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.List;
import java.util.Scanner;

public class StockTradingClient {
    private static final String SERVER_HOST = "localhost";
    private static final int SERVER_PORT = 12345;

    public static void main(String[] args) throws IOException, ClassNotFoundException {
        try (Socket socket = new Socket(SERVER_HOST, SERVER_PORT);
             ObjectInputStream in = new ObjectInputStream(socket.getInputStream());
             ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream())) {

            List<Stock> availableStocks = (List<Stock>) in.readObject();

            Scanner scanner = new Scanner(System.in);
            System.out.println("Enter username: ");
            String username = scanner.nextLine();
            out.writeObject(username);
            displayAvailableStocks(availableStocks);

            if (!username.equalsIgnoreCase("ADMIN")) {
                while (true) {
                    System.out.println("Enter a command (S, U, V, E): ");
                    String userInput = scanner.nextLine();
                    switch (userInput.charAt(0)) {
                        case 'S':
                            out.writeObject(userInput);
                            break;
                        case 'U':
                            out.writeObject(userInput);
                            break;
                        case 'V':
                            out.writeObject(userInput);
                            break;

                        case 'E':
                            System.exit(0);

                        default:
                            System.out.println("Invalid command. Please try again.");
                    }
                    Object receivedObject = in.readObject();
                    if (receivedObject instanceof String) {
                        String updateMessage = (String) receivedObject;
                        System.out.println("Received update: " + updateMessage);
                    } else if (receivedObject instanceof List) {
                        List<Stock> updatedStocks = (List<Stock>) receivedObject;
                        displayAvailableStocks(updatedStocks);
                    }
                }
            } else {
                while (true) {
                    System.out.println("Enter a command (C, D, I, E): ");
                    String userInput = scanner.nextLine();
                    switch (userInput.charAt(0)) {
                        case 'I':

                            out.writeObject(userInput);
                            break;

                        case 'D':
                            out.writeObject(userInput);
                            break;
                        case 'C':
                            out.writeObject(userInput);
                            break;
                        case 'E':
                            System.exit(0);

                        default:
                            System.out.println("Invalid command. Please try again.");
                    }

                    Object receivedObject = in.readObject();
                    if (receivedObject instanceof String) {
                        String updateMessage = (String) receivedObject;
                        System.out.println("Received update: " + updateMessage);
                    } else if (receivedObject instanceof List) {
                        List<Stock> updatedStocks = (List<Stock>) receivedObject;
                        displayAvailableStocks(updatedStocks);
                    }
                }
            }
        }
    }

    private static void displayAvailableStocks(List<Stock> availableStocks) {
        System.out.println("Available stocks:");
        for (Stock stock : availableStocks) {
            System.out.println(stock.getName() + " - Count: " + stock.getCount() + ", Price: " + stock.getPrice());
        }
    }
}
