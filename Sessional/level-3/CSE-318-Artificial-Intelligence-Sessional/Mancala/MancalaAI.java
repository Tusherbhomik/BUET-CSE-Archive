import java.util.Random;

public class MancalaAI {
    private static final int MAX_DEPTH = 12;

    private int player1Heuristic;
    private int player2Heuristic;

    private int W1;
    private int W2;
    private int W3;
    private int W4;
    public MancalaAI() {
        Random rand = new Random();
        this.player1Heuristic = rand.nextInt(4) + 1;
        this.player2Heuristic = rand.nextInt(4) + 1;
        this.W1 = rand.nextInt(10) ;
        this.W2 = rand.nextInt(10) ;
        this.W3 = rand.nextInt(10) ;
        this.W4 = rand.nextInt(10) ;
    }

    public int chooseBestMove(MancalaBoard board, boolean isPlayer1) {
        int bestValue = Integer.MIN_VALUE;
        int bestMove = -1;
        int start = isPlayer1 ? 0 : 7;
        int end = isPlayer1 ? 6 : 13;

        for (int move = start; move < end; move++) {
            if (board.isValidMove(move, isPlayer1)) {
                MancalaBoard newBoard = new MancalaBoard(board);
                boolean extraTurn = newBoard.makeMove(move, isPlayer1);
                int moveValue = minimax(newBoard, MAX_DEPTH, Integer.MIN_VALUE, Integer.MAX_VALUE, !extraTurn, isPlayer1);
                if (moveValue > bestValue) {
                    bestValue = moveValue;
                    bestMove = move;
                }
            }
        }

        return bestMove;
    }

    public int minimax(MancalaBoard board, int depth, int alpha, int beta, boolean isMaximizingPlayer, boolean isPlayer1) {
        if (depth == 0 || board.isGameOver()) {
            return evaluateBoard(board, isPlayer1);
        }
        if (isMaximizingPlayer) {
            int maxEval = Integer.MIN_VALUE;
            for (int move = 0; move < 6; move++) {
                if (board.isValidMove(move, true)) {
                    MancalaBoard newBoard = new MancalaBoard(board);
                    boolean extraTurn = newBoard.makeMove(move, true);
                    int eval = minimax(newBoard, depth - 1, alpha, beta, !extraTurn, true);
                    maxEval = Math.max(maxEval, eval);
                    alpha = Math.max(alpha, eval);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
            return maxEval;
        } else {
            int minEval = Integer.MAX_VALUE;
            for (int move = 7; move < 13; move++) {
                if (board.isValidMove(move, false)) {
                    MancalaBoard newBoard = new MancalaBoard(board);
                    newBoard.makeMove(move, false);
                    int eval = minimax(newBoard, depth - 1, alpha, beta, true, false);
                    minEval = Math.min(minEval, eval);
                    beta = Math.min(beta, eval);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
            return minEval;
        }
    }

    public int evaluateBoard(MancalaBoard board, boolean isPlayer1) {
        if (isPlayer1) {
            return applyHeuristic(board, player1Heuristic, true);
        } else {
            return applyHeuristic(board, player2Heuristic, false);
        }
    }

    public int applyHeuristic(MancalaBoard board, int heuristic, boolean isPlayer1) {
        int stonesInMyStorage = isPlayer1 ? board.getPlayer1Storage() : board.getPlayer2Storage();
        int stonesInOpponentsStorage = isPlayer1 ? board.getPlayer2Storage() : board.getPlayer1Storage();
        int stonesOnMySide = isPlayer1 ? board.getPlayer1SideStones() : board.getPlayer2SideStones();
        int stonesOnOpponentsSide = isPlayer1 ? board.getPlayer2SideStones() : board.getPlayer1SideStones();
        int additionalMoveEarned = board.hasExtraTurn(isPlayer1) ? 1 : 0;
        int stonesCaptured = board.getStonesCaptured(isPlayer1);

        switch (heuristic) {
            case 1:
                return stonesInMyStorage - stonesInOpponentsStorage;
            case 2:
                return W1 * (stonesInMyStorage - stonesInOpponentsStorage) + W2 * (stonesOnMySide - stonesOnOpponentsSide);
            case 3:
                return W1 * (stonesInMyStorage - stonesInOpponentsStorage) + W2 * (stonesOnMySide - stonesOnOpponentsSide) + W3 * additionalMoveEarned;
            case 4:
                return W1 * (stonesInMyStorage - stonesInOpponentsStorage) + W2 * (stonesOnMySide - stonesOnOpponentsSide) + W3 * additionalMoveEarned + W4 * stonesCaptured;
            default:
                throw new IllegalArgumentException("Unknown heuristic: " + heuristic);
        }
    }
}
