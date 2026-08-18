public class MancalaGame {
    private MancalaBoard board;
    private MancalaAI aiPlayer1;
    private MancalaAI aiPlayer2;
    private boolean isPlayer1Turn;
    private static final int totalMatch =10;
    private static int player1Win=0;
    private static int player2Win=0;
    private static int tie=0;

    public MancalaGame() {
        board = new MancalaBoard();
        aiPlayer1 = new MancalaAI();
        aiPlayer2 = new MancalaAI();
        isPlayer1Turn = true;
    }

    public void playGame() {
        while (!board.isGameOver()) {
            board.printBoard();
            if (isPlayer1Turn) {
                System.out.println("Player 1's turn.");
                int bestMove = aiPlayer1.chooseBestMove(board, true);
                System.out.println("AI Player 1 chooses bin " + bestMove);
                boolean extraTurn = board.makeMove(bestMove, true);
                if (extraTurn) {
                    System.out.println("Player 1 gets an extra turn!");
                } else {
                    isPlayer1Turn = false;
                }
            } else {
                System.out.println("Player 2's turn.");
                int bestMove = aiPlayer2.chooseBestMove(board, false);
                System.out.println("AI Player 2 chooses bin " + bestMove);
                boolean extraTurn = board.makeMove(bestMove, false);
                if (extraTurn) {
                    System.out.println("Player 2 gets an extra turn!");
                } else {
                    isPlayer1Turn = true;
                }
            }
        }
        board.printBoard();
        if(board.determineWinner()==1){
            player1Win++;
        }else  if(board.determineWinner()==2){
            player2Win++;
        }else{
            tie++;
        }

    }

    public static void main(String[] args) {

        for(int i=0;i<totalMatch;i++){
            MancalaGame game = new MancalaGame();
            game.playGame();
        }

        System.out.println("Total match: "+totalMatch);
        System.out.println("Player 1 win: "+player1Win);
        System.out.println("Player 2 win: "+player2Win);
        System.out.println("Tie: "+tie);
    }
}
