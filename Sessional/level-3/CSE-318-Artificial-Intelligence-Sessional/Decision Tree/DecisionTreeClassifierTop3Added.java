import java.io.*;
import java.util.*;
import java.util.stream.Collectors;
import java.nio.file.*;

public class DecisionTreeClassifierTop3Added {

    public enum SplitCriterion {
        INFORMATION_GAIN,
        GINI_IMPURITY
    }

    public enum AttributeSelectionStrategy {
        BEST,
        TOP_THREE_RANDOM
    }


    private static final List<Pair<String, List<String>>> ALL_ATTRIBUTES = Arrays.asList(
            new Pair<>("buying", Arrays.asList("vhigh", "high", "med", "low")),
            new Pair<>("maint", Arrays.asList("vhigh", "high", "med", "low")),
            new Pair<>("doors", Arrays.asList("2", "3", "4", "5more")),
            new Pair<>("persons", Arrays.asList("2", "4", "more")),
            new Pair<>("lug_boot", Arrays.asList("small", "med", "big")),
            new Pair<>("safety", Arrays.asList("low", "med", "high"))
    );

    private static class Pair<K, V> {
        private K first;
        private V second;

        Pair(K first, V second) {
            this.first = first;
            this.second = second;
        }

        K getFirst() { return first; }
        V getSecond() { return second; }
    }

    private static class Node {
        int attribute;
        String label;
        Map<String, Node> children;

        Node() {
            this(-1, "");
        }

        Node(int attr, String lbl) {
            this.attribute = attr;
            this.label = lbl;
            this.children = new HashMap<>();
        }
    }

    private static class Dataset {
        List<List<String>> matrix;

        Dataset() {
            matrix = new ArrayList<>();
        }

        Dataset subset(int attrIndex, String attrValue) {
            Dataset subset = new Dataset();
            subset.matrix = matrix.stream()
                    .filter(row -> row.get(attrIndex).equals(attrValue))
                    .collect(Collectors.toList());
            return subset;
        }
    }

    private static Dataset loadDataset() {
        Dataset dataset = new Dataset();
        try (BufferedReader br = new BufferedReader(new FileReader("car.data"))) {
            String line;
            while ((line = br.readLine()) != null) {
                List<String> rowTokens = Arrays.asList(line.split(","));
                dataset.matrix.add(rowTokens);
            }
        } catch (IOException e) {
            System.err.println("Failed to open the dataset file.");
            e.printStackTrace();
        }
        return dataset;
    }

    private static void splitDataset(Dataset dataset, Dataset trainingSet, Dataset testingSet) {
        Collections.shuffle(dataset.matrix, new Random(System.currentTimeMillis()));
        int splitIdx = (int)(dataset.matrix.size() * 0.2);
        trainingSet.matrix.addAll(dataset.matrix.subList(0, splitIdx));
        testingSet.matrix.addAll(dataset.matrix.subList(splitIdx, dataset.matrix.size()));
    }

    // Get the plurality (majority) value
    private static String getPluralityValue(Dataset dataset) {
        Map<String, Integer> labelCounts = new HashMap<>();
        String majorityLabel = "";
        int maxCount = 0;

        for (List<String> instance : dataset.matrix) {
            String label = instance.get(instance.size() - 1);
            labelCounts.put(label, labelCounts.getOrDefault(label, 0) + 1);

            if (labelCounts.get(label) > maxCount) {
                maxCount = labelCounts.get(label);
                majorityLabel = label;
            }
        }
        return majorityLabel;
    }

    private static double calculateEntropy(Dataset dataset) {
        Map<String, Integer> labelCounts = new HashMap<>();
        for (List<String> instance : dataset.matrix) {
            String label = instance.get(instance.size() - 1);
            labelCounts.put(label, labelCounts.getOrDefault(label, 0) + 1);
        }

        double entropy = 0.0;
        for (int count : labelCounts.values()) {
            double probability = 1.0 * count / dataset.matrix.size();
            entropy -= probability * Math.log(probability) / Math.log(2);
        }

        return entropy;
    }

    private static double calculateGiniImpurity(Dataset dataset) {
        Map<String, Integer> labelCounts = new HashMap<>();
        for (List<String> instance : dataset.matrix) {
            String label = instance.get(instance.size() - 1);
            labelCounts.put(label, labelCounts.getOrDefault(label, 0) + 1);
        }

        double gini = 1.0;
        for (int count : labelCounts.values()) {
            double probability = 1.0 * count / dataset.matrix.size();
            gini -= probability * probability;
        }

        return gini;
    }

    private static double calculateInformationGain(Dataset dataset, int attrIdx) {
        int nExamples = dataset.matrix.size();
        double eBs = calculateEntropy(dataset);

        Map<String, Dataset> dataSplits = new HashMap<>();
        for (List<String> instance : dataset.matrix) {
            String attrValue = instance.get(attrIdx);
            dataSplits.putIfAbsent(attrValue, new Dataset());
            dataSplits.get(attrValue).matrix.add(instance);
        }

        double eAs = 0.0;
        for (Dataset split : dataSplits.values()) {
            double probability = 1.0 * split.matrix.size() / nExamples;
            eAs += probability * calculateEntropy(split);
        }

        return eBs - eAs;
    }

