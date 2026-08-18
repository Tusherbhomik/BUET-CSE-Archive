import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;

class City {
    private final int id;
    private final double x;
    private final double y;

    public City(int id, double x, double y) {
        this.id = id;
        this.x = x;
        this.y = y;
    }

    public double distance(City other) {
        return Math.sqrt(Math.pow(this.x - other.x, 2) + Math.pow(this.y - other.y, 2));
    }

    public int getId() {
        return id;
    }
}

class TSPSolver {
    private final List<City> cities;
    private final int numCities;
    private final double[][] distances;

    public TSPSolver(List<City> cities) {
        this.cities = cities;
        this.numCities = cities.size();
        this.distances = new double[numCities][numCities];
        for (int i = 0; i < numCities; i++) {
            for (int j = 0; j < numCities; j++) {
                distances[i][j] = cities.get(i).distance(cities.get(j));
            }
        }
    }

    public double calculateTourLength(List<Integer> tour) {
        // System.out.println(Main.formatTour(tour));
        double length = 0;
        for (int i = 0; i < tour.size() - 1; i++) {
            length += distances[tour.get(i)][tour.get(i + 1)];
        }
        // System.out.println(length);
        return length;
    }

    public List<Integer> nearestNeighbor(Integer startCity) {
        if (startCity == null) {
            startCity = new Random().nextInt(numCities);
            // System.out.println(startCity);
        }

        Set<Integer> unvisited = new HashSet<>();
        for (int i = 0; i < numCities; i++) {
            unvisited.add(i);
        }

        List<Integer> tour = new ArrayList<>();
        tour.add(startCity);
        unvisited.remove(startCity);

        while (!unvisited.isEmpty()) {
            int lastCity = tour.get(tour.size() - 1);
            double highValue = 100000000.00;
            double minDistance = highValue;
            int nextCity = -1;

            for (int city : unvisited) {
                // for (int a : unvisited) {
                // System.out.println(a);
                // }
                if (distances[lastCity][city] < minDistance) {
                    minDistance = distances[lastCity][city];
                    nextCity = city;
                    // System.out.println(nextCity);
                }
            }
            // System.out.println(Main.formatTour(tour));

            tour.add(nextCity);
            unvisited.remove(nextCity);
        }
        tour.add(startCity);
        // System.out.println(Main.formatTour(tour));

        return tour;
    }

    public List<Integer> nearestInsertion() {
        int startNode = new Random().nextInt(numCities);
        List<Integer> tour = new ArrayList<>();
        tour.add(startNode);

        Set<Integer> unvisited = new HashSet<>();
        for (int i = 0; i < numCities; i++) {
            if (i != startNode) {
                unvisited.add(i);
            }
        }
        double highValue = 100000000.00;
        double minDistance = highValue;
        int closestNode = -1;
        for (int r : unvisited) {
            // System.out.println(distances[startNode][r]);
            if (distances[startNode][r] < minDistance) {
                minDistance = distances[startNode][r];
                // System.out.println(r);
                closestNode = r;
            }
        }
        tour.add(closestNode);
        unvisited.remove(closestNode);

        while (!unvisited.isEmpty()) {
            minDistance = Double.POSITIVE_INFINITY;
            int selectedNode = -1;
            for (int r : unvisited) {
                for (int j : tour) {
                    if (distances[r][j] < minDistance) {
                        minDistance = distances[r][j];
                        selectedNode = r;
                        // System.out.println(selectedNode);
                    }
                }
            }
            double minCost = Double.POSITIVE_INFINITY;
            int insertPosition = -1;

            for (int i = 0; i < tour.size(); i++) {
                int j = (i + 1) % tour.size();
                int currentCity = tour.get(i);
                int nextCity = tour.get(j);
                // System.out.println(currentCity);
                // System.out.println(nextCity);
                double insertionCost = distances[currentCity][selectedNode] +
                        distances[selectedNode][nextCity] -
                        distances[currentCity][nextCity];

                if (insertionCost < minCost) {
                    // System.out.println(insertionCost);
                    minCost = insertionCost;
                    insertPosition = i + 1;
                }
            }

            tour.add(insertPosition, selectedNode);
            unvisited.remove(selectedNode);
        }
        tour.add(startNode);
        // System.out.println(Main.formatTour(tour));

        return tour;
    }

