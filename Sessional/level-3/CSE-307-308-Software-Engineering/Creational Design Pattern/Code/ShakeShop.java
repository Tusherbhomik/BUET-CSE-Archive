import java.util.Scanner;

public class ShakeShop {

    public static void createCustomizedShake(Shake.ShakeBuilder shakeBuilder) {
        Scanner scanner = new Scanner(System.in);


        System.out.println("Do u want to make it lactose-free? (y/n)");
        char lactoseChoice = scanner.next().charAt(0);
        if (lactoseChoice == 'y') {
            shakeBuilder.setLactoseFree();
            shakeBuilder.addIngredient("Almond Milk");
        }
        System.out.println("Do you want to add candy topping? (y/n)");
        char candyChoice = scanner.next().charAt(0);
        if (candyChoice == 'y') {
            shakeBuilder.setHasCandy();

        }
        System.out.println("Do you want to add cookies topping ?(y/n)");
        char cookieChoice = scanner.next().charAt(0);
        if (cookieChoice == 'y') {
            shakeBuilder.setHasCookies();
        }

    }

    public static Shake newShakeAdd(Director director) {
        Scanner scanner = new Scanner(System.in);
        String[] shakes = {"Chocolate Shake", "Coffee Shake", "Strawberry Shake", "Vanilla Shake", "Zero Shake"};

        System.out.println("Select a shake:");
        for (int i = 0; i < shakes.length; i++) {
            System.out.println((i + 1) + ") " + shakes[i]);
        }

        Shake.ShakeBuilder builder = new Shake.ShakeBuilder();

        int shakeChoice = scanner.nextInt();


        if (shakeChoice == 1) {
            director.createChocolateShake(builder);
            createCustomizedShake(builder);
        } else if (shakeChoice == 2) {
            director.createCoffeeShake(builder);
            createCustomizedShake(builder);
        } else if (shakeChoice == 3) {
            director.createStrawberryShake(builder);
            createCustomizedShake(builder);
        } else if (shakeChoice == 4) {
            director.createVanillaShake(builder);
            createCustomizedShake(builder);
        } else if (shakeChoice == 5) {
            director.createZeroShake(builder);
            createCustomizedShake(builder);
        } else {
            System.out.println("Enter a Valid choice");
        }
        return builder.build();

    }

    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);
        Order currentOrder = null;
        Director director = new Director();
        char input;

        System.out.println("Welcome to Shake Shop");
        System.out.println("Press 'o' to open an order, 'e' to close an order, or 'q' to quit:");

        while (true) {

            input = scanner.next().charAt(0);

            if (input == 'q') {
                break;
            } else if (input == 'a') {
                assert currentOrder != null;
                currentOrder.addShake(newShakeAdd(director));
                System.out.println("Shake added to the order. Total Price so far: " + currentOrder.calculateTotalPrice());
                System.out.println("'a' to add more to the current order , 'e' to close this order, or 'q' to quit:");


            } else if (input == 'o') {
                if (currentOrder == null) {
                    currentOrder = new Order();
                    System.out.println("New order Opened");
                    currentOrder.addShake(newShakeAdd(director));
                    System.out.println("Shake added to the order. Total Price so far: " + currentOrder.calculateTotalPrice());
                    System.out.println("'a' to add more to the current order , 'e' to close the order, or 'q' to quit:");
                } else {
                    System.out.println("An order is already open. Please close the current order first ('e').");
                    System.out.println("Or you can add more to the current order press 'a");
                }


            } else if (input == 'e') {
                if (currentOrder != null && !currentOrder.isEmpty()) {
                    currentOrder.displayOrderDetails();
                    currentOrder = null;
                } else {
                    System.out.println("There are no items in the current order.");
                }
                System.out.println("Press 'o' to open an new order or 'q' to quit");

            } else {
                System.out.println("Invalid input. Please try again.");
            }

        }
    }
}