    private static double calculateGiniGain(Dataset dataset, int attrIdx) {
        int nExamples = dataset.matrix.size();
        double gBs = calculateGiniImpurity(dataset);

        Map<String, Dataset> dataSplits = new HashMap<>();
        for (List<String> instance : dataset.matrix) {
            String attrValue = instance.get(attrIdx);
            dataSplits.putIfAbsent(attrValue, new Dataset());
            dataSplits.get(attrValue).matrix.add(instance);
        }

        double gAs = 0.0;
        for (Dataset split : dataSplits.values()) {
            double probability = 1.0 * split.matrix.size() / nExamples;
            gAs += probability * calculateGiniImpurity(split);
        }

        return gBs - gAs;
    }

    private static int getBestAttribute(Dataset dataset, Set<Integer> attributes, SplitCriterion criterion) {
        double maxGain = -1.0;
        int bestAttr = -1;
        for (int attrIdx : attributes) {
            double gain = (criterion == SplitCriterion.INFORMATION_GAIN)
                    ? calculateInformationGain(dataset, attrIdx)
                    : calculateGiniGain(dataset, attrIdx);

            if (gain > maxGain) {
                maxGain = gain;
                bestAttr = attrIdx;
            }
        }
        return bestAttr;
    }

    private static int getRandomTopAttribute(Dataset dataset, Set<Integer> attributes, SplitCriterion criterion) {

        List<Pair<Integer, Double>> attributeGains = new ArrayList<>();
        for (int attrIdx : attributes) {
            double gain = (criterion == SplitCriterion.INFORMATION_GAIN)
                    ? calculateInformationGain(dataset, attrIdx)
                    : calculateGiniGain(dataset, attrIdx);
            attributeGains.add(new Pair<>(attrIdx, gain));
        }
        attributeGains.sort((a, b) -> b.getSecond().compareTo(a.getSecond()));

        int topCount = Math.min(3, attributeGains.size());
        List<Pair<Integer, Double>> topAttributes = attributeGains.subList(0, topCount);


        Random random = new Random();
        return topAttributes.get(random.nextInt(topCount)).getFirst();
    }

    private static boolean isSameClassification(Dataset dataset) {
        String uniqueLabel = dataset.matrix.get(0).get(dataset.matrix.get(0).size() - 1);
        return dataset.matrix.stream()
                .allMatch(instance -> instance.get(instance.size() - 1).equals(uniqueLabel));
    }

    private static int depth = 0;

    // Pad for logging
    private static String pad() {
        return "--".repeat(depth);
    }

    private static Node buildDecisionTree(Dataset dataset, Set<Integer> attributes, SplitCriterion criterion, AttributeSelectionStrategy strategy) {
        if (isSameClassification(dataset)) {
            Node leaf = new Node(-1, dataset.matrix.get(0).get(dataset.matrix.get(0).size() - 1));
//            System.err.println(pad() + leaf.label + " (Leaf: All same label)");
            return leaf;
        } else if (attributes.isEmpty()) {
            Node leaf = new Node();
            leaf.label = getPluralityValue(dataset);
//            System.err.println(pad() + leaf.label + " (Leaf: No more attributes)");
            return leaf;
        }

        int bestAttr;
        if (strategy == AttributeSelectionStrategy.BEST) {
            bestAttr = getBestAttribute(dataset, attributes, criterion);
        } else {
            bestAttr = getRandomTopAttribute(dataset, attributes, criterion);
        }

//        System.err.println(pad() + ALL_ATTRIBUTES.get(bestAttr).getFirst() + " (Test)");
        Node decisionNode = new Node();
        decisionNode.attribute = bestAttr;
        attributes.remove(bestAttr);

        for (String attrValue : ALL_ATTRIBUTES.get(bestAttr).getSecond()) {
            Dataset subset = dataset.subset(bestAttr, attrValue);
//            System.err.println(pad() + ALL_ATTRIBUTES.get(bestAttr).getFirst() + "." + attrValue + " (Branch)");

            if (subset.matrix.isEmpty()) {
                Node leaf = new Node();
                leaf.label = getPluralityValue(dataset);
                depth++;
                decisionNode.children.put(attrValue, leaf);
//                System.err.println(pad() + leaf.label + " (Leaf: No more examples)");
                depth--;
            } else {
                depth++;
                decisionNode.children.put(attrValue, buildDecisionTree(subset, new HashSet<>(attributes), criterion, strategy));
                depth--;
            }
        }

        return decisionNode;
    }

    private static Node train(Dataset dataset, Dataset trainingSet, SplitCriterion criterion, AttributeSelectionStrategy strategy) {
        Set<Integer> attributes = new HashSet<>();
        for (int i = 0; i < ALL_ATTRIBUTES.size(); i++) {
            attributes.add(i);
        }
        return buildDecisionTree(trainingSet, attributes, criterion, strategy);
    }

