public class Director {
    public void createChocolateShake(Shake.ShakeBuilder shakeBuilder) {
        shakeBuilder
                .setName("Chocolate Shake")
                .setBasePrice(230)
                .addIngredient("Chocolate syrup")
                .addIngredient("Sugar")
                .addIngredient("Chocolate ice cream");

    }
    public void createCoffeeShake(Shake.ShakeBuilder shakeBuilder) {
        shakeBuilder
                .setName("Coffee Shake")
                .setBasePrice(250)
                .addIngredient("Coffee")
                .addIngredient("Sugar")
                .addIngredient("Jello");

    }

    public void createStrawberryShake(Shake.ShakeBuilder shakeBuilder) {
        shakeBuilder
                .setName("Strawberry Shake")
                .setBasePrice(200)
                .addIngredient("Strawberry syrup")
                .addIngredient("Sugar")
                .addIngredient("Strawberry ice cream");
    }

    public void createVanillaShake(Shake.ShakeBuilder shakeBuilder) {
        shakeBuilder
                .setName("Vanilla Shake")
                .setBasePrice(190)
                .addIngredient("Sugar")
                .addIngredient("Vanilla flavoring")
                .addIngredient("Jello");

    }

    public void createZeroShake(Shake.ShakeBuilder shakeBuilder) {
        shakeBuilder
                .setName("Zero Shake")
                .setBasePrice(240)
                .addIngredient("Sweetener")
                .addIngredient("Vanilla flavoring")
                .addIngredient("Sugar-free jello");
    }
}
