package Update;

import java.io.Serializable;

public class SystemAdministrator implements Serializable {

    public String increasePrice(Stock stock, double amount) {
        stock.setPrice(stock.getPrice() + amount);
        stock.notifyObservers();
        System.out.println("Increased price of " + stock.getName() + " by " + amount);
        return "Increased price of " + stock.getName() + " by " + amount;
    }

    public String decreasePrice(Stock stock, double amount) {
        double newPrice = stock.getPrice() - amount;
        if (newPrice >= 0) {
            stock.setPrice(newPrice);
            stock.notifyObservers();
            System.out.println("Decreased price of " + stock.getName() + " by " + amount);
            return "Decreased price of " + stock.getName() + " by " + amount;
        } else {
            System.out.println( "Error: Price cannot be negative.");
            return  "Error: Price cannot be negative.";
        }
    }

    public String changeCount(Stock stock, int countChange) {
        stock.setCount( countChange);
        stock.notifyObservers();
        System.out.println("Changed count of " + stock.getName() + " by " + countChange);
        return "Changed count of " + stock.getName() + " by " + countChange;
    }
}
