package Update;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class StockMarket implements Serializable {
    private static final long serialVersionUID = 1L;

    private List<Stock> availableStocks;

    public StockMarket() {
        this.availableStocks = new ArrayList<>();
    }

    public void initializeStocksFromFile(String fileName) {
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] parts = line.split(" ");
                String name = parts[0];
                int count = Integer.parseInt(parts[1]);
                double price = Double.parseDouble(parts[2]);
                availableStocks.add(new Stock(name, count, price));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public List<Stock> getAvailableStocks() {
        return availableStocks;
    }
}
