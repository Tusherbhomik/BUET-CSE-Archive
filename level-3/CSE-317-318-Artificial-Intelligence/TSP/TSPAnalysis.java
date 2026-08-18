import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class TSPAnalysis {
    private static final Map<String, Integer> optimalLengths = new HashMap<>() {{
        put("a280", 2579);
        put("berlin52", 7542);
        put("bier127", 118282);
        put("ch130", 6110);
        put("ch150", 6528);
        put("eil51", 426);
        put("eil76", 538);
        put("eil101", 629);
        put("kroA100", 21282);
        put("kroB100", 22141);
        put("kroC100", 20749);
        put("kroD100", 21294);
        put("kroE100", 22068);
        put("lin105", 14379);
        put("lin318", 42029);
        put("pr76", 108159);
        put("pr124", 59030);
        put("pr144", 58537);
        put("rat99", 1211);
        put("rat195", 2323);
        put("st70", 675);
    }};

    public static double calculateError(double computedLength, int optimalLength) {
        return Math.abs((computedLength - optimalLength)/optimalLength)*100;
    }
    public static double calculateStandardDeviation(List<Double> errors) {
        double mean = errors.stream().mapToDouble(Double::doubleValue).average().orElse(0);
        double sumSquaredDifferences = errors.stream().mapToDouble(e -> Math.pow(e - mean, 2)).sum();
        return Math.sqrt(sumSquaredDifferences / errors.size());
    }
    public static void generateErrorCSV(String inputCSV, String outputCSV) throws IOException {
        BufferedReader reader = new BufferedReader(new FileReader(inputCSV));
        BufferedWriter writer = new BufferedWriter(new FileWriter(outputCSV));
        reader.readLine();
        
        writer.write("File Name,Constructive Method,Initial Tour Length,After 2-opt,After Node Swap,After Node Shift,2-opt Time (ms),Node Swap Time (ms),Node Shift Time (ms),2-opt Error,Node Swap Error,Node Shift Error,Standard Deviation\n");
        
        String line;
        while ((line = reader.readLine()) != null) {
            String[] parts = line.split(",");
            if (parts.length < 9) continue; 

            String fileName = parts[0].replace(".tsp", ""); 
            String method = parts[1];
            double initialTourLength = Double.parseDouble(parts[2]);
            double afterTwoOpt = Double.parseDouble(parts[3]);
            double afterNodeSwap = Double.parseDouble(parts[4]);
            double afterNodeShift = Double.parseDouble(parts[5]);
            int twoOptTime = Integer.parseInt(parts[6]);
            int nodeSwapTime = Integer.parseInt(parts[7]);
            int nodeShiftTime = Integer.parseInt(parts[8]);

            int optimalLength = optimalLengths.getOrDefault(fileName, -1);
            
            if (optimalLength != -1) {
                double errorTwoOpt = calculateError(afterTwoOpt, optimalLength);
                double errorNodeSwap = calculateError(afterNodeSwap, optimalLength);
                double errorNodeShift = calculateError(afterNodeShift, optimalLength);
                
                List<Double> errors = Arrays.asList(errorTwoOpt, errorNodeSwap, errorNodeShift);
                double standardDeviation = calculateStandardDeviation(errors);
                
                writer.write(String.format("%s,%s,%.2f,%.2f,%.2f,%.2f,%d,%d,%d,%.2f,%.2f,%.2f,%.2f\n",
                        fileName, method, initialTourLength, afterTwoOpt, afterNodeSwap, afterNodeShift,
                        twoOptTime, nodeSwapTime, nodeShiftTime,
                        errorTwoOpt, errorNodeSwap, errorNodeShift, standardDeviation));
            }
        }
        
        reader.close();
        writer.close();
    }
    public static void main(String[] args) {
        try {
            String inputCSV = "tsp_results.csv"; 
            String outputCSV = "output_with_errors.csv";
            
            generateErrorCSV(inputCSV, outputCSV);
            System.out.println("CSV file with errors and standard deviation generated successfully.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    
}
