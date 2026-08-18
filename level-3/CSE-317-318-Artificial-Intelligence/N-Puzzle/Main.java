import java.util.*;

class Position {
    public int row;
    public int col;

    Position(int x, int y) {
        this.row = x;
        this.col = y;
    }
}

class Grid {

    private String[][] grid;

    Grid(String[][] grid) {
        this.grid = grid;
    }

    String[][] getGrid() {
        return grid;
    }

    void showGrid() {
        for (String[] strings : grid) {
            for (int j = 0; j < grid[0].length; j++) {
                System.out.print(strings[j] + " ");
            }
            System.out.println();
        }
    }

    boolean equals(Grid otherGrid) {
        String[][] other = otherGrid.getGrid();
        for (int i = 0; i < grid.length; i++) {
            for (int j = 0; j < grid[i].length; j++) {
                if (!grid[i][j].equals(other[i][j])) {
                    return false;
                }
            }
        }
        return true;
    }
}

class GridState {
    private Grid grid;

    public int gValue;
    public int hValue;
    public int fValue;
    GridState parentState;
    boolean useHamming;


    public GridState(Grid grid, int gValue, GridState parentState,boolean useHamming) {
        this.grid = grid;
        this.gValue = gValue;
        this.fValue = calculateFValue();
        this.parentState = parentState;
        this.useHamming=useHamming;
    }

    public int calculateFValue() {

            if(useHamming){
                hValue = calculateHammingDistance();
            }else {
                hValue=calculateManhattanDistance();
            }

        return gValue + hValue;
    }

    int calculateHammingDistance() {
        int distance = 0;
        String[][] currentGrid = grid.getGrid();
        String[][] goalGrid = Main.goal;

        for (int i = 0; i < currentGrid.length; i++) {
            for (int j = 0; j < currentGrid[i].length; j++) {

                if (!currentGrid[i][j].equals("*") && !currentGrid[i][j].equals(goalGrid[i][j])) {
                    distance++;
                }
            }
        }
        return distance;
    }

    int calculateManhattanDistance() {
        int distance = 0;
        String[][] currentGrid = grid.getGrid();
        String[][] goalGrid = Main.goal;

        for (int i = 0; i < currentGrid.length; i++) {
            for (int j = 0; j < currentGrid[i].length; j++) {
                String currentValue = currentGrid[i][j];
                if (!currentValue.equals("*")) {
                    int[] goalPosition = findPosition(goalGrid, currentValue);
                    int targetRow = goalPosition[0];
                    int targetCol = goalPosition[1];
                    distance += Math.abs(i - targetRow) + Math.abs(j - targetCol);
                }
            }
        }
        return distance;
    }


    private int[] findPosition(String[][] grid, String value) {
        for (int i = 0; i < grid.length; i++) {
            for (int j = 0; j < grid[i].length; j++) {
                if (grid[i][j].equals(value)) {
                    return new int[]{i, j};
                }
            }
        }
        return null;
    }

    Grid getGrid() {
        return grid;
    }

    private Position findBlankPosition(String[][] grid) {
        for (int i = 0; i < grid.length; i++) {
            for (int j = 0; j < grid[i].length; j++) {
                if (grid[i][j].equals("*")) {
                    return new Position(i, j);
                }
            }
        }
        return null;
    }

    public List<GridState> generateNeighboringGridStates() {
        List<GridState> neighborStates = new ArrayList<>();
        String[][] currentGrid = grid.getGrid();
        int row = currentGrid.length;
        int col = currentGrid[0].length;
        Position blankPosition = findBlankPosition(currentGrid);

        List<Position> neighboringPositions = new ArrayList<>();

        //right move
        if (blankPosition.col < col - 1) {
            neighboringPositions.add(new Position(blankPosition.row, blankPosition.col + 1));
        }
        //left move
        if (blankPosition.col > 0) {
            neighboringPositions.add(new Position(blankPosition.row, blankPosition.col - 1));
        }
        //down move
        if (blankPosition.row < row - 1) {
            neighboringPositions.add(new Position(blankPosition.row + 1, blankPosition.col));
        }
        //up move
        if (blankPosition.row > 0) {
            neighboringPositions.add(new Position(blankPosition.row - 1, blankPosition.col));
        }

        for (Position position : neighboringPositions) {
            String[][] newGridArray = deepCopyGrid(currentGrid);
            swapPositions(newGridArray, blankPosition.row, blankPosition.col, position.row, position.col);
            Grid newGrid = new Grid(newGridArray);
            GridState neighborState = new GridState(newGrid, gValue + 1, this,this.useHamming);
            neighborStates.add(neighborState);

        }

        return neighborStates;
    }

    private String[][] deepCopyGrid(String[][] originalGrid) {
        String[][] newGrid = new String[originalGrid.length][originalGrid[0].length];
        for (int i = 0; i < originalGrid.length; i++) {
            for (int j = 0; j < originalGrid[i].length; j++) {
                newGrid[i][j] = originalGrid[i][j];
            }
        }
        return newGrid;
    }

