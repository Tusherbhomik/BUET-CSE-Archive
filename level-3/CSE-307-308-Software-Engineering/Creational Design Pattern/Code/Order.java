import java.util.ArrayList;
import java.util.List;

public class Order {
    private List<Shake> shakes;
    public Order(){
       this.shakes=new ArrayList<>();
    }
    public void addShake(Shake shake)
    {
        shakes.add(shake);
    }
    public double calculateTotalPrice()
    {

        double totalPrice=0;
        for(Shake shake:shakes){
            double shakePrice=shake.getBasePrice();

            if (shake.isLactoseFree()) {

                shakePrice += 60;
            }

            if (shake.isHasCandy()) {

                shakePrice += 50;
            }

            if (shake.isHasCookies()) {

                shakePrice += 40;
            }
            totalPrice+=shakePrice;
        }
        return  totalPrice;
    }
    public  void displayOrderDetails()
    {

        for(Shake shake: shakes)
        {
            System.out.println("*********************************************************");
            System.out.println("Shake :"+shake.getName());
            System.out.println("Ingredients:"+shake.getIngredients());
            System.out.println("Base Price: "+shake.getBasePrice());
            double customPrice=0;
            if(shake.isHasCandy())customPrice+=50;
            if(shake.isHasCookies())customPrice+=40;
            if(shake.isLactoseFree())customPrice+=60;

            System.out.println("After customization: "+(shake.getBasePrice()+customPrice));
            System.out.println("*********************************************************");
        }

        System.out.println("Total Price: "+calculateTotalPrice());
    }
    public boolean isEmpty(){
        return  shakes.isEmpty();
    }
}
