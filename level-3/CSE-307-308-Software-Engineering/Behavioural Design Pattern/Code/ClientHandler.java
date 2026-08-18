package Update;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class ClientHandler implements Runnable {
    private Socket clientSocket;
    private StockTradingServer server;
    private User user;
    private final SystemAdministrator systemAdministrator=new SystemAdministrator();

    public ClientHandler(Socket clientSocket, StockTradingServer server) {
        this.clientSocket = clientSocket;
        this.server = server;
    }


    public User createUser(String username) {
        // Create a User object based on the provided username
        this.user=new User(username);
        return user;
        }


    @Override
    public void run() {
        try (ObjectOutputStream out = new ObjectOutputStream(clientSocket.getOutputStream());
             ObjectInputStream in = new ObjectInputStream(clientSocket.getInputStream())) {

            out.writeObject(server.getStockMarket().getAvailableStocks());

            String personType=(String) in.readObject();
            if(!personType.equalsIgnoreCase("Admin")){

                createUser(personType);
                //System.out.println("Not come");
            }
            while (true) {
                String command = (String) in.readObject();
                processCommand(command,out,in);

            }
        } catch (IOException | ClassNotFoundException e) {
            server.notifyUserDisconnect(user); // Notify other clients about user disconnect,error here
            server.getClients().remove(this);
            System.out.println("Client disconnected: " + clientSocket);
        }
    }

    public void sendMessage(String message,ObjectOutputStream out,ObjectInputStream in)  {
        try {
            out.writeObject(message);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public void sendUpdateNotification(Stock stock) {
        if (user != null && user.isSubscribed(stock)) {
            try {
                ObjectOutputStream out = new ObjectOutputStream(clientSocket.getOutputStream());
                out.writeObject("Stock update: " + stock.getName() +
                        " - Count: " + stock.getCount() + ", Price: " + stock.getPrice());
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void processCommand(String command,ObjectOutputStream out,ObjectInputStream in)  {

        String[] parts = command.split(" ");
        String action = parts[0];

        switch (action) {
            case "S":
                if (parts.length >= 2) {
                    String stockName = parts[1];
                    Stock stockToSubscribe = findStockByName(stockName);
                    if (stockToSubscribe != null) {
                        String msg=user.subscribe(stockToSubscribe);
                        System.out.println(msg);
                        sendMessage(msg,out,in);
                    } else {
                        System.out.println("Error: Stock not found");
                    }
                } else {
                    System.out.println("Error: Invalid subscribe command");
                }
                break;

            case "U":

                if (parts.length >= 2) {
                    String stockName = parts[1];
                    Stock stockToUnsubscribe = findStockByName(stockName);
                    if (stockToUnsubscribe != null) {
                        String msg=user.unsubscribe(stockToUnsubscribe);
                        sendMessage(msg,out,in);
                        System.out.println(user.getUsername() + " unsubscribed from " + stockToUnsubscribe.getName());
                    } else {
                        System.out.println("Error: Stock not found");
                    }
                } else {
                    System.out.println("Error: Invalid unsubscribe command");
                }
                break;

            case "I":

                if (parts.length >= 2) {
                    String stockName = parts[1];
                    Stock stockToUnsubscribe = findStockByName(stockName);
                    if (stockToUnsubscribe != null) {
                        System.out.println("Eddur ashlam");
                        String msg=systemAdministrator.increasePrice(stockToUnsubscribe, Double.parseDouble(parts[2]));
                        sendMessage(msg,out,in);
                    } else {
                        System.out.println("Error: Stock not found");
                    }
                } else {
                    System.out.println("Error: Invalid unsubscribe command");
                }
                break;

            case "D":
                if (parts.length >= 2) {
                    String stockName = parts[1];
                    Stock stockToUnsubscribe = findStockByName(stockName);
                    if (stockToUnsubscribe != null) {
                        String msg=systemAdministrator.decreasePrice(stockToUnsubscribe, Double.parseDouble(parts[2]));
                        sendMessage(msg,out,in);
                    } else {
                        System.out.println("Error: Stock not found");
                    }
                } else {
                    System.out.println("Error: Invalid unsubscribe command");
                }
                break;

            case "C":

                if (parts.length >= 2) {
                    String stockName = parts[1];
                    Stock stockToUnsubscribe = findStockByName(stockName);
                    if (stockToUnsubscribe != null) {
                        String msg=systemAdministrator.changeCount(stockToUnsubscribe, (int) Double.parseDouble(parts[2]));
                        sendMessage(msg,out,in);
                    } else {
                        System.out.println("Error: Stock not found");
                    }
                } else {
                    System.out.println("Error: Invalid unsubscribe command");
                }
                break;

            case "V":
                try {
                    out.writeObject(user.viewSubscribedStocks());
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }

                break;

            default:
                System.out.println("Error: Unknown command");
        }
    }

    private Stock findStockByName(String stockName) {
        for (Stock stock : server.getStockMarket().getAvailableStocks()) {
            if (stock.getName().equals(stockName)) {
                return stock;
            }
        }
        return null;
    }


    public void sendUserDisconnectNotification(User disconnectedUser) {
        try {
            ObjectOutputStream out = new ObjectOutputStream(clientSocket.getOutputStream());
            out.writeObject("User disconnect: " + disconnectedUser.getUsername());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
