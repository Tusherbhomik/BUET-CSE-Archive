public class MancalaBoard {
    private int[] board;
    private final int TOTAL_BINS = 14;
    private int player1StonesCaptured = 0;
    private int player2StonesCaptured = 0;
    private boolean extraTurnForPlayer1 = false;
    private boolean extraTurnForPlayer2 = false;

    public MancalaBoard() {
        board = new int[TOTAL_BINS];
        initializeBoard();
    }
    public MancalaBoard(MancalaBoard originalBoard) {
        this.board = new int[TOTAL_BINS];
        System.arraycopy(originalBoard.board, 0, this.board, 0, TOTAL_BINS);
    }

    private void initializeBoard() {
        for (int i = 0; i < 6; i++) {
            board[i] = 4;
            board[i + 7] = 4;
        }
        board[6] = 0;
        board[13] = 0;
    }
    public int getPlayer1Storage() {
        return board[6];
    }

    public int getPlayer2Storage() {
        return board[13];
    }
    public int getPlayer1SideStones() {
        int totalStones = 0;
        for (int i = 0; i < 6; i++) {
            totalStones += board[i];
        }
        return totalStones;
    }
    public int getPlayer2SideStones() {
        int totalStones = 0;
        for (int i = 7; i < 13; i++) {
            totalStones += board[i];
        }
        return totalStones;
    }
    public boolean hasExtraTurn(boolean isPlayer1) {
        return isPlayer1 ? extraTurnForPlayer1 : extraTurnForPlayer2;
    }
    public int getStonesCaptured(boolean isPlayer1) {
        return isPlayer1 ? player1StonesCaptured : player2StonesCaptured;
    }
    public void printBoard() {
        System.out.println("--------------------------Printing board Status--------------------------");
        System.out.println("Player 1's bins: " + java.util.Arrays.toString(java.util.Arrays.copyOfRange(board, 0, 6)));
        System.out.println("Player 1's storage: " + board[6]);
        System.out.println("Player 2's bins: " + java.util.Arrays.toString(java.util.Arrays.copyOfRange(board, 7, 13)));
        System.out.println("Player 2's storage: " + board[13]);
        System.out.println("---------------------------------------------------------------------------");
    }
    public boolean isValidMove(int binIndex, boolean isPlayer1) {
        return isPlayer1 ? binIndex >= 0 && binIndex <= 5 && board[binIndex] > 0
                : binIndex >= 7 && binIndex <= 12 && board[binIndex] > 0;
    }
    public boolean makeMove(int binIndex, boolean isPlayer1) {
        if (!isValidMove(binIndex, isPlayer1)) return false;

        int stones = board[binIndex];
        board[binIndex] = 0;
        int currentIndex = binIndex;

        while (stones > 0) {
            currentIndex = (currentIndex + 1) % 14;
            if ((isPlayer1 && currentIndex == 13) || (!isPlayer1 && currentIndex == 6)) continue;
            board[currentIndex]++;
            stones--;
        }
        if ((isPlayer1 && currentIndex == 6) || (!isPlayer1 && currentIndex == 13)) {
            return true;
        }
        if (isPlayer1 && currentIndex >= 0 && currentIndex < 6 && board[currentIndex] == 1) {
            captureStones(currentIndex, 12 - currentIndex);
        } else if (!isPlayer1 && currentIndex >= 7 && currentIndex < 13 && board[currentIndex] == 1) {
            captureStones(currentIndex, 12 - currentIndex);
        }
        return false;
    }
    private void captureStones(int currentBin, int opponentBinIndex) {
        if (board[opponentBinIndex] > 0) {
            int storageIndex = (currentBin < 6) ? 6 : 13;
            board[storageIndex] += board[opponentBinIndex] + 1;
            board[currentBin] = 0;
            board[opponentBinIndex] = 0;
        }
    }
    public boolean isGameOver() {
        boolean player1Empty = true, player2Empty = true;

        for (int i = 0; i < 6; i++) {
            if (board[i] > 0) {
                player1Empty = false;
                break;
            }
        }

        for (int i = 7; i < 13; i++) {
            if (board[i] > 0) {
                player2Empty = false;
                break;
            }
        }

        if (player1Empty || player2Empty) {
            if (!player1Empty) {
                for (int i = 0; i < 6; i++) {
                    board[6] += board[i];
                    board[i] = 0;
                }
            }
            if (!player2Empty) {
                for (int i = 7; i < 13; i++) {
                    board[13] += board[i];
                    board[i] = 0;
                }
            }
            return true;
        }

        return false;
    }
    public int determineWinner() {
        if (board[6] > board[13]) {
            return  1;
        } else if (board[13] > board[6]) {
            return 2;
        } else {
            return  0;
        }
    }
}