    private static String predictLabel(Node node, List<String> instance) {
        if (!node.label.isEmpty()) {
            return node.label;
        }
        String attrValue = instance.get(node.attribute);
        return predictLabel(node.children.get(attrValue), instance);
    }

    private static double classify(Node decisionTree, Dataset testingSet) {
        int correctPredictions = 0;
        int nTests = testingSet.matrix.size();

        for (List<String> instance : testingSet.matrix) {
            String trueLabel = instance.get(instance.size() - 1);
            String predictedLabel = predictLabel(decisionTree, instance);

            if (predictedLabel.equals(trueLabel)) {
                correctPredictions++;
            }
        }
        return 1.0 * correctPredictions / nTests;
    }
    private static void runExperimentsAndLogToCSV(
            int nExperiments,
            SplitCriterion[] criterions,
            AttributeSelectionStrategy[] strategies,
            String outputFilePath
    ) {
        try (PrintWriter csvWriter = new PrintWriter(new FileWriter(outputFilePath))) {

            csvWriter.println("Exp_No,Info_Gain,Info_Gain(With_Random),Gini_Impurity,Gini_Impurity(With_Random");
            Dataset dataset = loadDataset();
            for (int i = 0; i < nExperiments; ++i) {
                Dataset trainingSet = new Dataset();
                Dataset testingSet = new Dataset();
                splitDataset(dataset, trainingSet, testingSet);

                Node decisionTree1 = train(dataset, trainingSet, criterions[0], strategies[0]);
                double accuracy1 = classify(decisionTree1, testingSet)*100;
                Node decisionTree2 = train(dataset, trainingSet, criterions[0], strategies[1]);
                double accuracy2 = classify(decisionTree2, testingSet)*100;
                Node decisionTree3 = train(dataset, trainingSet, criterions[1], strategies[0]);
                double accuracy3 = classify(decisionTree3, testingSet)*100;
                Node decisionTree4 = train(dataset, trainingSet, criterions[1], strategies[1]);
                double accuracy4 = classify(decisionTree4, testingSet)*100;
                csvWriter.printf("%d,%.2f,%.2f,%.2f,%.2f\n", i + 1, accuracy1, accuracy2, accuracy3, accuracy4);
            }
            System.out.println("Individual experiment results written to " + outputFilePath);
        } catch (IOException e) {
            System.err.println("Error writing to CSV file: " + e.getMessage());
        }
    }
    private static void calculateAndPrintSummaryStatistics(String csvFilePath) throws Exception {

        List<String[]> rows = readCSV(csvFilePath);

        String[] header = rows.get(0);
        List<String[]> data = rows.subList(1, rows.size());

        double infoGainSum = 0.0, infoGainWithRandomSum = 0.0;
        double giniImpuritySum = 0.0, giniImpurityWithRandomSum = 0.0;
        int rowCount = data.size();

        for (String[] row : data) {
            infoGainSum += Double.parseDouble(row[1]);
            infoGainWithRandomSum += Double.parseDouble(row[2]);
            giniImpuritySum += Double.parseDouble(row[3]);
            giniImpurityWithRandomSum += Double.parseDouble(row[4]);
        }

        double avgInfoGain = infoGainSum / rowCount;
        double avgInfoGainWithRandom = infoGainWithRandomSum / rowCount;
        double avgGiniImpurity = giniImpuritySum / rowCount;
        double avgGiniImpurityWithRandom = giniImpurityWithRandomSum / rowCount;

        System.out.printf("Average Info Gain: %.2f%n", avgInfoGain);
        System.out.printf("Average Info Gain (With Random): %.2f%n", avgInfoGainWithRandom);
        System.out.printf("Average Gini Impurity: %.2f%n", avgGiniImpurity);
        System.out.printf("Average Gini Impurity (With Random): %.2f%n", avgGiniImpurityWithRandom);
    }
    private static List<String[]> readCSV(String filePath) throws IOException {
        List<String[]> rows = new ArrayList<>();

        // Read all lines from the file
        List<String> lines = Files.readAllLines(Paths.get(filePath));

        // Process each line
        for (String line : lines) {
            // Split each line by commas and add it to the rows list
            String[] values = line.split(",");
            rows.add(values);
        }

        return rows;
    }
    public static void main(String[] args) throws Exception {

        SplitCriterion[] criterions = {
                SplitCriterion.INFORMATION_GAIN,
                SplitCriterion.GINI_IMPURITY,
        };

        AttributeSelectionStrategy[] strategies = {
                AttributeSelectionStrategy.BEST,
                AttributeSelectionStrategy.TOP_THREE_RANDOM,
        };

        int nExperiments = 20;
        String csvFile = "decision_tree_individual_runs.csv";
        DecisionTreeClassifierTop3Added.runExperimentsAndLogToCSV(
                nExperiments, criterions, strategies,
                csvFile
        );
        DecisionTreeClassifierTop3Added.calculateAndPrintSummaryStatistics(csvFile);
    }
}