    public List<Integer> cheapestInsertion() {
        int startCity = new Random().nextInt(numCities);
        int nearestCity = -1;
        double highValue = 100000000.00;
        double minDistance = highValue;

        for (int i = 0; i < numCities; i++) {
            if (i != startCity && distances[startCity][i] < minDistance) {
                minDistance = distances[startCity][i];
                nearestCity = i;
                // System.out.println(nearestCity);
            }
        }
        // System.out.println(minDistance);
        List<Integer> tour = new ArrayList<>();
        tour.add(startCity);
        tour.add(nearestCity);
        Set<Integer> unvisited = new HashSet<>();
        for (int i = 0; i < numCities; i++) {
            if (i != startCity && i != nearestCity) {
                unvisited.add(i);
            }
        }
        // System.out.println(unvisited.size());

        while (!unvisited.isEmpty()) {
            double minIncrease = 100000000.00;
            int bestCity = -1;
            int bestInsertionIndex = -1;
            for (int city : unvisited) {
                for (int i = 0; i < tour.size() - 1; i++) {
                    int current = tour.get(i);
                    int next = tour.get(i + 1);
                    // System.out.println(current);
                    // System.out.println(next);
                    double increase = distances[current][city] +
                            distances[city][next] -
                            distances[current][next];
                    // System.out.println(increase);

                    if (increase < minIncrease) {
                        minIncrease = increase;
                        // System.out.println(minIncrease);
                        // here is problem --> thik thak increase hocche na i guess
                        bestCity = city;
                        bestInsertionIndex = i + 1;
                    }
                }
            }
            // System.out.println(bestInsertionIndex);
            // System.out.println(bestCity);
            tour.add(bestInsertionIndex, bestCity);
            unvisited.remove(bestCity);
        }
        tour.add(startCity);
        // System.out.println(Main.formatTour(tour));

        return tour;
    }

    public List<Integer> twoOpt(List<Integer> tour, int maxIterations) {
        int n = tour.size();
        double bestDistance = calculateTourLength(tour);
        // System.out.println(bestDistance);
        boolean improvement = true;
        int iteration = 0;

        while (improvement && iteration < maxIterations) {
            improvement = false;
            iteration++;
            // System.out.println(iteration);

            for (int i = 1; i < n - 2; i++) {
                for (int j = i + 1; j < n - 1; j++) {
                    List<Integer> newTour = new ArrayList<>(tour);
                    reverseSubArray(newTour, i, j);
                    // System.out.println("debug");
                    // System.out.println(newTour.get(i));
                    // System.out.println(newTour.get(j));
                    double newDistance = calculateTourLength(newTour);
                    if (newDistance < bestDistance) {
                        tour = new ArrayList<>(newTour);
                        bestDistance = newDistance;
                        improvement = true;
                    }
                }
            }
        }
        // System.out.println(bestDistance);

        return tour;
    }

    private void reverseSubArray(List<Integer> tour, int i, int j) {
        while (i < j) {
            Collections.swap(tour, i, j);
            i++;
            j--;
        }
    }

    public List<Integer> nodeSwap(List<Integer> tour, int maxIterations) {
        int n = tour.size();
        double bestDistance = calculateTourLength(tour);
        boolean improvement = true;
        int iteration = 0;

        while (improvement && iteration < maxIterations) {
            improvement = false;
            iteration++;
            // System.out.println(iteration);
            for (int i = 1; i < n - 1; i++) {
                for (int j = i + 1; j < n - 1; j++) {
                    List<Integer> newTour = new ArrayList<>(tour);
                    int temp = newTour.get(i);
                    // System.out.println(i);
                    // System.out.println(j);
                    newTour.set(i, newTour.get(j));
                    newTour.set(j, temp);
                    double newDistance = calculateTourLength(newTour);
                    // System.out.println(newDistance);
                    // System.out.println(bestDistance);
                    if (newDistance < bestDistance) {
                        tour = new ArrayList<>(newTour);
                        bestDistance = newDistance;
                        improvement = true;
                    }
                }
            }
        }

        return tour;
    }