    private void swapPositions(String[][] grid, int x1, int y1, int x2, int y2) {
        String temp = grid[x1][y1];
        grid[x1][y1] = grid[x2][y2];
        grid[x2][y2] = temp;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null || getClass() != obj.getClass()) return false;
        GridState other = (GridState) obj;
        return this.grid.equals(other.grid);
    }

    @Override
    public int hashCode() {
        return Arrays.deepHashCode(grid.getGrid());
    }
}

class GridStateComparator implements Comparator<GridState> {

    @Override
    public int compare(GridState o1, GridState o2) {
        int fValueComparison = Integer.compare(o1.calculateFValue(), o2.calculateFValue());
        if (fValueComparison == 0) {
            return Integer.compare(o1.hValue, o2.hValue);
        }
        return fValueComparison;
    }
}

public class Main {
    static String[][] goal = {
            {"1", "2", "3"},
            {"4", "5", "6"},
            {"7", "8", "*"}
    };

    private static boolean isSolvable(String[][] grid, int k) {
        int[] tempArr = new int[k * k];
        int tempArrIndex = 0;
        int invCnt = 0;
        int blankRow = 0;
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                if (grid[i][j].equals("*")) {
                    blankRow = i;
                    tempArr[tempArrIndex++] = 0;
                } else {
                    tempArr[tempArrIndex++] = Integer.parseInt(grid[i][j]);
                }
            }
        }
        for (int i = 0; i < tempArr.length; i++) {
            for (int j = i + 1; j < tempArr.length; j++) {
                if (tempArr[i] != 0 && tempArr[j] != 0 && tempArr[i] > tempArr[j]) {
                    invCnt++;
                }
            }
        }

        if (k % 2 != 0) {
            return invCnt % 2 == 0;
        } else {
            int fromBottom = k - blankRow;

            if (fromBottom % 2 == 0) {
                return invCnt % 2 != 0;
            } else {
                return invCnt % 2 == 0;
            }
        }

    }

    private static void handleHeuristic(Grid initialGrid, Grid goalGrid,boolean useHamming) {

        PriorityQueue<GridState> openList = new PriorityQueue<>(new GridStateComparator());
        Set<GridState> closedSet = new HashSet<>();
        GridState initState = new GridState(initialGrid, 0, null,useHamming);
        openList.add(initState);

        int step = 0;
        int explored=0;
        int expanded =0;

        while (!openList.isEmpty()) {
            GridState currentState = openList.poll();
            expanded++;
            closedSet.add(currentState);

            if (currentState.getGrid().equals(goalGrid)) {
                GridState current = currentState;
                List<GridState> path = new ArrayList<>();
                while (current != null) {
                    path.add(current);
                    current = current.parentState;
                }
                Collections.reverse(path);
                for (GridState gridState : path) {
                    System.out.println("Step: " + gridState.gValue);
                    gridState.getGrid().showGrid();
                    System.out.println();
                }
                break;
            }

            for (GridState neighboringGrid : currentState.generateNeighboringGridStates()) {
                if (!closedSet.contains(neighboringGrid) && !openList.contains(neighboringGrid)) {
                    openList.add(neighboringGrid);
                    explored++;
                }
            }
        }

        System.out.println("Expanded state: "+expanded);
        System.out.println("Explored state: "+explored);

    }

    private static String[][] takeGridInput() {
        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        scanner.nextLine();
        String[][] grid = new String[n][n];
        for (int i = 0; i < n; i++) {
            String row = scanner.nextLine();
            grid[i] = row.split(" ");
        }

        return grid;
    }

    private static String[][] generateGoalGrid(int n) {
        String[][] goalGrid = new String[n][n];
        int num = 1;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == n - 1 && j == n - 1) {
                    goalGrid[i][j] = "*";
                } else {
                    goalGrid[i][j] = String.valueOf(num++);
                }
            }
        }
        return goalGrid;
    }



    public static void main(String[] args) {
        String[][] startGrid = takeGridInput();
        Main.goal= generateGoalGrid(startGrid.length);
        System.out.println("--------------------------------------------------------");
        System.out.println("Provided Puzzle ");
        for (int i = 0; i < startGrid.length; i++) {
            for (int j = 0; j < startGrid[i].length; j++) {
                System.out.print(startGrid[i][j] + " ");
            }
            System.out.println();
        }


        //solve ability check
        if (isSolvable(startGrid, startGrid.length)) {
            System.out.println("This puzzle is solvable");
            System.out.println("--------------------------------------------------------");
            Grid initialGrid = new Grid(startGrid);
            Grid goalGrid = new Grid(goal);

            System.out.println("Using Hamming Distance");
            handleHeuristic(initialGrid, goalGrid,true);
            System.out.println("--------------------------------------------------------");
            System.out.println("Using Manhattan Distance");
            handleHeuristic(initialGrid, goalGrid,false);

        } else {
            System.out.println("This puzzle is Not solvable");
            System.out.println("--------------------------------------------------------");
        }
    }
}

//i am taking input from a file  , name Grid.txt
// and my goal state is hardcoded
//static String[][] goal = {
//        {"1", "2", "3","4"},
//        {"5","6","7","8"},
//        {"9","10","11","12"},
//        {"13","14","15","*"}
//};
