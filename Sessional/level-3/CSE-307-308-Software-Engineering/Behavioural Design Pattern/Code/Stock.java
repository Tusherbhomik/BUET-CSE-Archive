package Update;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class Stock implements Serializable {
    private static final long serialVersionUID = 1L;

    private String name;
    private int count;
    private double price;
    private List<User> observers;

    public Stock(String name, int count, double price) {
        this.name = name;
        this.count = count;
        this.price = price;
        this.observers = new ArrayList<>();
    }

    public String getName() {
        return name;
    }

    public int getCount() {
        return count;
    }

    public double getPrice() {
        return price;
    }

    public void setCount(int count) {
        this.count = count;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    public void addObserver(User observer) {
        observers.add(observer);
    }

    public void removeObserver(User observer) {
        observers.remove(observer);
    }

    public void notifyObservers() {
        for (User observer : observers) {
            observer.update(this);
        }
    }
}