    public List<Integer> nodeShift(List<Integer> tour, int maxIterations) {
        List<Integer> bestTour = new ArrayList<>(tour);
        double bestLength = calculateTourLength(bestTour);

        boolean improved = true;
        int iteration = 0;

        while (improved && iteration < maxIterations) {
            improved = false;
            iteration++;
            // System.out.println(iteration);

            for (int i = 1; i < bestTour.size() - 1; i++) {
                for (int j = 1; j < bestTour.size() - 1; j++) {
                    if (i != j) {
                        List<Integer> newTour = new ArrayList<>(bestTour);
                        int city = newTour.remove(i);
                        // System.out.println("removed city");
                        // System.out.println(city);
                        newTour.add(j, city);
                        double newLength = calculateTourLength(newTour);
                        if (newLength < bestLength) {
                            bestTour = newTour;
                            bestLength = newLength;
                            improved = true;
                        }
                    }
                }
            }
        }
        // System.out.println(bestLength);
        // System.out.println(Main.formatTour(bestTour));

        return bestTour;
    }

}

class TSPFileReader {
    public static List<City> readTSPFile(String filename) throws IOException {
        List<City> cities = new ArrayList<>();
        boolean readingCoords = false;
        // System.out.println(filename);

        BufferedReader reader = new BufferedReader(new FileReader(filename));
            String line;
            while ((line = reader.readLine()) != null) {
                line = line.trim();
                if (line.equals("NODE_COORD_SECTION")) {
                    readingCoords = true;
                    continue;
                } else if (line.equals("EOF")) {
                    break;
                }

                if (readingCoords) {
                    String[] parts = line.split("\\s+");
                    int id = Integer.parseInt(parts[0]);
                    double x = Double.parseDouble(parts[1]);
                    double y = Double.parseDouble(parts[2]);
                    cities.add(new City(id - 1, x, y));
                }
            }
            reader.close();
        
        // System.out.println(cities.size());
        return cities;
    }
}

class TSPResult {
    private final String fileName;
    private final String constructiveMethod;
    private final double initialTourLength;
    private final double twoOptLength;
    private final double nodeSwapLength;
    private final double nodeShiftLength;
    private final long twoOptTime;
    private final long nodeSwapTime;
    private final long nodeShiftTime;

    public TSPResult(String fileName, String constructiveMethod, double initialTourLength,
            double twoOptLength, double nodeSwapLength, double nodeShiftLength,
            long twoOptTime, long nodeSwapTime, long nodeShiftTime) {
        this.fileName = fileName;
        this.constructiveMethod = constructiveMethod;
        this.initialTourLength = initialTourLength;
        this.twoOptLength = twoOptLength;
        this.nodeSwapLength = nodeSwapLength;
        this.nodeShiftLength = nodeShiftLength;
        this.twoOptTime = twoOptTime;
        this.nodeSwapTime = nodeSwapTime;
        this.nodeShiftTime = nodeShiftTime;
    }

    public String[] toStringArray() {
        DecimalFormat df = new DecimalFormat("#.##");
        return new String[] {
                fileName,
                constructiveMethod,
                df.format(initialTourLength),
                df.format(twoOptLength),
                df.format(nodeSwapLength),
                df.format(nodeShiftLength),
                String.valueOf(twoOptTime),
                String.valueOf(nodeSwapTime),
                String.valueOf(nodeShiftTime)
        };
    }
}

class ResultsWriter {
    private final String outputFilePath;
    private final List<TSPResult> results;

    public ResultsWriter(String outputFilePath, List<TSPResult> results) {
        this.outputFilePath = outputFilePath;
        this.results = results;
    }

    public void writeToCSV(String[] headers) throws IOException {
        FileWriter writer = new FileWriter(outputFilePath);
            if (headers != null && headers.length > 0) {
                writer.write(String.join(",", headers) + "\n");
            }
            for (TSPResult result : results) {
                writer.write(String.join(",", result.toStringArray()) + "\n");
            }
            writer.close();
        
    }
}

