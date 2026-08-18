public class Utility {
    public static double stringToNumber(String numberStr) {

        String cleanNumberStr = numberStr.replace(",", "");
        try {
            double number = Double.parseDouble(cleanNumberStr);

            if (number < 0) {
                throw new IllegalArgumentException("Number cannot be negative");
            }
            return number;
        } catch (NumberFormatException e) {
            throw new IllegalArgumentException("Invalid number format");
        }
    }
}
