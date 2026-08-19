import java.util.ArrayList;
import java.util.List;

public class Shake {
    private String name;
    private double basePrice;
    private List<String> ingredients;
    private boolean isLactoseFree;
    private boolean hasCandy;
    private boolean hasCookies;

    private Shake(String name, double basePrice, List<String> ingredients, boolean isLactoseFree, boolean hasCandy, boolean hasCookies) {
        this.name = name;
        this.basePrice = basePrice;
        this.ingredients = new ArrayList<>(ingredients);
        this.isLactoseFree = isLactoseFree;
        this.hasCandy = hasCandy;
        this.hasCookies = hasCookies;
    }

    public String getName() {
        return name;
    }

    public double getBasePrice() {
        return basePrice;
    }

    public List<String> getIngredients() {
        return ingredients;
    }

    public boolean isLactoseFree() {
        return isLactoseFree;
    }

    public boolean isHasCandy() {
        return hasCandy;
    }

    public boolean isHasCookies() {
        return hasCookies;
    }


    public static class ShakeBuilder {
        private String name;
        private double basePrice;
        private List<String> ingredients = new ArrayList<>();
        private boolean isLactoseFree = false;
        private boolean hasCandy = false;
        private boolean hasCookies = false;


        public ShakeBuilder setName(String name) {
            this.name = name;
            return this;
        }

        public ShakeBuilder setBasePrice(double basePrice) {
            this.basePrice = basePrice;
            return this;
        }

        public ShakeBuilder addIngredient(String ingredient) {
            if (!ingredients.contains(ingredient)) {
                ingredients.add(ingredient);
            }
            return this;
        }

        public ShakeBuilder setLactoseFree() {
            this.isLactoseFree = true;
            return this;
        }

        public ShakeBuilder setHasCandy() {
            this.hasCandy = true;
            return this;
        }

        public ShakeBuilder setHasCookies() {
            this.hasCookies = true;
            return this;
        }

        public Shake build() {
            return new Shake(name, basePrice, ingredients, isLactoseFree, hasCandy, hasCookies);
        }


    }
}