public class Main {
    public static String formatTour(List<Integer> tour) {
        if (tour == null || tour.isEmpty()) {
            return "Tour is empty!";
        }

        StringBuilder result = new StringBuilder();
        for (int i = 0; i < tour.size(); i++) {
            result.append(tour.get(i));
            if (i < tour.size() - 1) {
                result.append("-->");
            }
        }
        return result.toString();
    }

    public static void main(String[] args) {
        String directoryPath = "tspFiles";
        String outputCSV = "tsp_results.csv";
        String[] HEADERS = {
                "File Name",
                "Constructive Method",
                "Initial Tour Length",
                "After 2-opt",
                "After Node Swap",
                "After Node Shift",
                "2-opt Time (ms)",
                "Node Swap Time (ms)",
                "Node Shift Time (ms)"
        };
        List<TSPResult> allResults = new ArrayList<>();

        try {
            Files.list(Path.of(directoryPath))
                    .filter(path -> path.toString().endsWith(".tsp"))
                    .forEach(path -> {
                        try {
                            String fileName = path.getFileName().toString();
                            List<City> cities = TSPFileReader.readTSPFile(path.toString());
                            TSPSolver solver = new TSPSolver(cities);
                            allResults.addAll(processFile(solver, fileName));
                        } catch (Exception e) {
                            System.err.println(e.getMessage());
                        }
                    });
                    // System.out.println(allResults.size());
            ResultsWriter writer = new ResultsWriter(outputCSV, allResults);
            // System.out.println("hello");
            writer.writeToCSV(HEADERS);
            System.out.println("done");
        } catch (IOException e) {
            System.err.println(e.getMessage());
        }
    }

    private static List<TSPResult> processFile(TSPSolver solver, String fileName) {
        List<TSPResult> results = new ArrayList<>();
        List<Integer> nnTour = solver.nearestNeighbor(null);

        // System.out.println("Printing Size "+fileName);
        // System.out.println(nnTour.size());
        // System.out.println(formatTour(nnTour));
        // System.out.println("________________________________");
        // System.out.println("________________________________");
        // System.out.println("________________________________");
        // System.out.println("________________________________");

        List<Integer> niTour = solver.nearestInsertion();
        // System.out.println("Printing Size "+fileName);
        // System.out.println(nnTour.size());
        // System.out.println(formatTour(nnTour));
        // System.out.println("________________________________");
        // System.out.println("________________________________");
        // System.out.println("________________________________");
        // System.out.println("________________________________");
        List<Integer> ciTour = solver.cheapestInsertion();

        results.add(runImprovementMethods(solver, fileName, "Nearest Neighbor", new ArrayList<>(nnTour)));
        results.add(runImprovementMethods(solver, fileName, "Nearest Insertion", new ArrayList<>(niTour)));
        results.add(runImprovementMethods(solver, fileName, "Cheapest Insertion", new ArrayList<>(ciTour)));

        return results;
    }

    private static TSPResult runImprovementMethods(TSPSolver solver, String fileName, String methodName,
            List<Integer> initialTour) {
        long start, end;

        start = System.nanoTime();
        List<Integer> improvedTwoOpt = solver.twoOpt(new ArrayList<>(initialTour), 10000);
        end = System.nanoTime();
        long twoOptTime = (end - start) / 1_000_000;

        start = System.nanoTime();
        List<Integer> improvedNodeSwap = solver.nodeSwap(new ArrayList<>(initialTour), 10000);
        end = System.nanoTime();
        long nodeSwapTime = (end - start) / 1_000_000;

        start = System.nanoTime();
        List<Integer> improvedNodeShift = solver.nodeShift(new ArrayList<>(initialTour), 10000);
        end = System.nanoTime();
        long nodeShiftTime = (end - start) / 1_000_000;

        return new TSPResult(
                fileName,
                methodName,
                solver.calculateTourLength(initialTour),
                solver.calculateTourLength(improvedTwoOpt),
                solver.calculateTourLength(improvedNodeSwap),
                solver.calculateTourLength(improvedNodeShift),
                twoOptTime,
                nodeSwapTime,
                nodeShiftTime);
    }
}
