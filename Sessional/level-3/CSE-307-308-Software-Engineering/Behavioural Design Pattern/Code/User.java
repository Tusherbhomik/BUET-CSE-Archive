package Update;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class User implements Serializable {
    private static final long serialVersionUID = 1L;

    private String username;
    private List<Stock> subscribedStocks;

    public User(String username) {
        this.username = username;
        this.subscribedStocks = new ArrayList<>();
    }

    public String getUsername() {
        return username;
    }

    public String subscribe(Stock stock) {
        if (!subscribedStocks.contains(stock)) {
            subscribedStocks.add(stock);
            stock.addObserver(this);
            return username +" subscribed to " +stock.getName();

        } else {
            return  username + " is already subscribed to " + stock.getName();
        }
    }

    public String unsubscribe(Stock stock) {
        if (subscribedStocks.remove(stock)) {
            stock.removeObserver(this);
            return username + " unsubscribed from " + stock.getName();
        } else {
            return  username + " is not subscribed to " + stock.getName();
        }
    }

    public String viewSubscribedStocks() {
        StringBuilder result = new StringBuilder();
        result.append(username).append("'s subscribed stocks:\n");
        for (Stock stock : subscribedStocks) {
            result.append(stock.getName())
                    .append(" - Count: ").append(stock.getCount())
                    .append(", Price: ").append(stock.getPrice())
                    .append("\n");
        }
        return result.toString();
    }

    public boolean isSubscribed(Stock stock) {
        return subscribedStocks.contains(stock);
    }

    public void update(Stock stock) {
        System.out.println(username + " received update for " + stock.getName() +
                ": Count - " + stock.getCount() + ", Price - " + stock.getPrice());
    }
}
