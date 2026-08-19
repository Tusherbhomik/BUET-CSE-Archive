package Update;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class StockTradingServer {
    private static final int PORT = 12345;
    private List<ClientHandler> clients;
    private StockMarket stockMarket;

    public StockTradingServer() {
        this.clients = new ArrayList<>();
        this.stockMarket = new StockMarket();
        this.stockMarket.initializeStocksFromFile("init_stocks.txt");
    }

    public void start() throws IOException {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("Server is running on port " + PORT);

            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("New client connected: " + clientSocket);

                // Create a new thread for each client
                ClientHandler clientHandler = new ClientHandler(clientSocket, this);
                clients.add(clientHandler);
                new Thread(clientHandler).start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public synchronized void broadcastUpdate(Stock stock) {
        for (ClientHandler client : clients) {
            client.sendUpdateNotification(stock);
        }
    }

    public synchronized void broadcastUserDisconnect(User user) {
        for (ClientHandler client : clients) {
            client.sendUserDisconnectNotification(user);
        }
    }

    public List<ClientHandler> getClients() {
        return clients;
    }

    public StockMarket getStockMarket() {
        return stockMarket;
    }
    public synchronized void notifyUserDisconnect(User user) {
        for (ClientHandler client : clients) {
            client.sendUserDisconnectNotification(user);
        }
    }

    public static void main(String[] args) throws IOException {
        StockTradingServer server = new StockTradingServer();
        server.start();
    }
}
